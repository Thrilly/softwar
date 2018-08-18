//  check_params.c
//  softwar
//
//  Created by Brice Fouepe on 23/06/2018.
//  Copyright © 2018 Brice Fouepe. All rights reserved.
//

#include <czmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

void printfc(char* str, char* color)
{
    if (strcmp(color,"green") == 0) {
        printf("\x1B[32m%s", str);
    } else if(strcmp(color,"red") == 0){
        printf("\x1B[31m%s", str);
    } else if(strcmp(color,"blue") == 0){
        printf("\x1B[36m%s", str);
    } else if(strcmp(color,"yellow") == 0){
        printf("\x1B[33m%s", str);
    }
    printf("\x1B[0m");
}

char* notify(int notificationType, GameInfo gameinfo, Player* list_players, EnergyCell* list_energy)
{
    char* json = malloc(sizeof (char) * 1024);
    if (1 == 2) {
        sprintf(json, "{\"notification_type\": %i, \"data\" : {\"players\": %s, \"energy_cells\" : %s}}\n",notificationType, get_player_list(list_players), get_energy_list(list_energy));
        sprintf(json, "{\"notification_type\": %i, \"data\" : {\"map_size\": %i, \"game_status\" : %i}}\n",notificationType, gameinfo.map_size, gameinfo.game_status);
    } else {
        sprintf(json, "{\"notification_type\": %i, \"data\" : {\"players\": %s, \"energy_cells\" : %s}}\n",notificationType, get_player_list(list_players), get_energy_list(list_energy));
    }
    return json;
}

void print_server_state(int status, int loop, int pubPort, int cycle, int nbConn, char* msg){
    printf("\e[1;1H\e[2J");
    printf("########## SOFTWAR SERVER V1.0 ##########\n\n");
    printf("STATUS     | ");
    if (status == 1) { printfc("RUNNING\n", "green"); }
    else if (status == 2) { printfc("PAUSE\n", "yellow"); }
    else if (status == 3) { printfc("STOPPED\n", "red"); }
    printf("PKT SENDED | %i\n", loop);
    printf("PUB PORT   | %i\n", pubPort);
    printf("CYCLE      | %i ms\n", cycle);
    printf("NB CONN    | %i\n", nbConn);
    printf("MSG        | ");
    printfc(msg, "blue");
    printf("\n");
    printf("##########################################\n");
}

char *substr(char *src,int pos,int len) {
  char *dest=NULL;
  if (len>0) {
    dest = (char *) malloc(len+1);
    strncat(dest,src+pos,len);
  }
  return dest;
}

char* exec_command(char* id_ia, char* command){
    printf("%s\n", substr(command, 9, 14));
    if (strcmp(substr(command, 0, 9), "identify|")) {
        if (strlen(command) == 14 ) {
            char* ia = substr(command, 9, 5);
            printf("%s\n", ia);
            printf("%s", id_ia);
            return "MSG RECU";
        }
    }
    // return command;
    //     char* msg =
    // } else if (/* condition */) {
    //     /* code */
    // } else {
    //     /* code */
    // }
    return "";
}
