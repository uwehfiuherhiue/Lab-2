#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "equipment.h"
#include "time_system.h"
#include "inventory.h"
#include "adventure.h"
#include "map.h"

void create_or_recreate_player(void) {
    if (player != NULL) {
        printf("Персонаж уже есть (адрес %p). Удалить? (1/0): ", (void*)player);
        int c; 
        if (scanf("%d", &c) != 1) { int ch; while((ch=getchar())!='\n'&&ch!=EOF){} return; }
        if (c != 1) return;
        free_player(player); 
        player = NULL;
    }
    player = (Player*)malloc(sizeof(Player));
    if (!player) { printf("Ошибка памяти!\n"); return; }
    
    player->hp = player->max_hp = 100;
    player->stamina = player->max_stamina = 50;
    player->gold = 0; 
    player->x = player->y = 0;
    
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        player->inventory[i] = 0;
    }
    
    equip_init(player->equipment);
    ask_farmer_name(player);
    printf("Персонаж создан по адресу %p\n", (void*)player);
}

void show_card(void) {
    if (player == NULL) { printf("Персонаж не создан!\n"); return; }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    printf("\n=== Карточка: %s ===\n", player->name);
    printf("HP: %d/%d | Stam: %d/%d | Gold: %d\n", player->hp, player->max_hp, player->stamina, player->max_stamina, player->gold);
    printf("Реальное время: %02d.%02d.%04d %02d:%02d\n", t->tm_mday, t->tm_mon+1, t->tm_year+1900, t->tm_hour, t->tm_min);
    
    printf("Инвентарь:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = player->inventory[i];
        printf("  Слот %d: [%d] - %s\n", i, id, (id>=0 && id<MAX_ITEMS) ? item_names[id] : "?");
    }
    
    printf("Экипировка:\n");
    for (int i = 0; i < EQUIP_SLOT_COUNT; i++) {
        EquipmentSlot *s = &player->equipment[i];
        printf("  %-7s: ", equip_type_name((EquipType)i));
        if (s->equipped) {
            printf("%s (+%dHP, +%dStam)\n", s->item.name, s->item.bonus_max_hp, s->item.bonus_max_stamina);
        } else {
            printf("(пусто)\n");
        }
    }
}
void time_menu(void) {
    while (1) {
        printf("\n[1] Часы  [2] Ждать  [3] Спать  [0] Назад\nВыбор: ");
        int c;
        if (scanf("%d", &c) != 1) { int ch; while ((ch=getchar())!='\n'&&ch!=EOF){} continue; }
        if (c == 0) return;
        if (c == 1) see_time();
        else if (c == 2) work_hours();
        else if (c == 3) sleep_rest();
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    load_items_catalog();

    while (1) {
        printf("\n=== Меню | День %d, %02d:00 | %s ===\n", current_day, current_hour, player ? player->name : "нет персонажа");
        printf("[0]  Выход\n");
        printf("[1]  Создать/пересоздать персонажа\n");
        printf("[2]  Карточка персонажа\n");
        printf("[3]  Время (часы/ждать/спать)\n");
        printf("[4]  Управление снаряжением\n");
        printf("[5]  Посмотреть инвентарь\n");
        printf("[6]  Положить предмет\n");
        printf("[7]  Выбросить предмет\n");
        printf("[8]  Очистка от мусора\n");
        printf("[9]  Сжать инвентарь\n");
        printf("[10] Поиск предмета\n");
        printf("[11] Дневник\n");
        printf("[12] Поставки\n");
        printf("[13] В приключение\n");
        printf("Выбор: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            int ch; while ((ch=getchar())!='\n'&&ch!=EOF){}
            printf("Некорректный выбор.\n"); 
            continue;
        }

        switch ((MenuCode)choice) {
            case MENU_EXIT:            
                if (player) free_player(player); 
                printf("Выход.\n"); 
                return 0;
            case MENU_CREATE_PLAYER:   create_or_recreate_player(); break;
            case MENU_CARD:            show_card(); break;
            case MENU_TIME:            time_menu(); break;
            case MENU_EQUIPMENT:       equipment_menu(); break;
            case MENU_INVENTORY_VIEW:  view_inventory(); break;
            case MENU_INVENTORY_PUT:   put_item(); break;
            case MENU_INVENTORY_REMOVE:remove_item(); break;
            case MENU_INVENTORY_CLEAR: clear_trash(); break;
            case MENU_INVENTORY_COMPRESS: compress_inventory(); break;
            case MENU_SEARCH_ITEM:     search_item_by_name(); break;
            case MENU_DIARY:           write_diary(); break;
            case MENU_SUPPLIES:        process_supplies(); break;
            case MENU_ADVENTURE:       start_adventure(); break;
            default: printf("Некорректный выбор.\n");
        }
    }
    return 0;
}