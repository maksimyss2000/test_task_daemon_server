#include "client.h"

#define HELP_INFO               "Usage: server.out FILE [IP] [PORT]\n"\
                                "With no IP and no PORT, use default PORT and default IP\n"
#define FATAL_ERROR             "client: fatal error: no input files\n"\
                                "use \"-h\" option for help information\n"

void helpInfo(int argc, char *argv[]) {
    if (argc <= 2) {
        if (argc < 2) {
            std::cout << FATAL_ERROR; 
            exit(EXIT_FAILURE);
        }
        if (!strcmp(argv[1], "-h")) {
        std::cout << HELP_INFO;
        exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]){
    helpInfo(argc, argv);
    Client client(argc, argv);
    client.sendFile();
    return 0;
}
