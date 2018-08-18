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
        list->next = delete_energy(x, y, list->next);
        return list;
    }
}

char*	get_energy_list(EnergyCell *list)
{
	char* json = malloc(sizeof (char) * 1024);
	if (list != NULL) {
		while (list != NULL)
		{
			char* src = malloc(sizeof (char) * 256);
			sprintf(src, "{\"x\": %i, \"y\": %i, \"value\": %i}", list->x, list->y, list->value);
			strcat(json, src);
			free(src);
			list = list->next;
		}
	} else {
		sprintf(json, "{}");
	}

	return json;
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
