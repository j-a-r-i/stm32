#include <stdio.h>
#include "hw.h"
#include "queue.h"

int main(int argc, char** argv)
{
#define SIZE 5
	queue_t queue;
	uint8_t data[SIZE];
	uint8_t i;

	queue_init(&queue,SIZE,data);

	for (i=1; i<8; i++)
		queue_push(&queue, i);

	while (queue_some(queue)) {
		printf("%d\n", queue_pop(&queue));
	}
}
