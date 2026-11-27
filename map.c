#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "player.h"
#include "time_system.h"

Map* generate_floor(int depth) {
    Map *map = (Map*)malloc(sizeof(Map));
    if (!map) return NULL;
    map->width = 5 + rand() % 26;
    map->height = 5 + rand() % 26;
    map->floors_passed = depth;
    map->grid = (char**)malloc(map->height * sizeof(char*));
    map->traps = (char**)malloc(map->height * sizeof(char*));
    for (int i = 0; i < map->height; i++) {
        map->grid[i] = (char*)malloc(map->width * sizeof(char));
        map->traps[i] = (char*)malloc(map->width * sizeof(char));
    }
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (y == 0 || y == map->height-1 || x == 0 || x == map->width-1) {
                map->grid[y][x] = '#'; map->traps[y][x] = '.';
            } else {
                int r = rand() % 100;
                if (r < 20) { map->grid[y][x] = (r < 10) ? 'T' : 'R'; map->traps[y][x] = '.'; }
                else { map->grid[y][x] = '.'; map->traps[y][x] = (rand() % 100 < 10) ? 'T' : '.'; }
            }
        }
    }
    int px, py;
    do { px = 1 + rand() % (map->width - 2); py = 1 + rand() % (map->height - 2); } while (map->grid[py][px] != '.');
    player->x = px; player->y = py; map->grid[py][px] = '@';
    int dx, dy;
    do { dx = 1 + rand() % (map->width - 2); dy = 1 + rand() % (map->height - 2); } while (map->grid[dy][dx] != '.');
    map->grid[dy][dx] = 'D';
    if (depth > 1) {
        int ux, uy;
        do { ux = 1 + rand() % (map->width - 2); uy = 1 + rand() % (map->height - 2); } while (map->grid[uy][ux] != '.' || (abs(ux-px)<3 && abs(uy-py)<3));
        map->grid[uy][ux] = 'U';
    }
    return map;
}

void free_map(Map* map) {
    if (!map) return;
    for (int i = 0; i < map->height; i++) { free(map->grid[i]); free(map->traps[i]); }
    free(map->grid); free(map->traps); free(map);
}

void print_map(Map* map, int depth) {
    printf("\n=== Глубина %d ===\n", depth);
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) printf("%c ", map->grid[y][x]);
        printf("\n");
    }
}

void print_hud(Map* map, int depth) {
    (void)map;
    printf("HP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n",
           player->hp, player->max_hp, player->stamina, player->max_stamina, player->gold, depth, current_hour);
}