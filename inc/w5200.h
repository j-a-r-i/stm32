#ifndef __W5200_H
#define __W5200_H


// see http://www.wiznet.co.kr/UpLoad_Files/ReferenceFiles/W5200_DS_V130K.pdf

// Common registers
//
#define W5200_MR    0x0000  // Mode register
#define W5200_GAR1  0x0001  // Gateway IP address register
#define W5200_GAR2  0x0002
#define W5200_GAR3  0x0003
#define W5200_GAR4  0x0004
#define W5200_SUBR1 0x0005 // subnet mask register
#define W5200_SUBR2 0x0006
#define W5200_SUBR3 0x0007
#define W5200_SUBR4 0x0008
#define W5200_SHAR1 0x0009 // source hardware address register
#define W5200_SHAR1 0x0009
#define W5200_SHAR2 0x000A
#define W5200_SHAR3 0x000B
#define W5200_SHAR4 0x000C
#define W5200_SHAR5 0x000D
#define W5200_SHAR6 0x000E
#define W5200_SIPR1 0x000F // source IP address register
#define W5200_SIPR2 0x0010
#define W5200_SIPR3 0x0011 
#define W5200_SIPR4 0x0012 
#define W5200_IR    0x0015 // Interrupt Register
#define W5200_IMR   0x0016 // Interrupt Mask Register
#define W5200_RTR1  0x0017 // Retry Time-value Register
#define W5200_RTR2  0x0018
#define W5200_RCR   0x0019 // Retry Count Register


// Socket registers
//
#define W5200_S1_MR     0x4100
#define W5200_S1_CR     0x4101
#define W5200_S1_IR     0x4102
#define W5200_S1_SR     0x4103
#define W5200_S1_PORT1  0x4104
#define W5200_S1_PORT2  0x4105

// Socket registers bits
//
#define W5200_Sn_CR_OPEN		0x01
#define W5200_Sn_CR_LISTEN		0x02
#define W5200_Sn_CR_CONNECT	    0x04
#define W5200_Sn_CR_DISCON		0x08
#define W5200_Sn_CR_CLOSE		0x10
#define W5200_Sn_CR_SEND		0x20
#define W5200_Sn_CR_SEND_MAC    0x21
#define W5200_Sn_CR_SEND_KEEP  	0x22
#define W5200_Sn_CR_RECV		0x40

#define W5200_Sn_IR_CON         0x01
#define W5200_Sn_IR_DISCON      0x02
#define W5200_Sn_IR_RECV        0x04
#define W5200_Sn_IR_TIMEOUT     0x08
#define W5200_Sn_IR_SENDOK      0x10


// Interface
//
extern void w5200_init(void);
extern void w5200_write(uint16_t reg, uint8_t value);
extern uint8_t w5200_read(uint16_t reg);

#endif
