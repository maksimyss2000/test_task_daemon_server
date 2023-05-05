#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <thread>
#include <cstdio>
#include <cstring>
#include <memory>
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
#include <mutex>          

#define DEFAULT_PORT         4001
#define DEFAULT_IP           "127.0.0.10"
#define BUF_SIZE             1024            /* [1 : 1048576]  the optimum size is selected according to the specific task */
 
extern bool        server_accepted;
extern bool        stop_process;
extern std::mutex  mtx;  

class Server {
private:
    int                     server;
    int                     sock;
    int                     port;
    struct sockaddr_in      server_address;
    std::string             ip;
    std::string             part_file;
    std::string             name_file;
    std::unique_ptr<char[]> buf;
    void                    acceptClinet();
    void                    setStateServerAccepting();
    void                    setStateServerWorking();
    void                    receiveFile(std::ofstream& output_file);
    void                    recognizeArgument(int argc, char *argv[]);
    bool                    getFileName();
    std::string             getTime();
    std::ofstream           openFile();

public:
                            Server(int argc, char *argv[]);
    void                    startServer();
};

#endif // SERVER_H