#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#define MAX_EQUIP_NAME 64

typedef enum {
    EQUIP_HEAD = 0,
    EQUIP_BODY,
    EQUIP_FEET,
    EQUIP_SLOT_COUNT
} EquipType;

typedef struct {
    char name[MAX_EQUIP_NAME];
    EquipType type;
    int bonus_max_hp;
    int bonus_max_stamina;
} EquipItem;

typedef struct {
    EquipItem item;
    int equipped;
} EquipmentSlot;

void equip_init(EquipmentSlot slots[EQUIP_SLOT_COUNT]);
void equipment_menu(void);
int  equip_item(EquipmentSlot slots[EQUIP_SLOT_COUNT], int *max_hp, int *hp, int *max_stamina, int *stamina, EquipItem new_item);
int  unequip_item(EquipmentSlot slots[EQUIP_SLOT_COUNT], int *max_hp, int *hp, int *max_stamina, int *stamina, EquipType type);
const char* equip_type_name(EquipType t);

#endif