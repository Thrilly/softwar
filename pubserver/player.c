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

void	display_player_list(Player *list)
{
	while (list != NULL)
		{
			printf(list->name);
			printf("\n");
			list = list->next;
		}
}
