#ifndef DAEMON_H
#define DAEMON_H

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

extern bool        server_accepted;
extern bool        stop_process;
extern std::mutex  mtx;  

void stopAcceptClient(int sig);
void daemon();

#endif // SERVER_H