#include <iostream>
#include "server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: ./server <path to config file>\n");
        return 1;
    }

    Server server;
    server.runServer(argv[1]);

    return 0;
}