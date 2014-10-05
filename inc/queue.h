#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct queue {
	uint8_t *data;
	uint8_t top;
	uint8_t bottom;
	uint8_t size;
} queue_t;

void queue_init(queue_t *q, uint8_t size, uint8_t *data);
void queue_push(queue_t *q, uint8_t data);
uint8_t queue_pop(queue_t *q);

#define queue_empty(q) (q.top == q.bottom)
#define queue_some(q) (q.top != q.bottom)

#endif
