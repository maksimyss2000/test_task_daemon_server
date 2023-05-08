#include "server.h"

/* see daemon.cpp stopAcceptClient() for why a global variable is used here */
bool server_accepted;
bool stop_process;

Server::Server(int argc, char *argv[]){
    recognizeArgument(argc, argv);
    server_address.sin_port        = htons(port);
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip.c_str());
}

/* TODO#1: make a handle invalid arguments and check the port and IP */
/* TODO#2: take it out of the class and pass ip port to the constructor */
void Server::recognizeArgument(int argc, char *argv[]){ 
    if (argc >= 3) {
        ip   = argv[1];
        port = atoi(argv[2]);
    } else {
        ip   = DEFAULT_IP;
        port = DEFAULT_PORT;
    }
}

void Server::setStateServerAccepting(){
    server_accepted = true;
}

void Server::setStateServerWorking(){
    server_accepted = false;
}

/* if we catch a signal, the server stops accepting new connections and the server terminates correctly */
void Server::start(){
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
            /* @dbg */
            //syslog(LOG_NOTICE, "Error accept");
        }
        acceptClient();
    }
    close(sock);
    closelog();
}

bool Server::getFileName(std::string& name_file){
    int lenght;
    int bytes_read;
    bytes_read = recv(server, &lenght, sizeof(int), MSG_NOSIGNAL);
    if (bytes_read <= 0 || lenght > BUF_SIZE) {
        return false;
    }
    bytes_read = recv(server, buf, lenght, MSG_NOSIGNAL);
    if (bytes_read <= 0) {
        return false;
    }
    buf[lenght] = '\0';
    if (bytes_read < 0) {
            /* @dbg */                
            //syslog(LOG_NOTICE, "Error recv getFileName");
            return false;
    }      
    name_file = std::string(buf);   
    return true;
}

std::string Server::getTime(){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    return oss.str();
}
/* TODO:change while(true) to while (std::filesystem::exists(filePath)) { ... } */
/* if the file exists, the new file of the form like "name_file(copy$count:time)" will be created */
std::ofstream  Server::openFile(std::string& name_file){
    int count  = 0;
    std::string addition;
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
    return std::ofstream(name_file);    
}

/* TODO: change while(true) to while(bytes_read <= 0) */
void Server::receiveFile(std::ofstream& output_file){
    int bytes_read;
    while (true) {
        bytes_read = recv(server, buf, BUF_SIZE, 0);
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
        part_file = std::string(buf);  
        output_file << part_file;
    }
}

void  Server::acceptClient(){
    std::string name_file;
    if (!getFileName(name_file)) {
        /* @dbg */
        //syslog(LOG_NOTICE, "Error getFileName");
        return;
    }
    std::ofstream output_file = openFile(name_file);
    receiveFile(output_file);
    output_file.close();
}
