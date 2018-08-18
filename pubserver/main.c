#include "global.h"
#include "params.h"

void *thread_router(zsock_t *router) {
    while (!zsys_interrupted) {
        zmsg_t *message = zmsg_recv(router);

        if (message != NULL) {
            // char *resp_server;

            zframe_t *identity = zmsg_pop(message);
            zframe_t *identity2 = zframe_from("test");
            zframe_t *empty = zmsg_pop(message);
            zframe_t *content = zmsg_pop(message);

            // resp_server = exec_command(zframe_strdup(identity), zframe_strdup(content));

            zmsg_destroy(&message);
            printf("Content of message is : %s\n", zframe_strdup(content));

            zmsg_t *response = zmsg_new();

            // zframe_t *resp_frame = zframe_new (resp_server, strlen (resp_server));

            zmsg_prepend(response, &identity2);
            zmsg_append(response, &empty);
            zmsg_append(response, &content);

            zmsg_send(&response, router);

            zmsg_destroy(&response);

            zframe_destroy(&identity);
            zframe_destroy(&empty);
            zframe_destroy(&content);
            // zframe_destroy(&resp_frame);

        }

    }
    return router;
}

int main(int argc, char *argv[])
{
    // Starting control
    printf("[INFO] - Checking and registering params ...\n");
    sleep(1);
    if (!check_params(argc, argv)) {
        return 0;
    }

    // Players Init
    Player *list_players;
    list_players = NULL;
    // list_players = add_player(list_players, "#0100", 5, 3, 50, 2);
    // list_players = add_player(list_players, "#0200", 6, 4, 50, 1);
    // list_players = add_player(list_players, "#0300", 4, 2, 50, 3);
    // list_players = add_player(list_players, "#0400", 3, 7, 50, 4);

    //EnergyCell Init
    EnergyCell *list_energy;
    list_energy = NULL;
    // list_energy = add_energy(list_energy, 2, 4, 10);
    // list_energy = add_energy(list_energy, 1, 5, 20);

    //Game Initialisation
    GameInfo gameinfo;
    gameinfo.map_size = 5;
    gameinfo.game_status = 0;


    // Connect to sockets PUB
    zsock_t *chat_srv_socket = zsock_new(ZMQ_PUB);
    zsock_bind(chat_srv_socket, "tcp://*:%s", PUBPORT);

    // Connect to sockets SUB
    zsock_t *router = zsock_new(ZMQ_ROUTER);
    zsock_bind(router, "tcp://*:%s", REQPORT);

    // Thread
    pthread_t throuter;
    pthread_create (&throuter, NULL, (void *) thread_router, router);

    // main vars
    int loop = 0;
    int status = 1;

    // Processus
    while (!zsys_interrupted) {
        // **** PUBLICATION PART ****
        loop++;

        if (loop == 3) {
            edit_energy("#0100", 10, list_players);
        }
        if (loop == 5) {
            move("#0100", DOWN, 2, atoi(MAPSIZE), list_players);
        }

        char* msg = notify(NOTIFICATION_CYCLE_INFO, gameinfo, list_players, list_energy);
        if (status == 1) {
            print_server_state(status, loop, atoi(PUBPORT), atoi(CYCLE), 0, msg);
            zstr_sendf(chat_srv_socket, "#general: %s\n", msg);
        }
        free(msg);
        usleep(atoi(CYCLE));
        // **** END PUBLICATION PART ****
    }
    status = 3;
    print_server_state(status, loop, atoi(PUBPORT), atoi(CYCLE), 0, "- null -\n");

    destroy_energy(list_energy);
    destroy_players(list_players);
    zsock_destroy(&chat_srv_socket);
    zsock_destroy(&router);

    return 0;
}
