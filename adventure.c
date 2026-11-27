#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "adventure.h"
#include "map.h"
#include "player.h"
#include "time_system.h"

#define GOLD_MULTIPLIER 10

static void emergency_evacuation(void) {
    int lost = player->gold / 2;
    player->gold -= lost;
    printf("\n=== ЭВАКУАЦИЯ ===\nГерой упал без сил! Спасатели забрали %d золота.\n", lost);
}

static int check_emergency(void) {
    if (player->stamina <= 0) { emergency_evacuation(); return 1; }
    if (is_night_02()) { printf("\n02:00 — пора спать!\n"); emergency_evacuation(); return 1; }
    return 0;
}

void start_adventure(void) {
    if (player == NULL) { printf("Сначала создайте персонажа!\n"); return; }
    if (player->stamina <= 0) { printf("Нет сил!\n"); return; }
    if (is_night_02()) { printf("02:00 — спать!\n"); return; }
    
    int depth = 1;
    Map* map = generate_floor(depth);
    if (!map) return;
    
    printf("\nВы в приключении! Глубина: %d\n", depth);
    char input[10];
    
    while (1) {
        print_map(map, depth);
        print_hud(map, depth);
        printf("Ход: ");
        if (scanf("%9s", input) != 1) break;
        char cmd = toupper(input[0]);
        
        if (cmd == 'Q') {
            if (map->grid[player->y][player->x] == 'U') { printf("Вы выбрались!\n"); free_map(map); return; }
            else { printf("Выход только на U!\n"); continue; }
        }
        if (cmd != 'W' && cmd != 'A' && cmd != 'S' && cmd != 'D') { printf("Неверно.\n"); continue; }
        
        int old_x = player->x, old_y = player->y;
        int next_x = old_x, next_y = old_y;
        
        if (cmd == 'W') next_y--; if (cmd == 'S') next_y++;
        if (cmd == 'A') next_x--; if (cmd == 'D') next_x++;
        
        char target = map->grid[next_y][next_x];
        if (target == '#' || target == 'T' || target == 'R') { printf("Препятствие.\n"); continue; }
        
        if (target == 'D') {
            free_map(map); depth++;
            player->stamina -= 5; current_hour = (current_hour + 1) % 24; if (current_hour == 0) current_day++;
            player->gold += GOLD_MULTIPLIER * depth;
            if (check_emergency()) return;
            map = generate_floor(depth); if (!map) return;
            continue;
        }
        if (target == 'U') {
            free_map(map); depth--; if (depth < 1) depth = 1;
            map = generate_floor(depth); if (!map) return;
            continue;
        }

        if (map->traps[next_y][next_x] == 'T') {
            if (map->grid[next_y][next_x] == '.' || map->grid[next_y][next_x] == '@') {
                map->grid[old_y][old_x] = (map->traps[old_y][old_x] == 'T') ? ',' : '.';
                player->x = next_x; player->y = next_y;
                map->grid[next_y][next_x] = '@';
                player->stamina -= 1;
                printf("\nСкрипучий пол!\n");
                if (check_emergency()) { free_map(map); return; }
                continue;
            } else if (map->grid[next_y][next_x] == ',') {
                if (rand() % 100 < 50) {
                    printf("\nПрошли дальше.\n");
                    map->grid[old_y][old_x] = (map->traps[old_y][old_x] == 'T') ? ',' : '.';
                    player->x = next_x; player->y = next_y;
                    map->grid[next_y][next_x] = '@';
                    player->stamina -= 1;
                    if (check_emergency()) { free_map(map); return; }
                } else {
                    printf("\nПол проломился!\n");
                    free_map(map); depth++;
                    player->stamina -= 5; current_hour = (current_hour + 1) % 24; if (current_hour == 0) current_day++;
                    player->gold += GOLD_MULTIPLIER * depth;
                    if (check_emergency()) return;
                    map = generate_floor(depth); if (!map) return;
                }
                continue;
            }
        } else {
            if (target != 'U') map->grid[old_y][old_x] = (map->traps[old_y][old_x] == 'T') ? ',' : '.';
            player->x = next_x; player->y = next_y;
            map->grid[next_y][next_x] = '@';
            player->stamina -= 1;
            if (check_emergency()) { free_map(map); return; }
        }
    }
    free_map(map);
}