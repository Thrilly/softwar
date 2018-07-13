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
    char       *name;
    int         x;
    int         y;
    int         energy;
    int         looking;
    struct      s_energy_cell *next;
} EnergyCell;

typedef struct s_game_info
{
    int         map_size;
    int         game_status;
    Player      *list_players;
    EnergyCell  *list_energy_cell;
} GameInfo;

int     check_params(int argc, char *argv[]);
void    printfc(char* str, char* color);
char*   notify(int notificationType, GameInfo gameinfo);
void    print_server_state(int status, int loop, int pubPort, int cycle, int nbConn, char* msg);
Player	*add_player(Player *list, char* name, int x, int y, int energy, int looking);
void	display_player_list(Player *list);
