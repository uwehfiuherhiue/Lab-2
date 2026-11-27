#include <stdio.h>
#include "time_system.h"
#include "player.h"

int current_day = 1;
int current_hour = 8;

int is_night_02(void) { return current_hour == 2; }

void see_time(void) { printf("Текущее время: День %d, %02d:00\n", current_day, current_hour); }

void work_hours(void) {
    int hours;
    printf("Введите количество часов для работы: ");
    if (scanf("%d", &hours) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF) {} printf("Некорректное значение.\n"); return; }
    if (hours < 1) { printf("Нужно не менее 1 часа.\n"); return; }
    int new_hour = current_hour + hours;
    int new_day = current_day;
    while (new_hour >= 24) { new_hour -= 24; new_day++; }
    int crossed_night = 0;
    if (new_day > current_day || new_hour > 2) {
        if (current_hour < 2 || new_day > current_day) crossed_night = 1;
    }
    if (current_hour >= 2 && new_day > current_day && new_hour > 2) crossed_night = 1;
    if (crossed_night && !(new_hour == 2 && new_day == current_day + 1)) {
        printf("Нельзя проматывать время через 02:00! Фермер должен спать.\n");
        return;
    }
    current_hour = new_hour;
    current_day = new_day;
    printf("Время: День %d, %02d:00\n", current_day, current_hour);
}

void sleep_rest(void) {
    current_day++;
    current_hour = 8;
    if (player) {
        player->hp = player->max_hp;
        player->stamina = player->max_stamina;
        printf("Вы отдохнули. HP и выносливость восстановлены.\n");
    }
    printf("Наступил День %d, 08:00\n", current_day);
}