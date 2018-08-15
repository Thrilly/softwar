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

#define NOTIFICATION_CYCLE_INFO     1
#define NOTIFICATION_GAME_STARTED   2
#define NOTIFICATION_GAME_FINISHED  3
#define NOTIFICATION_CLIENT_DEAD    4
#define NOTIFICATION_CLIENT_WIN     5

#define LEFT                        0
#define UP                          0
#define RIGHT                       0
#define DOWN                        0

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
Player	    *add_player(Player *list, char* name, int x, int y, int energy, int looking);
char*        get_player_list(Player *list);
Player	    *delete_player(char* name, Player *list);
int         edit_energy(char* name, int value, Player *list);
int	        move(char* name, int direction, int value, int map_size, Player *list);

// Energy functions
EnergyCell	*add_energy(EnergyCell *list, int x, int y, int value);
EnergyCell	*delete_energy(int x, int y, EnergyCell *list);
char* 	    get_energy_list(EnergyCell *list);
