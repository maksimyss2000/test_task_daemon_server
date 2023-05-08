#include "client.h"

constexpr auto HELP_INFO   = "Usage: server.out FILE [IP] [PORT]\n"\
                             "With no IP and no PORT, use default PORT and default IP\n";
constexpr auto FATAL_ERROR = "client: fatal error: no input files\n"\
                             "use \"-h\" option for help information\n";

void checkArgsPrintHelp(int argc, char *argv[]) {
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
    checkArgsPrintHelp(argc, argv);
    Client client(argc, argv);
    client.sendFile();
    return 0;
}
