#include "global.h"
#include "functions.c"

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
        // gameinfo_to_json(gameinfo);
        // puts("\x1B[2J");
        char message[1024] = "#general: TEST\n\0";
        printf("%s", message);
        zstr_sendf(chat_srv_socket, "%s", message);
        usleep(atoi(CYCLE));

    }
    zsock_destroy(&chat_srv_socket);
    return 0;
}
