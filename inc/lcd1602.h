/*
 * lcd1602.h
 *
 *  Created on: 2019Äê4ÔÂ1ÈÕ
 *      Author: lifim
 */

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define                 CPU_F                               ((double)8000000)
#define                 DELAY_US(x)                         __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define                 DELAY_MS(x)                         __delay_cycles((long)(CPU_F*(double)x/1000.0))
// defined in msp430 p5 gpio group.
#define             LCD1602_PIN_RS              BIT5
#define             LCD1602_PIN_RW              BIT6
#define             LCD1602_PIN_EN              BIT7
#define             LCD1602_DATA_PORT           P4OUT

typedef struct lcd1602_t LCD1602;

struct lcd1602_hw_t {

    uint16_t pin_rs;
    uint16_t pin_rw;
    uint16_t pin_en;

};

struct lcd1602_t {

    LCD1602 *self;
    struct lcd1602_hw_t hw;
    uint16_t mem[20];

    void (*init)(LCD1602 *self);
    void (*write_str)(LCD1602 *self, unsigned char x, unsigned char y, unsigned char *s);
    void (*write_char)(LCD1602 *self, unsigned char x, unsigned char y, unsigned char c);
    void (*clear)(LCD1602* self);
};

extern LCD1602* lcd1602_new_dev();

extern LCD1602* lcd1602;

#endif /* INC_LCD1602_H_ */
