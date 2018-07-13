#include "global.h"
#include "params.h"

int main(int argc, char *argv[])
{
    // Starting control
    printf("[INFO] - Starting server ...\n");
    if (!check_params(argc, argv)) {
        return 0;
    }

    //Notification Initialisation
    NotificationType notificationType;
    notificationType.cycle_info    = 0;
	notificationType.game_started  = 1;
	notificationType.game_finished = 2;
	notificationType.client_dead   = 3;
	notificationType.client_win    = 4;

    Player *list;
    list = NULL;
    list = add_player(list, "#0100", 1, 6, 50, 2);
    list = add_player(list, "#0200", 6, 4, 50, 1);
    list = add_player(list, "#0300", 4, 2, 50, 3);
    list = add_player(list, "#0400", 3, 7, 50, 4);

    //Game Initialisation
    GameInfo gameinfo;
    gameinfo.map_size = 5;
    gameinfo.game_status = 0;


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
        char* msg = notify(notificationType.cycle_info, gameinfo);

        if (status == 1) {
            print_server_state(status, loop, pubPort, cycle, 0, msg);
            display_player_list(list);

            zstr_sendf(chat_srv_socket, "%s%i", "#general: OK\n\0", loop);
        }

        free(msg);
        usleep(atoi(CYCLE));
    }
    status = 3;
    print_server_state(status, loop, pubPort, cycle, 0, "- null -\n");

    zsock_destroy(&chat_srv_socket);
    return 0;
}
