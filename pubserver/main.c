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
    int loop = 0;
    // Loop
    while (!zsys_interrupted) {
        loop++;
        // gameinfo_to_json(gameinfo);
        printf("\e[1;1H\e[2J");
        printf("STATUS     | ");
        printfc("RUNNING\n", "green");
        printf("PKT SENDED | %i\n", loop);
        printf("PUB PORT   | %s\n", PUBPORT);
        printf("CYCLE      | %s\n", CYCLE);
        printf("NB CONN    | %i\n", 0);
        printf("MSG        | ");
        gameinfo_to_json(gameinfo);
        zstr_sendf(chat_srv_socket, "%s", "");
        usleep(atoi(CYCLE));

    }
    zsock_destroy(&chat_srv_socket);
    return 0;
}
