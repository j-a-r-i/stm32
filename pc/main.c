#include <stdio.h>
#include "hw.h"
#include "queue.h"
#include "menu.h"
#include "nmenu.h"

menuItems_t menu1data[] = {
	{"time",   NULL},
	{"reset",   NULL},
	{"warming", NULL},
	NULL
};

void lcd_str(const char *s)
{
	printf(s);
}
void lcd_write(char ch)
{
	printf("%c", ch);
}

int main(int argc, char** argv)
{
#define SIZE 5
	queue_t queue;
	uint8_t data[SIZE];
	uint8_t i, ch;
	uint8_t running;
	menu_t menu1;
	nmenu_t menu2;

	queue_init(&queue,SIZE,data);
#if 0
	for (i=1; i<8; i++)
		queue_push(&queue, i);

	while (queue_some(queue)) {
		printf("%d\n", queue_pop(&queue));
	}
#endif

#if 0
	running = 1;
	menu_init(&menu1, menu1data, 3);
	while (running) {
		ch = getchar();
		printf("%c %d\n", ch, ch);
		switch (ch) {
		case '1':
			menu_up(&menu1);
			break;
		case '2':
			menu_down(&menu1);
			break;
		case '3':
			menu_select(&menu1);
			break;
		case 'q':
			running=0;
			break;
		}
	}
#endif

	running = 1;
	nmenu_init(&menu2, 4, 1,2,3,4);
	while (running) {
		ch = getchar();
		//printf("%c %d\n", ch, ch);
		switch (ch) {
		case '1':
			nmenu_up(&menu2);
			break;
		case '2':
			nmenu_down(&menu2);
			break;
		case '3':
			if (nmenu_select(&menu2) == MENU_DONE) {
				running=0;
			}
			break;
		case 'q':
			running=0;
			break;
		}
	}
}
