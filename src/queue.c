#include "hw.h"
#include "queue.h"

void queue_init(queue_t *q, uint8_t size, uint8_t *data)
{
	q->top = 0;
	q->bottom = 0;
	q->size = size;
	q->data = data;
}

void queue_push(queue_t *q, uint8_t data)
{
	int next = (q->top + 1) % q->size;
	if (next == q->bottom)
		return;
	q->data[q->top] = data;
	q->top = next;
}

uint8_t queue_pop(queue_t *q)
{
	if (q->top == q->bottom) {
		return 0;
	}

	uint8_t val = q->data[q->bottom];
	q->bottom = (q->bottom + 1) % q->size;	
	return val;
}

