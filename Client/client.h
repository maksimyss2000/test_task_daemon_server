#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <thread>
#include <string>
#include <chrono>
#include <fstream>
#include <memory>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CONNECT_LIMIT_ERROR     "client: connect limit error: time connect limit exceeded\n"
#define OPEN_FILE_ERROR         "client: open file error: unable to open file\n"
#define DEFAULT_PORT            4001
#define DEFAULT_IP              "127.0.0.10"
#define CONNECT_ITERATE_LIMIT   10
#define BUF_SIZE                1024          

class Client {
private:
    int                      sock;
    int                      bytes_count;
    int                      port;
    struct sockaddr_in       server_address;
    std::string              ip;
    std::string              name_file;
    std::unique_ptr<char[]>  buf;
    void                     tryConnect();
    void                     recognizeArgument(int argc, char *argv[]);
    void                     sendPartFile(char* part);
    void                     sendNameFile();
    std::ifstream            openFile();

public:
                             Client(int argc, char *argv[]);
    void                     sendFile(); 
};

#endif // CLIENT_H
