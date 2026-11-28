#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

Player *player = NULL;
char item_names[MAX_ITEMS][MAX_NAME_LEN];

const char* default_item_names[MAX_ITEMS] = {
    "Пусто", "Дерево", "Камень", "Семена", "Ягода",
    "Палка", "Гриб", "Яблоко", "Яйцо", "Ткань"
};

void ask_farmer_name(Player *p) {
    char buf[MAX_FARMER_NAME + 2];
    printf("Введите имя фермера (до %d символов): ", MAX_FARMER_NAME);
    if (fgets(buf, sizeof(buf), stdin)) {
        size_t len = strlen(buf);
        while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r')) buf[--len] = '\0';
        if (len == 0) strcpy(p->name, "Фермер");
        else {
            if (len > MAX_FARMER_NAME) buf[MAX_FARMER_NAME] = '\0';
            strcpy(p->name, buf);
        }
    } else strcpy(p->name, "Фермер");
    printf("Здравствуйте, %s!\n", p->name);
}

void free_player(Player *p) {
    if (!p) return;
    printf("Освобождаем память персонажа по адресу %p\n", (void*)p);
    free(p);
}