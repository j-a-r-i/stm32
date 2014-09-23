#include "stm32f0xx.h"
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
	q->data[q->top] = data;
	q->top = (q->top + 1) % q->size;
}

uint8_t queue_pop(queue_t *q)
{
	uint8_t val = q->data[q->bottom];
	q->top = (q->top + 1) % q->size;	
	return val;
}

