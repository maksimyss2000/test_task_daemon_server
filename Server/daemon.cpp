#include "daemon.h"

/* signal is a low-level function and it does not support any other variable transfer than global */
void stopAcceptClient(int sig){
    mtx.lock();
    if (server_accepted) {
        exit(EXIT_SUCCESS);
    } else {
    stop_process = true;    
    }
    mtx.unlock();  
}

void daemon(){
    pid_t pid;
    pid = fork();
    if (pid < 0)   
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    signal(SIGHUP,  stopAcceptClient);   /* kill -1  ID */
    signal(SIGTERM, stopAcceptClient);   /* kill -15 ID */    
    signal(SIGPIPE, SIG_IGN);            /* Trying to read from a closed/broken pipe */
    pid = fork();
        if (pid < 0)
        exit(EXIT_FAILURE);
        if (pid > 0)
        exit(EXIT_SUCCESS);
    umask(0);
    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
}