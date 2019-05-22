/*
 * key.h
 *
 *  Created on: 2019年4月3日
 *      Author: lifim
 */

#ifndef INC_KEY_H_
#define INC_KEY_H_


#include <msp430.h>
#define KeyPort         P1IN                              //独立键盘接在P10~P13

extern unsigned char key_scan();
extern void key_init();
extern unsigned char key;

#define                 CPU_F                               ((double)8000000)
#define                 DELAY_US(x)                         __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define                 DELAY_MS(x)                         __delay_cycles((long)(CPU_F*(double)x/1000.0))

#endif /* INC_KEY_H_ */
