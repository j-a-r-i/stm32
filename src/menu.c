#include "hw.h"
#include <stdio.h>
#include "menu.h"

void menu_display(menu_t *menu)
{
	menuItems_t *mi = menu->items;
	uint8_t i;

	for (i=0; i<menu->size; i++) {
		if (i == menu->current)
			lcd_str("<");
		else
			lcd_str(" ");

		lcd_str(mi->label);

		if (i == menu->current)
			lcd_str(">");
		else
			lcd_str(" ");

		mi++;
	}	
}

void menu_init(menu_t *menu, menuItems_t *items, uint8_t size)
{
	menu->items  = items;
	menu->current = 0;
	menu->size = size;

	menu_display(menu);
}

void menu_up(menu_t *menu)
{
	if (menu->current < (menu->size - 1)) {
		(menu->current)++;
		
		menu_display(menu);
	}
}

void menu_down(menu_t *menu)
{
	if (menu->current > 0) {
		(menu->current)--;
		
		menu_display(menu);
	}
}

void menu_select(menu_t *menu)
{
	uint8_t i = menu->current;
	//menuItems_t *mi = menu->items;

	lcd_str(menu->items[i].label);
}
