#ifndef PLAYER_H
#define PLAYER_H

#include "equipment.h"

#define INVENTORY_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 64
#define MAX_FARMER_NAME 33

typedef enum {
    MENU_EXIT = 0,
    MENU_CREATE_PLAYER,
    MENU_CARD,
    MENU_TIME,
    MENU_EQUIPMENT,
    MENU_INVENTORY_VIEW,
    MENU_INVENTORY_PUT,
    MENU_INVENTORY_REMOVE,
    MENU_INVENTORY_CLEAR,
    MENU_INVENTORY_COMPRESS,
    MENU_SEARCH_ITEM,
    MENU_DIARY,
    MENU_SUPPLIES,
    MENU_ADVENTURE
} MenuCode;

typedef struct Player {
    char name[MAX_FARMER_NAME];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x;
    int y;
    int inventory[INVENTORY_SIZE];
    EquipmentSlot equipment[EQUIP_SLOT_COUNT];
} Player;

extern char item_names[MAX_ITEMS][MAX_NAME_LEN];
extern const char* default_item_names[MAX_ITEMS];
extern Player *player;

void ask_farmer_name(Player *p);
void free_player(Player *p);

#endif