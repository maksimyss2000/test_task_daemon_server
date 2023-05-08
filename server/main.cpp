#include "server.h"
#include "daemon.h"

constexpr auto HELP_INFO = "Usage: server.out [IP] [PORT]\n"\
                           "With no IP and no PORT, use default PORT and default IP\n";

void checkArgsPrintHelp(int argc, char *argv[]){ 
    if (argc == 2 && !strcmp(argv[1], "-h")) {
        std::cout << HELP_INFO;
        exit(EXIT_SUCCESS);
    } 
}

int main(int argc, char *argv[]){
    checkArgsPrintHelp(argc, argv);
    createDaemon();
    Server server(argc, argv);
    server.start();
    return 0;
}