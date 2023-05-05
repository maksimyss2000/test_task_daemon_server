#include "client.h"

/*TODO: make a handle invalid arguments and check the port and IP*/
void Client::recognizeArgument(int argc, char *argv[]){ 
    name_file = argv[1];
    if (argc >= 4) {
        ip   = argv[2];
        port = atoi(argv[3]);
    } else {
        ip   = DEFAULT_IP;
        port = DEFAULT_PORT;
    }
}

Client::Client(int argc, char *argv[]){
    recognizeArgument(argc, argv);
    server_address.sin_port        = htons(port);
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip.c_str());
    buf = std::make_unique<char[]>(BUF_SIZE + 1);
}

std::ifstream Client::openFile(){
        std::ifstream ifs(name_file);
        if (!ifs.is_open()) {
            std::cout << OPEN_FILE_ERROR;
            exit(EXIT_FAILURE);
        }
        return ifs;
}

void Client::tryConnect(){
    int count_connect_iterate = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        /* @dbg */
        //perror("Error init socket");
        exit(EXIT_FAILURE);
    }
    while (connect(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        /* @dbg */
        //perror("connect");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        count_connect_iterate++;
        if (count_connect_iterate >=CONNECT_ITERATE_LIMIT) {
            std::cout  <<  CONNECT_LIMIT_ERROR;
            exit(EXIT_FAILURE);
        }
    }
}

void Client::sendNameFile(){
    int lenght = name_file.length();
    bytes_count = send(sock, &lenght, sizeof(int), MSG_NOSIGNAL);
    bytes_count = send(sock, name_file.c_str(), name_file.length(), MSG_NOSIGNAL);
    if (bytes_count < 0) {               
            /* @dbg */
            //perror("sendNameFile");
            exit(EXIT_FAILURE);
    }      
}  

void Client::sendFile(){
    std::ifstream input_file = openFile();
    tryConnect();
    sendNameFile();
    int read_lenght;
    while (read_lenght = input_file.readsome(buf.get(), BUF_SIZE)) {
        buf[read_lenght] = '\0';
        sendPartFile(buf.get());   

    }
    close(sock);
    input_file.close();
}

void Client::sendPartFile(char* part){
    bytes_count = send(sock, part, strlen(part), MSG_NOSIGNAL);
    if (bytes_count < 0) {
            /* @dbg */
            //perror("sendPartFile");
            exit(EXIT_FAILURE);
    }                                                    
}
