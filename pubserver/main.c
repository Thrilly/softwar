#include "global.h"
#include "params.h"

int main(int argc, char *argv[])
{
    // Starting control
    printf("[INFO] - Checking params ...\n");
    usleep(200000);
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

    Player *list_players;
    list_players = NULL;
    list_players = add_player(list_players, "#0100", 1, 6, 50, 2);
    list_players = add_player(list_players, "#0200", 6, 4, 50, 1);
    list_players = add_player(list_players, "#0300", 4, 2, 50, 3);
    list_players = add_player(list_players, "#0400", 3, 7, 50, 4);

    //EnergyCell Init
    EnergyCell *list_energy;
    list_energy = NULL;
    list_energy = add_energy(list_energy, 2, 4, 10);
    list_energy = add_energy(list_energy, 1, 5, 20);

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
        char* msg = notify(notificationType.cycle_info, gameinfo, list_players, list_energy);
        if (status == 1) {
            print_server_state(status, loop, pubPort, cycle, 0, msg);
            zstr_sendf(chat_srv_socket, "#general: %s\n", msg);
        }
        free(msg);
        usleep(atoi(CYCLE));
    }
    status = 3;
    print_server_state(status, loop, pubPort, cycle, 0, "- null -\n");

    zsock_destroy(&chat_srv_socket);
    return 0;
}
