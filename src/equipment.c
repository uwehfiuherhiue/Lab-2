#include <stdio.h>
#include <string.h>
#include "equipment.h"
#include "player.h"

const char* equip_type_name(EquipType t) {
    switch (t) {
        case EQUIP_HEAD: return "Голова";
        case EQUIP_BODY: return "Тело";
        case EQUIP_FEET: return "Обувь";
        default: return "?";
    }
}

void equip_init(EquipmentSlot slots[EQUIP_SLOT_COUNT]) {
    for (int i = 0; i < EQUIP_SLOT_COUNT; i++) {
        slots[i].item.name[0] = '\0';
        slots[i].item.type = (EquipType)i;
        slots[i].item.bonus_max_hp = 0;
        slots[i].item.bonus_max_stamina = 0;
        slots[i].equipped = 0;
    }
}

int equip_item(EquipmentSlot slots[EQUIP_SLOT_COUNT], int *max_hp, int *hp, int *max_stamina, int *stamina, EquipItem new_item) {
    EquipmentSlot *slot = &slots[new_item.type];
    if (slot->equipped) {
        *max_hp -= slot->item.bonus_max_hp;
        *max_stamina -= slot->item.bonus_max_stamina;
        if (*hp > *max_hp) *hp = *max_hp;
        if (*stamina > *max_stamina) *stamina = *max_stamina;
    }
    slot->item = new_item;
    slot->equipped = 1;
    *max_hp += new_item.bonus_max_hp;
    *max_stamina += new_item.bonus_max_stamina;
    return 1;
}

int unequip_item(EquipmentSlot slots[EQUIP_SLOT_COUNT], int *max_hp, int *hp, int *max_stamina, int *stamina, EquipType type) {
    EquipmentSlot *slot = &slots[type];
    if (!slot->equipped) {
        printf("Слот %s пуст.\n", equip_type_name(type));
        return 0;
    }
    *max_hp -= slot->item.bonus_max_hp;
    *max_stamina -= slot->item.bonus_max_stamina;
    if (*hp > *max_hp) *hp = *max_hp;
    if (*stamina > *max_stamina) *stamina = *max_stamina;
    slot->item.name[0] = '\0';
    slot->item.bonus_max_hp = 0;
    slot->item.bonus_max_stamina = 0;
    slot->equipped = 0;
    return 1;
}

void equipment_menu(void) {
    if (player == NULL) { printf("Сначала создайте персонажа!\n"); return; }
    while (1) {
        printf("\n=== Снаряжение ===\n");
        for (int i = 0; i < EQUIP_SLOT_COUNT; i++) {
            EquipmentSlot *s = &player->equipment[i];
            printf("[%d] %-7s: ", i, equip_type_name((EquipType)i));
            if (s->equipped) printf("%s (+%d HP, +%d Stam)\n", s->item.name, s->item.bonus_max_hp, s->item.bonus_max_stamina);
            else printf("(пусто)\n");
        }
        printf("HP: %d/%d | Stam: %d/%d\n", player->hp, player->max_hp, player->stamina, player->max_stamina);
        printf("[1] Надеть [2] Снять [0] Назад\nВыбор: ");
        int choice;
        if (scanf("%d", &choice) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF) {} continue; }
        if (choice == 0) return;
        if (choice == 1) {
            EquipItem new_item;
            printf("Название: "); scanf("%63s", new_item.name);
            printf("Тип (0-Голова, 1-Тело, 2-Обувь): "); int t; scanf("%d", &t);
            if (t < 0 || t >= EQUIP_SLOT_COUNT) { printf("Неверный тип.\n"); continue; }
            new_item.type = (EquipType)t;
            printf("Бонус к max HP: "); scanf("%d", &new_item.bonus_max_hp);
            printf("Бонус к max Stamina: "); scanf("%d", &new_item.bonus_max_stamina);
            equip_item(player->equipment, &player->max_hp, &player->hp, &player->max_stamina, &player->stamina, new_item);
            printf("Надето: %s\n", new_item.name);
        } else if (choice == 2) {
            printf("Какой слот снять (0-Голова, 1-Тело, 2-Обувь): "); int t; scanf("%d", &t);
            if (t < 0 || t >= EQUIP_SLOT_COUNT) { printf("Неверный слот.\n"); continue; }
            if (unequip_item(player->equipment, &player->max_hp, &player->hp, &player->max_stamina, &player->stamina, (EquipType)t))
                printf("Снято.\n");
        }
    }
}