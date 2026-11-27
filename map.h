#ifndef MAP_H
#define MAP_H

typedef struct Map {
    char **grid;
    char **traps;
    int width;
    int height;
    int floors_passed;
} Map;

Map* generate_floor(int depth);
void free_map(Map* map);
void print_map(Map* map, int depth);
void print_hud(Map* map, int depth);

#endif