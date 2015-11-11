#ifndef __RFM12B_H
#define __RFM12B_H

extern void rfm12b_init(void);
extern void rfm12b_send(uint8_t data);
extern uint8_t rfm12b_receive();

#endif
