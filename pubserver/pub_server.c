#include <czmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "check_params.c"

int check_params(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if (!check_params(argc, argv)) {
        return 0;
    }

    // Connect to socket
    zsock_t *chat_srv_socket = zsock_new(ZMQ_PUB);
    zsock_bind(chat_srv_socket, "tcp://*:%s", PUBPORT);

    // Loop
    while (!zsys_interrupted) {
        sleep(CYCLE)
        char message[1024];

        if (!fgets(message, 1024, stdin)) {
            break;
        }
        zstr_sendf(chat_srv_socket, "%s", message);
    }
    zsock_destroy(&chat_srv_socket);
    return 0;
}
