/*
 * da5615.c
 *
 *  Created on: 2019��4��3��
 *      Author: lifim
 */

#include "da5615.h"

#define  uchar unsigned char
void SPI_Init(void)
{



    P3SEL   |= 0x0E;                       //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
    P3DIR  |= BIT1 + BIT3;                //DIN�ܽš�SCLK�ܽ����ģʽ��DOUT����ģʽ
    P3OUT   = 0xFF;                       //��ʼֵ0xFF

    P5SEL   = 0x00;                       //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
    P5DIR  |= BIT4;                       //CS�ܽ����ģʽ
    P5OUT   = 0xFF;                       //��ʼֵ0xFF

    P3SEL |= 0x0E;                            //����USART0���IO�ڹ��ܣ�CSƬѡ����Ϊ��ͨIO��3��SPI����Ϊ�ڶ�����
    P3SEL &= ~0x11;
    P3OUT |= 0x01;                            //��ʼ��ʱ����CSΪ�ߣ�SD����ѡ
    P3DIR |= 0x1B;                            //��������������򣬵ڶ�����Ҳ��Ҫ����
    P3DIR |= BIT0;

    UCTL0 = SWRST;                            // ����SPI�ӿڣ���ʼ��ǰ������SWRST��
    UTCTL0 = CKPH | SSEL1 | SSEL0 | STC;      // SPIʱ��ΪSMCLK, 3��ģʽ(STE��Ч), UCLK��ʱ�������, �������������������������
    UBR00 = 0x02;                             // 0x02: UCLK/2 (4 MHz), works also with 3 and 4
    UBR10 = 0x00;                             // -"-
    UMCTL0 = 0x00;                            // SPIģʽ�£��������øüĴ���Ϊ00
    UCTL0 = CHAR | SYNC | MM | SWRST;         // 8λSPI����ģʽ����λ״̬
    UCTL0 &= ~SWRST;                          // �����������SWRST�����������Ĺ��̣�������ܳ��ֲ���Ԥ�ϴ���
    ME1 |= USPIE0;                            // ʹ��USART0ΪSPIģʽ������
    while (!(IFG1 & UTXIFG0));                // USART0���ͻ�������
}

//*************************************************************************
//          SPI�������ݺ���
//*************************************************************************
void spiSendByte(uchar data)
{
    TXBUF0 = data;                              //д���ݵ����ͻ�����
    while ((IFG1&UTXIFG0) ==0);                 // ���δ׼���ã���ȴ�
    IFG1&=~UTXIFG0;
}

void da_out(unsigned int dat)
{
    dat = dat<<2;
    P5OUT &= ~BIT4;
    dat=(dat&0xff00);               //��ȡ���ݵĸ�8λ
    dat>>=8;                            //��8λ�Ƶ���8λ�����ڸ�ֵ
    spiSendByte(dat);
    spiSendByte(dat&0x00ff);             //д���ݵĵ�8λ��SPI���ݼĴ���
    P5OUT |= BIT4;

}
