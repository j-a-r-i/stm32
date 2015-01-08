#ifndef __NMENU_H
#define __NMENU_H

#define NMENU_MAX_DIGIT 4

typedef struct nmenu {
	uint8_t size;
	uint8_t current;
	uint8_t digit[NMENU_MAX_DIGIT];
} nmenu_t;

#define MENU_DONE     1 // return value to nmenu_select
#define MENU_NOT_DONE 0


extern void nmenu_init(nmenu_t *menu, uint8_t size, uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4);
extern void nmenu_up(nmenu_t *menu);
extern void nmenu_down(nmenu_t *menu);
extern uint8_t nmenu_select(nmenu_t *menu);


#endif
