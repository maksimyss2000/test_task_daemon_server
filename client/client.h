#pragma once

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

constexpr auto CONNECT_LIMIT_ERROR   =  "client: connect limit error: time connect limit exceeded\n";
constexpr auto OPEN_FILE_ERROR       =  "client: open file error: unable to open file\n";
constexpr auto DEFAULT_IP            =  "127.0.0.10";
constexpr auto DEFAULT_PORT          =  4001;
constexpr auto CONNECT_ITERATE_LIMIT =  10;
constexpr auto BUF_SIZE              =  1024;          

class Client {
private:
    int                      sock;
    int                      port;
    struct sockaddr_in       server_address;
    std::string              ip;
    std::string              name_file;
    char                     buf[BUF_SIZE +1];
    void                     tryConnect();
    void                     recognizeArgument(int argc, char *argv[]);
    void                     sendPartFile(char* part);
    void                     sendNameFile();
    std::ifstream            openFile();

public:
                             Client(int argc, char *argv[]);
    void                     sendFile(); 
};

