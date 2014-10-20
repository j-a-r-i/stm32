#include "hw.h"
#include <stdio.h>
#include "nmenu.h"

void nmenu_display(nmenu_t *menu)
{
	uint8_t i;

	for (i=0; i<menu->size; i++) {
		if (i == menu->current)
			lcd_write('<');

		lcd_write('0'+menu->digit[i]);

		if (i == menu->current)
			lcd_write('>');
	}	
}

void nmenu_init(nmenu_t *menu, uint8_t size, uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
{
	menu->size = size;
    menu->current = 0;
	menu->digit[0] = digit1;
	menu->digit[1] = digit2;
	menu->digit[2] = digit3;
	menu->digit[3] = digit4;

	nmenu_display(menu);
}

void nmenu_up(nmenu_t *menu)
{
	if (menu->digit[menu->current] < 9) {
		(menu->digit[menu->current])++;
		
		nmenu_display(menu);
	}
}

void nmenu_down(nmenu_t *menu)
{
	if (menu->digit[menu->current] > 0) {
		(menu->digit[menu->current])--;
		
		nmenu_display(menu);
	}
}

uint8_t nmenu_select(nmenu_t *menu)
{
	uint8_t ret = MENU_NOT_DONE;

	if (menu->current < menu->size) {
		(menu->current)++;
		nmenu_display(menu);
	}
	else {
		lcd_str("done nmenu");
		ret = MENU_DONE;
	}
	return ret;
}
