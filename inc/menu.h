#ifndef __MENU_H
#define __MENU_H

typedef void (*menu_callback_fn)(void);

typedef struct menuItems {
	const char* label;
	menu_callback_fn top;
} menuItems_t;

typedef struct menu {
	menuItems_t *items;
	uint8_t current;
	uint8_t size;
} menu_t;


extern void menu_init(menu_t *menu, menuItems_t *items, uint8_t size);
extern void menu_up(menu_t *menu);
extern void menu_down(menu_t *menu);
extern void menu_select(menu_t *menu);

#endif
