/*
 * da5615.c
 *
 *  Created on: 2019年4月3日
 *      Author: lifim
 */

#include "da5615.h"

#define  uchar unsigned char
void SPI_Init(void)
{



    P3SEL   |= 0x0E;                       //设置IO口为普通I/O模式，此句可省
    P3DIR  |= BIT1 + BIT3;                //DIN管脚、SCLK管脚输出模式，DOUT输入模式
    P3OUT   = 0xFF;                       //初始值0xFF

    P5SEL   = 0x00;                       //设置IO口为普通I/O模式，此句可省
    P5DIR  |= BIT4;                       //CS管脚输出模式
    P5OUT   = 0xFF;                       //初始值0xFF

    P3SEL |= 0x0E;                            //设置USART0相关IO口功能，CS片选设置为普通IO，3线SPI设置为第二功能
    P3SEL &= ~0x11;
    P3OUT |= 0x01;                            //初始化时设置CS为高，SD卡不选
    P3DIR |= 0x1B;                            //对于输入输出方向，第二功能也需要配置
    P3DIR |= BIT0;

    UCTL0 = SWRST;                            // 配置SPI接口，初始化前需先置SWRST高
    UTCTL0 = CKPH | SSEL1 | SSEL0 | STC;      // SPI时钟为SMCLK, 3线模式(STE无效), UCLK延时半个周期, 数据在上升沿输出或输入锁存
    UBR00 = 0x02;                             // 0x02: UCLK/2 (4 MHz), works also with 3 and 4
    UBR10 = 0x00;                             // -"-
    UMCTL0 = 0x00;                            // SPI模式下，必须设置该寄存器为00
    UCTL0 = CHAR | SYNC | MM | SWRST;         // 8位SPI主机模式、复位状态
    UCTL0 &= ~SWRST;                          // 配置完后清零SWRST，必须这样的过程，否则可能出现不可预料错误
    ME1 |= USPIE0;                            // 使能USART0为SPI模式，允许
    while (!(IFG1 & UTXIFG0));                // USART0发送缓冲器空
}

//*************************************************************************
//          SPI发送数据函数
//*************************************************************************
void spiSendByte(uchar data)
{
    TXBUF0 = data;                              //写数据到发送缓冲器
    while ((IFG1&UTXIFG0) ==0);                 // 如果未准备好，则等待
    IFG1&=~UTXIFG0;
}

void da_out(unsigned int dat)
{
    dat = dat<<2;
    P5OUT &= ~BIT4;
    dat=(dat&0xff00);               //提取数据的高8位
    dat>>=8;                            //高8位移到低8位，便于赋值
    spiSendByte(dat);
    spiSendByte(dat&0x00ff);             //写数据的低8位到SPI数据寄存器
    P5OUT |= BIT4;

}
