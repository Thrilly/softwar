//  check_params.c
//  softwar
//
//  Created by Brice Fouepe on 23/06/2018.
//  Copyright © 2018 Brice Fouepe. All rights reserved.
//

#include "global.h"

Player	*add_player(Player *list, char* name, int x, int y, int energy, int looking)
{
	Player				*node;

	node = malloc(sizeof(*node));
	if (node == NULL)
	{
		return (0);
	}
	node->next     = list;
	node->name     = name;
	node->x        = x;
	node->y        = y;
	node->energy   = energy;
	node->looking  = looking;
	return (node);
}


Player *delete_player(char* name, Player *list)
{
    if (list == NULL) {
		return NULL;
	}

    if (strcmp(name, list->name) == 0) {
        Player* tmp = list->next;
        free(list);
        tmp = delete_player(name, tmp);
        return tmp;
    } else {
        list->next = delete_player(name, list->next);
        return list;
    }
}

char*	get_player_list(Player *list)
{
	char* json = malloc(sizeof (char) * 1024);
	while (list != NULL)
	{
		char* src = malloc(sizeof (char) * 256);
		sprintf(src, "{\"name\": \"%s\", \"x\": %i, \"y\": %i, \"energy\": %i, \"looking\": %i}", list->name, list->x, list->y, list->energy, list->looking);
		strcat(json, src);
		free(src);
		list = list->next;
	}
	return json;
}
