//  check_params.c
//  softwar
//
//  Created by Brice Fouepe on 23/06/2018.
//  Copyright © 2018 Brice Fouepe. All rights reserved.
//

#include "global.h"

EnergyCell	*add_energy(EnergyCell *list, int x, int y, int value)
{
	EnergyCell				*node;

	node = malloc(sizeof(*node));
	if (node == NULL)
	{
		return (0);
	}
	node->next     = list;
	node->x        = x;
	node->y        = y;
	node->value    = value;
	return (node);
}

EnergyCell *delete_energy(int x, int y, EnergyCell *list)
{
    if (list == NULL) {
		return NULL;
	}
    if (x == list->x && y == list->y) {
        EnergyCell* tmp = list->next;
        free(list);
        tmp = delete_energy(x, y, tmp);
        return tmp;
    } else {
        list->next = delete_energy(list->next->x, list->next->y, list->next);
        return list;
    }
}

EnergyCell *delete_energy_by_ia_pos(char* ia_name, Player* list_players, EnergyCell *list_energy)
{
    if (list_players == NULL || list_energy == NULL) {
		return NULL;
	}

	list_players = search_player(ia_name, list_players);
	list_energy = delete_energy(list_players->x, list_players->y, list_energy);
	if (list_energy == NULL) {
		return NULL;
	}
	return list_energy;
}

char*	get_energy_list(EnergyCell *list)
{
	char* json = malloc(sizeof (char) * 1024);
	sprintf(json, "{}");
	if (list != NULL) {
		sprintf(json, " ");
		int count = 0;
		while (list != NULL)
		{
			if (count > 0) {
				strcat(json, ",");
			}
			char* src = malloc(sizeof (char) * 256);
			sprintf(src, "{\"x\": %i, \"y\": %i, \"value\": %i}", list->x, list->y, list->value);
			strcat(json, src);
			free(src);
			list = list->next;
			count++;
		}
	}

	return json;
}

int is_energy_slot(EnergyCell *list, int x, int y)
{
    if (list == NULL) {
		return -2;
	}

	while (list != NULL) {
		if (list->x == x && list->y == y) {
			return list->value;
		}
		list = list->next;
	}

	return -1;
}

EnergyCell *get_energy_slot(EnergyCell *list, int x, int y)
{
    if (list == NULL) {
		return NULL;
	}

	while (list != NULL) {
		if (list->x == x && list->y == y) {
			return list;
		}
		list = list->next;
	}

	return NULL;
}

int destroy_energy(EnergyCell *list)
{
	EnergyCell *tmp;
	while (list != NULL) {
		tmp = list;
		list = list->next;
		free(tmp);
	}
	return 1;
}
