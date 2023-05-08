#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <filesystem> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

constexpr auto BUF_SIZE     = 1024;        /* [1 : 1048576]  the optimum size is selected according to the specific task */
constexpr auto DEFAULT_PORT = 4001;
constexpr auto DEFAULT_IP   = "127.0.0.10";
 
extern bool    server_accepted;
extern bool    stop_process;

class Server {
private:
    int                     server;
    int                     sock;
    int                     port;
    char                    buf[BUF_SIZE + 1];
    struct sockaddr_in      server_address;
    std::string             ip;
    std::string             part_file;
    void                    acceptClient();
    void                    setStateServerAccepting();
    void                    setStateServerWorking();
    void                    receiveFile(std::ofstream& output_file);
    void                    recognizeArgument(int argc, char *argv[]);
    bool                    getFileName(std::string& name_file);
    std::string             getTime();
    std::ofstream           openFile(std::string& name_file);

public:
                            Server(int argc, char *argv[]);
    void                    start();
};