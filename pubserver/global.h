//  global.h
//  softwar
//
//  Created by Ordine Team on 23/06/2018.
//  Copyright © 2018 Ordine Team. All rights reserved.
//

#include <czmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define NOTIFICATION_CYCLE_INFO     1
#define NOTIFICATION_GAME_STARTED   2
#define NOTIFICATION_GAME_FINISHED  3
#define NOTIFICATION_CLIENT_DEAD    4
#define NOTIFICATION_CLIENT_WIN     5

#define LEFT                        0
#define UP                          1
#define RIGHT                       2
#define DOWN                        3
#define LEFTFWD                    -1
#define RIGHTFWD                    1
#define FORWARD                     0
#define BACKWARD                    2

#define WAIT_BEFORE_START_SERVER    0
#define WAIT_BEFORE_START_GAME      3
#define DEFAULT_PLAYER_ENERGY       50

typedef struct s_notification_type
{
    int        cycle_info;
	int        game_started;
	int        game_finished;
	int        client_dead;
	int        client_win;
} NotificationType;

typedef struct s_player
{
    char*       name;
    int         x;
    int         y;
    int         energy;
    int         looking;
    int         return_code;
    double      pa;
    struct      s_player *next;
} Player;

typedef struct s_energy_cell
{
    int         x;
    int         y;
    int         value;
    struct      s_energy_cell *next;
} EnergyCell;

typedef struct s_game_info
{
    int         map_size;
    int         game_status;
    Player      *list_players;
    EnergyCell  *list_energy_cell;
} GameInfo;


// Globals functions
int         check_params(int argc, char *argv[]);
void        printfc(char* str, char* color);
char*       notify(int notificationType, GameInfo gameinfo, Player* list_players, EnergyCell* list_energy);
void        print_server_state(int status, int loop, int pubPort, int cycle, int nbConn, char* msg);

// Playes functions
Player	    *add_player(Player *list, char* name, int map_size);
char*        get_player_list(Player *list);
Player	    *delete_player(char* name, Player *list);
int         edit_energy(char* name, int value, Player *list);
int         get_energy(char* name, Player *list_players, EnergyCell *list_energy);
int	        move(char* name, int direction, int value, int map_size, Player *list);
int         destroy_players(Player *list);
int         is_free_map_slot(Player *list, int x, int y);
Player      *search_player(char* name, Player *list);
int         player_exist(char* name, Player *list);
int         count_players(Player *list);
int	        rotate(char* name, int rotation, Player *list);
int	        get_looking(char* name, Player *list);
char*       watch(char* name, Player *list_players, EnergyCell *list_energy);
char*       get_element_in_case(int x, int y, Player* list_players, EnergyCell* list_energy);
Player      *get_player_slot(Player *list, int x, int y);
int         energy_val(char* name, Player *list);
double      time_diff(struct timeval x , struct timeval y);
int         use_action_point(char* name, double value, Player *list);
Player      *delete_player_without_energy(Player *list, zsock_t *socket, GameInfo gameinfo);
int         attack(char* name, Player *list_players);
// Energy functions
EnergyCell	*add_energy(EnergyCell *list, int x, int y, int value);
EnergyCell	*delete_energy(int x, int y, EnergyCell *list);
EnergyCell  *delete_energy_by_ia_pos(char* ia_name, Player* list_players, EnergyCell *list_energy);
EnergyCell  *get_energy_slot(EnergyCell *list, int x, int y);
int         is_energy_slot(EnergyCell *list, int x, int y);
char* 	    get_energy_list(EnergyCell *list);
int         destroy_energy(EnergyCell *list);

char        *substr(char *src,int pos,int len);
char*       exec_command(char* id_ia, char* command, Player *list_player);
void        logs(char *path, char* author, char* message, char* type);
