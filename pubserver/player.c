//  check_params.c
//  softwar
//
//  Created by Brice Fouepe on 23/06/2018.
//  Copyright © 2018 Brice Fouepe. All rights reserved.
//

// AXES :
//
//		^	Y
//		.
//		.
//		.
//		.
//		.
//		.
//   0 	-------------------------->	X

#include "global.h"

Player	*add_player(Player *list, char* name, int map_size)
{
	if (count_players(list) != 4) {
		Player				*node;
		int 				x;
		int 				test_x;
		int 				y;
		int 				test_y;
		int 				looking;
		int 				test_looking;

		node = malloc(sizeof(*node));
		if (node == NULL)
		{
			list->return_code = -1; // Malloc error
			return list;
		}

		for (int i = 0; i < 4; i++) {
			if (i == 0) {
				test_x = map_size;
				test_y = map_size;
				test_looking = DOWN;
			} else if (i == 1) {
				test_x = 0;
			} else if (i == 2) {
				test_x = map_size;
				test_y = 0;
				test_looking = UP;
			} else if (i == 3) {
				test_x = 0;
			}
			if (is_free_map_slot(list, test_x, test_y) < 1){
				x = test_x;
				y = test_y;
				looking = test_looking;
			}
		}

		if (player_exist(name, list) < 1) {
			node->next     		= list;
			node->name     		= name;
			node->x        		= x;
			node->y        		= y;
			node->energy   		= DEFAULT_PLAYER_ENERGY;
			node->looking  		= looking;
			node->pa  			= 0;
			node->return_code  	= 2; // Added successfully
			return (node);
		} else {
			list->return_code = 1; // Already Exist
			return (list);
		}
	} else {
		list->return_code = 0; // Gamefull
		return (list);
	}
}

int energy_val(char* name, Player *list){
	list = search_player(name, list);
	return list->energy;
}

Player *search_player(char* name, Player *list)
{
    if (list == NULL) {
		return NULL;
	}

	while (list != NULL) {
		if (strcmp(name, list->name) == 0) {
			return list;
		}
		list = list->next;
	}

	return NULL;
}

int player_exist(char* name, Player *list)
{
    if (list == NULL) {
		return -1;
	}

	while (list != NULL) {
		if (strcmp(name, list->name) == 0) {
			return 1;
		}
		list = list->next;
	}

	return 0;
}

int is_free_map_slot(Player *list, int x, int y)
{
    if (list == NULL) {
		return -1;
	}

	while (list != NULL) {
		if (list->x == x && list->y == y) {
			return 1;
		}
		list = list->next;
	}

	return 0;
}

Player *get_player_slot(Player *list, int x, int y)
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

int count_players(Player *list)
{
    if (list == NULL) {
		return 0;
	}

	int count = 0;

	while (list != NULL) {
		count++;
		list = list->next;
	}

	return count;
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
        list->next = delete_player(list->next->name, list->next);
        return list;
    }
}

Player *delete_player_without_energy(Player *list, zsock_t *socket, GameInfo gameinfo)
{
    if (list == NULL) {
		return NULL;
	}

    if (list->energy <= 0) {
		char *msgClientDead = malloc(sizeof (char) * 1024);
		msgClientDead = notify(NOTIFICATION_CLIENT_DEAD, gameinfo, NULL, NULL);
		zstr_sendf(socket, "#general: %s\n", msgClientDead);
		free(msgClientDead);
        Player* tmp = list->next;
        free(list);
        tmp = delete_player_without_energy(tmp, socket, gameinfo);
        return tmp;
    } else {
        list->next = delete_player_without_energy(list->next, socket, gameinfo);
        return list;
    }
}

int destroy_players(Player *list)
{
	Player *tmp;
	while (list != NULL) {
		tmp = list;
		list = list->next;
		free(tmp);
	}
	return 1;
}

char*	get_player_list(Player *list)
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
			sprintf(src, "{\"name\": \"%s\", \"x\": %i, \"y\": %i, \"energy\": %i, \"looking\": %i}", list->name, list->x, list->y, list->energy, list->looking);
			strcat(json, src);
			free(src);
			list = list->next;
			count++;
		}
	}
	return json;
}
//
int	edit_energy(char* name, int value, Player *list)
{
	while (list != NULL) {
		if (strcmp(name, list->name) == 0) {
			list->energy += value;
			if (list->energy > 100) {
				return 1;
			}else if (list->energy <= 0){
				return -1;
			}
			return 0;
		}
		list = list->next;
	}
	return -2;
}

int use_action_point(char* name, double value, Player *list){
	while (list != NULL) {
		if (strcmp(name, list->name) == 0) {
			if (list->pa <= 0) {
				return 0;
			}
			list->pa -= value;
			return 1;
		}
		list = list->next;
	}
	return -1;
}

int	get_energy(char* name, Player *list_players, EnergyCell *list_energy)
{
	while (list_players != NULL) {
		if (strcmp(name, list_players->name) == 0) {
			int value = is_energy_slot(list_energy, list_players->x, list_players->y);
			if (value >= 0) {
				list_players->energy += value;
			} else {
				return -1;
			}
			if (list_players->energy > 100) {
				return 0;
			}
			return 1;
		}
		list_players = list_players->next;
	}
	return -2;
}

char* watch(char* name, Player *list_players, EnergyCell *list_energy){
	list_players = search_player(name, list_players);
	if (list_players != NULL) {
		char* case1;
		char* case2;
		char* case3;
		char* case4;
		char* arrayStr = malloc(sizeof (char) * 1024);
		if (list_players->looking == LEFT) { // left
			case1 = get_element_in_case(list_players->x-1, list_players->y, list_players, list_energy);
			case2 = get_element_in_case(list_players->x-2, list_players->y-1, list_players, list_energy);
			case3 = get_element_in_case(list_players->x-2, list_players->y, list_players, list_energy);
			case4 = get_element_in_case(list_players->x-2, list_players->y+1, list_players, list_energy);
			sprintf(arrayStr, "[\"%s\", \"%s\", \"%s\", \"%s\"]", case1, case2, case3, case4);
		} else if (list_players->looking == UP) { // up
			case1 = get_element_in_case(list_players->x, list_players->y+1, list_players, list_energy);
			case2 = get_element_in_case(list_players->x-1, list_players->y+2, list_players, list_energy);
			case3 = get_element_in_case(list_players->x, list_players->y+2, list_players, list_energy);
			case4 = get_element_in_case(list_players->x+1, list_players->y+2, list_players, list_energy);
			sprintf(arrayStr, "[\"%s\", \"%s\", \"%s\", \"%s\"]", case1, case2, case3, case4);
		} else if (list_players->looking == RIGHT) { // right
			case1 = get_element_in_case(list_players->x+1, list_players->y, list_players, list_energy);
			case2 = get_element_in_case(list_players->x+2, list_players->y+1, list_players, list_energy);
			case3 = get_element_in_case(list_players->x+2, list_players->y, list_players, list_energy);
			case4 = get_element_in_case(list_players->x+2, list_players->y-1, list_players, list_energy);
			sprintf(arrayStr, "[\"%s\", \"%s\", \"%s\", \"%s\"]", case1, case2, case3, case4);
		} else if (list_players->looking == DOWN) { // down
			case1 = get_element_in_case(list_players->x, list_players->y-1, list_players, list_energy);
			case2 = get_element_in_case(list_players->x+1, list_players->y-2, list_players, list_energy);
			case3 = get_element_in_case(list_players->x, list_players->y-2, list_players, list_energy);
			case4 = get_element_in_case(list_players->x-1, list_players->y-2, list_players, list_energy);
			sprintf(arrayStr, "[\"%s\", \"%s\", \"%s\", \"%s\"]", case1, case2, case3, case4);
		}
		return arrayStr;
	}
	return NULL;
}

int attack(char* name, Player *list_players){
	list_players = search_player(name, list_players);
	if (list_players != NULL) {
		Player* case1;
		Player* case2;
		Player* case3;
		Player* case4;
		if (list_players->looking == LEFT) { // left
			if ((case1 = get_player_slot(list_players, list_players->x-1, list_players->y)) != NULL) { case1->pa = -2; }
			if ((case2 = get_player_slot(list_players, list_players->x-2, list_players->y-1)) != NULL) { case2->pa = -2; }
			if ((case3 = get_player_slot(list_players, list_players->x-2, list_players->y)) != NULL) { case3->pa = -2; }
			if ((case4 = get_player_slot(list_players, list_players->x-2, list_players->y+1)) != NULL) { case4->pa = -2; }
		} else if (list_players->looking == UP) { // up
			if ((case1 = get_player_slot(list_players, list_players->x, list_players->y+1)) != NULL) { case1->pa = -2; }
			if ((case2 = get_player_slot(list_players, list_players->x-1, list_players->y+2)) != NULL) { case2->pa = -2; }
			if ((case3 = get_player_slot(list_players, list_players->x, list_players->y+2)) != NULL) { case3->pa = -2; }
			if ((case4 = get_player_slot(list_players, list_players->x+1, list_players->y+2)) != NULL) { case4->pa = -2; }
		} else if (list_players->looking == RIGHT) { // right
			if ((case1 = get_player_slot(list_players, list_players->x+1, list_players->y)) != NULL) { case1->pa = -2; }
			if ((case2 = get_player_slot(list_players, list_players->x+2, list_players->y+1)) != NULL) { case2->pa = -2; }
			if ((case3 = get_player_slot(list_players, list_players->x+2, list_players->y)) != NULL) { case3->pa = -2; }
			if ((case4 = get_player_slot(list_players, list_players->x+2, list_players->y-1)) != NULL) { case4->pa = -2; }
		} else if (list_players->looking == DOWN) { // down
			if ((case1 = get_player_slot(list_players, list_players->x, list_players->y-1)) != NULL) { case1->pa = -2; }
			if ((case2 = get_player_slot(list_players, list_players->x+1, list_players->y-2)) != NULL) { case2->pa = -2; }
			if ((case3 = get_player_slot(list_players, list_players->x, list_players->y-2)) != NULL) { case3->pa = -2; }
			if ((case4 = get_player_slot(list_players, list_players->x-1, list_players->y-2)) != NULL) { case4->pa = -2; }
		}
		return 1;
	}
	return 0;
}

char* get_element_in_case(int x, int y, Player* list_players, EnergyCell* list_energy){
	list_players = get_player_slot(list_players, x, y);
	if (list_players == NULL) {
		list_energy = get_energy_slot(list_energy, x, y);
		if (list_energy == NULL) {
			return "empty";
		}
		return "energy";
	}
	return list_players->name;
}

int	move(char* name, int direction, int value, int map_size, Player *list)
{
	while (list != NULL) {
		if (strcmp(name, list->name) == 0) {
			int tmpDir = direction;
			direction += list->looking;

			if (tmpDir == -1 || tmpDir == 1) {
				list->looking += tmpDir;
				if (list->looking == -1) {
					list->looking = 3;
				} else if (list->looking == 4) {
					list->looking = 0;
				}
			}

			if (direction == -1) {
				direction = DOWN;
			} else if (direction == 4) {
				direction = LEFT;
			} else if (direction == 5) {
				direction = UP;
			}

			if (direction == LEFT) { // left
				if (list->x - value >= 0 && is_free_map_slot(list, (list->x - value), list->y) < 1) {
					list->x -= value;
					return 1;
				} else {
					return -1;
				}
			} else if (direction == UP) { // up
				if (list->y + value < map_size && is_free_map_slot(list, list->x, (list->y + value)) < 1) {
					list->y += value;
					return 1;
				} else {
					return -1;
				}
			} else if (direction == RIGHT) { // right
				if (list->x + value < map_size && is_free_map_slot(list, (list->x + value), list->y) < 1) {
					list->x += value;
					return 1;
				} else {
					return -1;
				}
			} else if (direction == DOWN) { // down
				if (list->y - value >= 0 && is_free_map_slot(list, list->x, (list->y - value)) < 1) {
					list->y -= value;
					return 1;
				} else {
					return -1;
				}
			}
		}
		list = list->next;
	}
	return 0;
}

int	rotate(char* name, int rotation, Player *list)
{
	while (list != NULL) {
		if (strcmp(name, list->name) == 0) {
			list->looking += rotation;

			if (list->looking == -1) {
				list->looking = 3;
			} else if (list->looking == 4) {
				list->looking = 0;
			}
			return 1;
		}
		list = list->next;
	}
	return 0;
}

int	get_looking(char* name, Player *list)
{
	while (list != NULL) {
		if (strcmp(name, list->name) == 0) {
			return list->looking;
		}
		list = list->next;
	}
	return -1;
}
