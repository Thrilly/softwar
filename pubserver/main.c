#include "global.h"
#include "functions.c"

int main(int argc, char *argv[])
{
    printf("[INFO] - Starting server ...\n");

    if (!check_params(argc, argv)) {
        return 0;
    }

    printf("[INFO] - Serveur listenning on tcp://*:%s \n", PUBPORT);
    printf("[INFO] - Serveur notify by %s microsecondes cycle \n", CYCLE);
    printfc("[SUCC] - Server is ready ! Start in 3 sec\n", "green");

    // Gameinfo Initialisation
    GameInfo gameinfo;
    gameinfo.map_size = 5;
    gameinfo.game_status = 0;

    for (int i = 0; i < 3; i++) {
        printfc(".\n", "green");
        sleep(1);
    }
    // Connect to socket
    zsock_t *chat_srv_socket = zsock_new(ZMQ_PUB);
    zsock_bind(chat_srv_socket, "tcp://*:%s", PUBPORT);

    // main vars
    int loop = 0;
    int status = 1;
    int pubPort = atoi(PUBPORT);
    int cycle = atoi(CYCLE);

    // Processus
    while (!zsys_interrupted) {
        loop++;
        char* msg = gameinfo_to_json(gameinfo);

        if (status == 1) {
            print_server_state(status, loop, pubPort, cycle, 0, msg);

            zstr_sendf(chat_srv_socket, "%s%i", "#general: OK\n\0", loop);
        }

        free(msg);
        usleep(atoi(CYCLE));
    }
    status = 3;
    print_server_state(status, loop, pubPort, cycle, 0, "- none -\n");

    zsock_destroy(&chat_srv_socket);
    return 0;
}
