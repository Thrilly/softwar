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
    int notif;
} NotificationType;

typedef struct s_player
{
    char*       name;
    int         x;
    int         y;
    int         energy;
    int         looking;
    struct      s_players *prev;
    struct      s_players *next;
} Players;

typedef struct s_energy_cells
{
    char       *name;
    int         x;
    int         y;
    int         energy;
    int         looking;
    struct      s_energy_cells *prev;
    struct      s_energy_cells *next;
} EnergyCell;

typedef struct s_game_info
{
    int         map_size;
    int         game_status;
    // Players     *list_players;
    // EnergyCell  *list_energy_cell;
} GameInfo;

char PUBPORT[]  = "4243";
char CYCLE[]    = "1000000";

int     check_params(int argc, char *argv[]);
void    printfc(char* str, char* color);
char*   gameinfo_to_json(GameInfo gameinfo);
void    print_server_state(int status, int loop, int pubPort, int cycle, int nbConn, char* msg);
