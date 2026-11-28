#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_SUPPLY_ITEMS 128
#define MAX_SUPPLY_NAME 64

void load_items_catalog(void);
void view_inventory(void);
void put_item(void);
void remove_item(void);
void clear_trash(void);
void compress_inventory(void);
void search_item_by_name(void);
void write_diary(void);
void process_supplies(void);

#endif