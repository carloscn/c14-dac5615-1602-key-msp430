/*
 * lcd1602.c
 *
 *  Created on: 2019年4月1日
 *      Author: lifim
 */
#include <msp430.h>
#include "lcd1602.h"


LCD1602* lcd1602;

static void _lcd1602_gpio_init()
{
    P5SEL &= ~(LCD1602_PIN_RS | LCD1602_PIN_EN | LCD1602_PIN_RW );
    P5DIR |=  (LCD1602_PIN_RS | LCD1602_PIN_EN | LCD1602_PIN_RW );
    P5OUT |=  (LCD1602_PIN_RS | LCD1602_PIN_EN | LCD1602_PIN_RW );
}

static void _lcd1602_hw_gpio_set(uint16_t port)
{
    P5OUT |= port;
}

static void _lcd1602_hw_gpio_clr(uint16_t port)
{
    P5OUT &= ~port;
}

static void _lcd1602_write_byte(unsigned char data)
{
    _lcd1602_hw_gpio_set(LCD1602_PIN_RS);
    _lcd1602_hw_gpio_clr(LCD1602_PIN_RW);
    _lcd1602_hw_gpio_set(LCD1602_PIN_EN);
    LCD1602_DATA_PORT = data;
    DELAY_MS(5);
    _lcd1602_hw_gpio_clr(LCD1602_PIN_EN);
}

static void _lcd1602_write_com(unsigned char cmd)
{
    _lcd1602_hw_gpio_clr(LCD1602_PIN_RS);
    _lcd1602_hw_gpio_clr(LCD1602_PIN_RW);
    _lcd1602_hw_gpio_set(LCD1602_PIN_EN);
    LCD1602_DATA_PORT = cmd;
    DELAY_MS(5);
    _lcd1602_hw_gpio_clr(LCD1602_PIN_EN);
}

void lcd1602_lcd_clear(LCD1602* self)
{
    _lcd1602_write_com(0x01);
    DELAY_MS(5);
}

void lcd1602_write_str(LCD1602* self, unsigned char x, unsigned char y, unsigned char *s)
{
    if (y == 0) {
        _lcd1602_write_com(0x80 + x);        //第一行显示
    }
    else {
        _lcd1602_write_com(0xC0 + x);        //第二行显示
    }
    while (*s) {
        _lcd1602_write_byte( *s);
        s ++;
    }
}

void lcd1602_write_char(LCD1602* self, unsigned char x, unsigned char y, unsigned char c)
{
    if (y == 0) {
        _lcd1602_write_com(0x80 + x);        //第一行显示
    }
    else {
        _lcd1602_write_com(0xC0 + x);        //第二行显示
    }
    _lcd1602_write_byte(c);
}

void lcd1602_init(LCD1602 *self)
{
    self->hw.pin_rw = LCD1602_PIN_RW;
    self->hw.pin_rs = LCD1602_PIN_RS;
    self->hw.pin_en = LCD1602_PIN_EN;

    _lcd1602_write_com(0x38);
    DELAY_MS(5);
    _lcd1602_write_com(0x08);
    DELAY_MS(5);
    _lcd1602_write_com(0x01);
    DELAY_MS(5);
    _lcd1602_write_com(0x06);
    DELAY_MS(5);
    _lcd1602_write_com(0x0c);
    DELAY_MS(5);

}

LCD1602* lcd1602_new_dev()
{
    LCD1602* dev = (LCD1602*)malloc(sizeof(LCD1602));
    dev->init       = &lcd1602_init;
    dev->write_str  = &lcd1602_write_str;
    dev->write_char = &lcd1602_write_char;
    dev->clear      = &lcd1602_lcd_clear;
    dev->init(dev);
    return dev;
}
