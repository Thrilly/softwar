#include "global.h"
#include "params.h"



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

    //EnergyCell Init
    EnergyCell *list_energy;
    list_energy = NULL;
    list_energy = add_energy(list_energy, 0, 0, 60);
    // list_energy = add_energy(list_energy, 0, 1, 20);

    //Game Initialisation
    GameInfo gameinfo;
    gameinfo.map_size = 5;
    gameinfo.game_status = 0;

    // Connect to sockets PUB
    zsock_t *chat_srv_socket = zsock_new(ZMQ_PUB);
    zsock_bind(chat_srv_socket, "tcp://*:%s", PUBPORT);

    // Connect to sockets ROUTER
    zsock_t *router = zsock_new(ZMQ_ROUTER);
    zsock_bind(router, "tcp://*:%s", REQPORT);

    // Main vars
    int loop = 0;
    int time_start = 0;
    // int status = 1;

    // Cycle manager
    struct timeval t1, t2;
    double elapsedTime;
    gettimeofday(&t1, NULL);

    // Processus
    while (!zsys_interrupted) {

        // ************************ ROUTER PART ************************
        zmsg_t *message = zmsg_recv_nowait(router);
        if (message != NULL) {
            char resp_server[1024] = "";

            zframe_t *identity = zmsg_pop(message);

            zframe_t *empty = zmsg_pop(message);
            zframe_t *content = zmsg_pop(message);
            char* command = zframe_strdup(content);
            printf("%s | Command : %s\n", zframe_strdup(identity), command);

            // ******* IDENTITY ********
            if (strcmp(substr(command, 0, 9), "identify|") == 0) {
                if (gameinfo.game_status == 0) {
                    if ((int)strlen(command) >= 14 ) {
                        char* ia_name = substr(command, 9, 5);
                        printf("%s\n", ia_name);
                        list_players = add_player(list_players, ia_name, atoi(MAPSIZE));
                        printf("%i\n", list_players->return_code);
                        if (list_players->return_code == 0) {
                            strcat(resp_server, "ko|game full");
                            logs(LOGS, ia_name, "Attemting join full game", "WARN");
                        } else if (list_players->return_code == 1) {
                            strcat(resp_server, "ko|identity already exists");
                            logs(LOGS, ia_name, "Attemting join game with already used ID", "WARN");
                        } else if (list_players->return_code == 2){
                            strcat(resp_server, "ok|");
                            strcat(resp_server, ia_name);
                            logs(LOGS, ia_name, "Join game", "INFO");
                        } else {
                            strcat(resp_server, "ko|error");
                            logs(LOGS, ia_name, "Cannot add player to list, addPlayer() return error code", "ERROR");
                        }
                    } else {
                        strcat(resp_server, "ko|null");
                        logs(LOGS, "server", "Attempting to join game with bad ID", "WARN");
                    }
                } else {
                    strcat(resp_server, "ko|game already started");
                    logs(LOGS, "server", "Attempting to join game during running status", "WARN");
                }

            // ******* FORWARD ********
            } else if (gameinfo.game_status == 1){
                if (strcmp(substr(command, 0, 8), "forward|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    if (use_action_point(ia_name, 0.5, list_players) == 1) {
                        int return_code = move(ia_name, FORWARD, 1, atoi(MAPSIZE), list_players);
                        if (return_code == 1) {
                            strcat(resp_server, "ok|null");
                        } else {
                            strcat(resp_server, "ko|null");
                            logs(LOGS, ia_name, "Move error, function move() return error code", "ERROR");
                        }
                    } else {
                        strcat(resp_server, "ko|all pa used");
                        logs(LOGS, ia_name, "Not enought PA", "WARN");
                    }

                // ******* BACKWARD ********
                } else if (strcmp(substr(command, 0, 9), "backward|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    if (use_action_point(ia_name, 1, list_players) == 1) {
                        int return_code = move(ia_name, BACKWARD, 1, atoi(MAPSIZE), list_players);
                        if (return_code == 1) {
                            strcat(resp_server, "ok|null");
                        } else {
                            strcat(resp_server, "ko|null");
                            logs(LOGS, ia_name, "Move error, function move() return error code", "ERROR");
                        }
                    } else {
                        strcat(resp_server, "ko|all pa used");
                        logs(LOGS, ia_name, "Not enought PA", "WARN");
                    }

                // ******* LEFTFWD ********
                } else if (strcmp(substr(command, 0, 8), "leftfwd|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    if (use_action_point(ia_name, 1, list_players) == 1) {
                        int return_code = move(ia_name, LEFTFWD, 1, atoi(MAPSIZE), list_players);
                        if (return_code == 1) {
                            strcat(resp_server, "ok|null");
                        } else {
                            strcat(resp_server, "ko|null");
                            logs(LOGS, ia_name, "Move error, function move() return error code", "ERROR");
                        }
                    } else {
                        strcat(resp_server, "ko|all pa used");
                        logs(LOGS, ia_name, "Not enought PA", "WARN");
                    }

                // ******* RIGHTFWD ********
                } else if (strcmp(substr(command, 0, 9), "rightfwd|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    if (use_action_point(ia_name, 1, list_players) == 1) {
                        int return_code = move(ia_name, RIGHTFWD, 1, atoi(MAPSIZE), list_players);
                        if (return_code == 1) {
                            strcat(resp_server, "ok|null");
                        } else {
                            strcat(resp_server, "ko|null");
                            logs(LOGS, ia_name, "Move error, function move() return error code", "ERROR");
                        }
                    } else {
                        strcat(resp_server, "ko|all pa used");
                        logs(LOGS, ia_name, "Not enought PA", "WARN");
                    }
                // ******* RIGHT ********
                } else if (strcmp(substr(command, 0, 6), "right|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    if (use_action_point(ia_name, 0.5, list_players) == 1) {
                        int return_code = rotate(ia_name, RIGHTFWD, list_players);
                        if (return_code == 1) {
                            strcat(resp_server, "ok|null");
                        } else {
                            strcat(resp_server, "ko|null");
                            logs(LOGS, ia_name, "Rotate error, function rotate() return error code", "ERROR");
                        }
                    } else {
                        strcat(resp_server, "ko|all pa used");
                        logs(LOGS, ia_name, "Not enought PA", "WARN");
                    }

                // ******* LEFT ********
                } else if (strcmp(substr(command, 0, 5), "left|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    if (use_action_point(ia_name, 0.5, list_players) == 1) {
                        int return_code = rotate(ia_name, LEFTFWD, list_players);
                        if (return_code == 1) {
                            strcat(resp_server, "ok|null");
                        } else {
                            strcat(resp_server, "ko|null");
                            logs(LOGS, ia_name, "Rotate error, function rotate() return error code", "ERROR");
                        }
                    } else {
                        strcat(resp_server, "ko|all pa used");
                        logs(LOGS, ia_name, "Not enought PA", "WARN");
                    }

                // ******* LOOKING ********
                } else if (strcmp(substr(command, 0, 8), "looking|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    int looking = get_looking(ia_name, list_players);
                    if (looking != -1) {
                        char strLooking[12];
                        strcat(resp_server, "ok|");
                        sprintf(strLooking, "%d", looking);
                        strcat(resp_server, strLooking);
                    } else {
                        strcat(resp_server, "ko|null");
                        logs(LOGS, ia_name, "Looking error, function getlooking() return error code", "ERROR");
                    }

                // ******* GATHER ********
                } else if (strcmp(substr(command, 0, 7), "gather|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    if (use_action_point(ia_name, 0.5, list_players) == 1) {
                        int return_code = get_energy(ia_name, list_players, list_energy);
                        if (return_code == 1) {
                            list_energy = delete_energy_by_ia_pos(ia_name, list_players, list_energy);
                            strcat(resp_server, "ok|null");
                        } else if(return_code == 0) {
                            strcat(resp_server, "ok|you dead");
                            logs(LOGS, ia_name, "Player DEAD by implosion of energy", "INFO");
                            list_energy = delete_energy_by_ia_pos(ia_name, list_players, list_energy);
                            list_players = delete_player(ia_name, list_players);
                        } else if(return_code == -1) {
                            strcat(resp_server, "ok|you dead");
                            list_energy = delete_energy_by_ia_pos(ia_name, list_players, list_energy);
                            list_players = delete_player(ia_name, list_players);
                        } else {
                            strcat(resp_server, "ko|null");
                            logs(LOGS, ia_name, "Energy gather error, function get_energy() return error code", "ERROR");
                        }
                    }

                // ******* SELFID ********
                } else if (strcmp(substr(command, 0, 7), "selfid|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    strcat(resp_server, "ok|");
                    strcat(resp_server, ia_name);

                // ******* SELFSTATS ********
                } else if (strcmp(substr(command, 0, 10), "selfstats|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    char energyVal[2];
                    sprintf(energyVal, "%i", energy_val(ia_name, list_players));
                    strcat(resp_server, "ok|");
                    strcat(resp_server, energyVal);

                // ******* WATCH ********
                } else if (strcmp(substr(command, 0, 6), "watch|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    char* arrayStr = malloc(sizeof (char) * 1024);
                    strcat(arrayStr, watch(ia_name, list_players, list_energy));
                    strcat(resp_server, "ok|");
                    strcat(resp_server, arrayStr);
                    free(arrayStr);

                // ******* ATTACK ********
                } else if (strcmp(substr(command, 0, 7), "attack|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    int return_code = attack(ia_name, list_players);
                    if (return_code == 1) {
                        strcat(resp_server, "ok|null");
                    }else{
                        strcat(resp_server, "ko|error");
                        logs(LOGS, ia_name, "Attack error, function attack() return error code", "ERROR");
                    }


                // ******* INSPECT ********
                } else if (strcmp(substr(command, 0, 8), "inspect|") == 0) {
                    char* ia_name = zframe_strdup(identity);
                    if (use_action_point(ia_name, 0.5, list_players) == 1) {
                        char* ia_inspect = substr(command, 8, 5);
                        char energyVal[2];
                        sprintf(energyVal, "%i", energy_val(ia_inspect, list_players));
                        strcat(resp_server, "ok|");
                        strcat(resp_server, energyVal);
                    }
                } else {
                    strcat(resp_server, "ko|null");
                }

            } else {
                strcat(resp_server, "ko|null");
            }

            zmsg_destroy(&message);

            zmsg_t *response = zmsg_new();

            zframe_t *resp_frame = zframe_new (resp_server, strlen (resp_server));
            zframe_t *identity2 = zframe_from(zframe_strdup(identity));

            zmsg_prepend(response, &identity2);
            zmsg_append(response, &empty);
            zmsg_append(response, &resp_frame);

            zmsg_send(&response, router);

            zmsg_destroy(&response);

            zframe_destroy(&identity);
            zframe_destroy(&empty);
            zframe_destroy(&content);
            zframe_destroy(&resp_frame);
            free(command);
        }
        // ************************ END ROUTER PART ************************

        // ************************ PUBLICATION PART ************************
        gettimeofday(&t2, NULL);
        elapsedTime = time_diff(t1 , t2);
        if (elapsedTime > atoi(CYCLE)) {

            gettimeofday(&t1, NULL);
            int countPlayers = count_players(list_players);

            // PREVENT STARTING GAME
            if (gameinfo.game_status == 0 && countPlayers == 4) {
                time_start++;
                if (time_start == WAIT_BEFORE_START_GAME) {
                    gameinfo.game_status = 1;
                    char *msgGameStarted = malloc(sizeof (char) * 1024);
                    msgGameStarted = notify(NOTIFICATION_GAME_STARTED, gameinfo, list_players, list_energy);
                    zstr_sendf(chat_srv_socket, "#all: %s\n", msgGameStarted);
                    free(msgGameStarted);
                }
            } else if (gameinfo.game_status == 0 && countPlayers != 4){
                time_start = 0;
            }
            // END

            if (gameinfo.game_status == 1) {
                Player* tmp;
                tmp = list_players;
                while (tmp != NULL) {
                    if (tmp->pa > 0) {
                        tmp->pa = 1;
                    } else {
                        tmp->pa += 1;
                    }
                    tmp->energy -= 2;
                    if (tmp->energy <= 0) {
                        // tmp->next = delete_player(tmp->name, list_players);
                        char *msgClientDead = malloc(sizeof (char) * 1024);
                        msgClientDead = notify(NOTIFICATION_CLIENT_DEAD, gameinfo, NULL, NULL);
                        zstr_sendf(socket, "#all: %s\n", msgClientDead);
                        free(msgClientDead);

                    } else {
                        tmp = tmp->next;
                    }
            	}
                free(tmp);
                // list_players = delete_player_without_energy(list_players, chat_srv_socket, gameinfo);
                if (countPlayers == 1) {
                    gameinfo.game_status = 2;
                    char *msgGameStarted = malloc(sizeof (char) * 1024);
                    msgGameStarted = notify(NOTIFICATION_GAME_FINISHED, gameinfo, list_players, list_energy);
                    zstr_sendf(chat_srv_socket, "#all: %s\n", msgGameStarted);
                    free(msgGameStarted);
                    zsys_interrupted = true;
                }
            }

            loop++;
            char *msg = malloc(sizeof (char) * 1024);
            msg = notify(NOTIFICATION_CYCLE_INFO, gameinfo, list_players, list_energy);
            print_server_state(gameinfo.game_status, loop, atoi(PUBPORT), atoi(CYCLE), countPlayers, msg);
            zstr_sendf(chat_srv_socket, "#all: %s\n", msg);
            free(msg);
        }

        // ************************ END PUBLICATION PART ************************

    }
    print_server_state(-1, loop, atoi(PUBPORT), atoi(CYCLE), 0, "- null -\n");
    if (gameinfo.game_status == 2) {
        printf("%s WIN THE GAME\n", list_players->name);
    }
    destroy_energy(list_energy);
    destroy_players(list_players);

    zsock_destroy(&chat_srv_socket);
    zsock_destroy(&router);
    logs(LOGS, "server", "Server stop instance", "INFO");
    return 0;
}
