#include "server.h"

bool        server_accepted;
bool        stop_process;
std::mutex  mtx;  

Server::Server(int argc, char *argv[]){
    recognizeArgument(argc, argv);
    buf = std::make_unique<char[]>(BUF_SIZE + 1);
    server_address.sin_port        = htons(port);
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip.c_str());
}

/*TODO: make a handle invalid arguments and check the port and IP*/
void Server::recognizeArgument(int argc, char *argv[]){ 
    if (argc >= 3) {
        ip   = argv[1];
        port = atoi(argv[2]);
    } else {
        ip   = DEFAULT_IP;
        port = DEFAULT_PORT;
    }
}

void Server::setStateServerAccepting() {
    mtx.lock();
    server_accepted = true;
    mtx.unlock();
}

void Server::setStateServerWorking() {
    mtx.lock();
    server_accepted = false;
    mtx.unlock();
}

/* if we catch a signal, the server stops accepting new connections and the server terminates correctly */
void Server::startServer(){
    int bytes_read;
    openlog("my_server_log", LOG_NOWAIT | LOG_PID, LOG_USER);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        /* @dbg */
        //syslog(LOG_NOTICE, "Error init socket");
        exit(EXIT_FAILURE);
    }
    if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        /* @dbg */
        //syslog(LOG_NOTICE, "Error bind");
        exit(EXIT_FAILURE);
    }
    listen(sock, 1);
    stop_process = false;
    while (!stop_process) {
        setStateServerAccepting();
        server = accept(sock, NULL, NULL);
        setStateServerWorking();
        if (server < 0) {
            syslog(LOG_NOTICE, "Error accept");
        }
        acceptClinet();
    }
    close(sock);
    closelog();
}

bool Server::getFileName(){
    int lenght;
    int bytes_read;
    bytes_read = recv(server, &lenght, sizeof(int), MSG_NOSIGNAL);
    bytes_read = recv(server, buf.get(), lenght, MSG_NOSIGNAL);
    if (lenght > BUF_SIZE) {
        return false;
    }
    buf[lenght] = '\0';
    if (bytes_read < 0) {
            /* @dbg */                
            //syslog(LOG_NOTICE, "Error recv getFileName");
            return false;
    }      
    name_file = std::string(buf.get());   
    return true;
}

std::string Server::getTime(){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    return oss.str();
}

/* if the file exists, the new file of the form like "name_file(copy$count:time)" will be created */
std::ofstream  Server::openFile(){
    int count  = 0;
    std::string addition = "";
    while (true) {        
        std::string new_name = name_file + addition;
        std::filesystem::path filePath(new_name);
        if (std::filesystem::exists(filePath)) {
            addition = "(copy" + std::to_string(count) + ":" + getTime() + ")";  
            count++;
        } else {
            name_file = new_name;
            break;
        }
    }
    std::ofstream output_file(name_file);    
    return output_file;
}

void Server::receiveFile(std::ofstream& output_file){
    int bytes_read;
    while (true) {
        bytes_read = recv(server, buf.get(), BUF_SIZE, 0);
        if (bytes_read == 0) {
            /* @dbg */
            //syslog(LOG_NOTICE, "The sender closed the connection");
            break;
        }
        if (bytes_read < 0 ) {
            /* @dbg */
            //syslog(LOG_NOTICE, "Error recv socket");
            break;
        }
        buf[bytes_read] = '\0';
        part_file = std::string(buf.get());  
        output_file << part_file;
    }
}

void  Server::acceptClinet(){
    getFileName();
    std::ofstream output_file = openFile();
    receiveFile(output_file);
    output_file.close();
}
