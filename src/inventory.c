#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inventory.h"
#include "player.h"
#include "time_system.h"

void load_items_catalog(void) {
    FILE *f = fopen("items.txt", "r");
    int loaded = 0;
    if (f) {
        char line[256];
        int id = 0;
        while (id < MAX_ITEMS && fgets(line, sizeof(line), f)) {
            size_t len = strlen(line);
            while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
            if (len == 0) continue;
            int parsed_id = id;
            char *p = line;
            while (*p == ' ' || *p == '\t') p++;
            if (isdigit((unsigned char)*p)) {
                parsed_id = *p - '0'; p++;
                while (*p == ' ' || *p == '\t') p++;
                if (parsed_id < 0 || parsed_id >= MAX_ITEMS) parsed_id = id;
            }
            if (strlen(p) >= MAX_NAME_LEN) p[MAX_NAME_LEN - 1] = '\0';
            strncpy(item_names[parsed_id], p, MAX_NAME_LEN - 1);
            item_names[parsed_id][MAX_NAME_LEN - 1] = '\0';
            loaded++; id++;
        }
        fclose(f);
    }
    if (!f || loaded == 0) {
        printf("Внимание: файл items.txt не найден или пуст. Используются названия по умолчанию.\n");
        for (int i = 0; i < MAX_ITEMS; i++) {
            strncpy(item_names[i], default_item_names[i], MAX_NAME_LEN - 1);
            item_names[i][MAX_NAME_LEN - 1] = '\0';
        }
        return;
    }
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (item_names[i][0] == '\0') {
            strncpy(item_names[i], default_item_names[i], MAX_NAME_LEN - 1);
            item_names[i][MAX_NAME_LEN - 1] = '\0';
        }
    }
}

void view_inventory(void) {
    if (!player) { printf("Персонаж не создан!\n"); return; }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = player->inventory[i];
        printf("Слот %d: [%d]", i, id);
        if (id >= 0 && id < MAX_ITEMS) printf(" - %s", item_names[id]);
        printf("\n");
    }
}

void put_item(void) {
    if (!player) { printf("Персонаж не создан!\n"); return; }
    int index, item_id;
    printf("Введите индекс слота (0-%d): ", INVENTORY_SIZE - 1);
    if (scanf("%d", &index) != 1) { int c; while ((c=getchar())!='\n'&&c!=EOF){} printf("Некорректный индекс.\n"); return; }
    if (index < 0 || index >= INVENTORY_SIZE) { printf("Некорректный индекс.\n"); return; }
    printf("Введите ID предмета (0-%d): ", MAX_ITEMS - 1);
    if (scanf("%d", &item_id) != 1) { int c; while ((c=getchar())!='\n'&&c!=EOF){} printf("Некорректный ID.\n"); return; }
    if (item_id < 0 || item_id >= MAX_ITEMS) { printf("Некорректный ID.\n"); return; }
    player->inventory[index] = item_id;
    printf("Предмет %s добавлен в слот %d.\n", item_names[item_id], index);
}

void remove_item(void) {
    if (!player) { printf("Персонаж не создан!\n"); return; }
    int index;
    printf("Введите индекс слота (0-%d): ", INVENTORY_SIZE - 1);
    if (scanf("%d", &index) != 1) { int c; while ((c=getchar())!='\n'&&c!=EOF){} printf("Некорректный индекс.\n"); return; }
    if (index < 0 || index >= INVENTORY_SIZE) { printf("Некорректный индекс.\n"); return; }
    player->inventory[index] = 0;
    printf("Предмет удалён из слота %d.\n", index);
}

void clear_trash(void) {
    if (!player) { printf("Персонаж не создан!\n"); return; }
    int item_id, count = 0;
    printf("Введите ID предмета для удаления: ");
    if (scanf("%d", &item_id) != 1) { int c; while ((c=getchar())!='\n'&&c!=EOF){} printf("Некорректный ID.\n"); return; }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (player->inventory[i] == item_id) { player->inventory[i] = 0; count++; }
    }
    printf("Удалено %d предметов.\n", count);
}

void compress_inventory(void) {
    if (!player) { printf("Персонаж не создан!\n"); return; }
    printf("Инвентарь ДО сжатия:\n"); view_inventory();
    int write = 0;
    for (int read = 0; read < INVENTORY_SIZE; read++) {
        if (player->inventory[read] != 0) { player->inventory[write] = player->inventory[read]; write++; }
    }
    while (write < INVENTORY_SIZE) { player->inventory[write] = 0; write++; }
    printf("Инвентарь ПОСЛЕ сжатия:\n"); view_inventory();
}

void search_item_by_name(void) {
    if (!player) { printf("Персонаж не создан!\n"); return; }
    char query[MAX_NAME_LEN];
    printf("Введите название предмета для поиска: ");
    int c; while ((c=getchar())!='\n'&&c!=EOF) {}
    if (!fgets(query, sizeof(query), stdin)) { printf("Ошибка.\n"); return; }
    size_t len = strlen(query);
    while (len > 0 && (query[len-1]=='\n'||query[len-1]=='\r')) query[--len] = '\0';
    if (len == 0) { printf("Название не введено.\n"); return; }
    int found_id = -1;
    for (int i = 0; i < MAX_ITEMS; i++) { if (strcmp(item_names[i], query) == 0) { found_id = i; break; } }
    if (found_id == -1) { printf("Такого предмета нет.\n"); return; }
    printf("Поиск '%s' (ID %d)...\n", query, found_id);
    int count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (player->inventory[i] == found_id) { printf("  Найдено в слоте %d\n", i); count++; }
    }
    if (count == 0) printf("В рюкзаке нет.\n"); else printf("Всего: %d\n", count);
}

void write_diary(void) {
    if (!player) { printf("Персонаж не создан!\n"); return; }
    FILE *f = fopen("diary.txt", "a");
    if (!f) { printf("Не удалось открыть diary.txt.\n"); return; }
    fprintf(f, "=== Дневник ===\nИмя: %s\nВремя: День %d, %02d:00\nHP: %d/%d | Stam: %d/%d | Gold: %d\nИнвентарь:\n", player->name, current_day, current_hour, player->hp, player->max_hp, player->stamina, player->max_stamina, player->gold);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = player->inventory[i];
        fprintf(f, "  Слот %d: [%d] - %s\n", i, id, (id >= 0 && id < MAX_ITEMS) ? item_names[id] : "?");
    }
    fprintf(f, "\n"); fclose(f);
    printf("Записано в diary.txt.\n");
}

void process_supplies(void) {
    FILE *in = fopen("input.txt", "r");
    if (!in) { printf("Не удалось открыть input.txt.\n"); return; }
    char names[MAX_SUPPLY_ITEMS][MAX_SUPPLY_NAME];
    int counts[MAX_SUPPLY_ITEMS];
    int unique = 0;
    char name[MAX_SUPPLY_NAME]; int qty;
    while (fscanf(in, "%63s %d", name, &qty) == 2) {
        int found = -1;
        for (int i = 0; i < unique; i++) if (strcmp(names[i], name) == 0) { found = i; break; }
        if (found == -1) {
            if (unique < MAX_SUPPLY_ITEMS) {
                strncpy(names[unique], name, MAX_SUPPLY_NAME - 1);
                names[unique][MAX_SUPPLY_NAME - 1] = '\0';
                counts[unique] = qty; unique++;
            }
        } else counts[found] += qty;
    }
    fclose(in);
    FILE *out = fopen("output.txt", "w");
    if (!out) { printf("Не удалось открыть output.txt.\n"); return; }
    fprintf(out, "Сводная ведомость поставок:\n");
    for (int i = 0; i < unique; i++) fprintf(out, "%s: %d шт.\n", names[i], counts[i]);
    fclose(out);
    printf("Записано в output.txt. Позиций: %d\n", unique);
}