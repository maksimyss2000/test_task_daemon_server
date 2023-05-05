#include "server.h"
#include "daemon.h"

#define HELP_INFO               "Usage: server.out [IP] [PORT]\n"\
                                "With no IP and no PORT, use default PORT and default IP\n"

void helpInfo(int argc, char *argv[]){ 
    if (argc == 2 && !strcmp(argv[1], "-h")) {
        std::cout << HELP_INFO;
        exit(EXIT_SUCCESS);
    } 
}

int main(int argc, char *argv[]){
    helpInfo(argc, argv);
    daemon();
    Server server(argc, argv);
    server.startServer();
    return 0;
}