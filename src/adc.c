/*
 * adc.c
 *
 *  Created on: 2019年4月3日
 *      Author: lifim
 */


#include "adc.h"

#define         uchar       unsigned char
#define         uint        unsigned int
#define         ulong       unsigned long

bool Flag=0;                 //标志变量
uint TEMP=0;                         //ADC值暂存变量

void ADC_Init()
{
    //ADC12设置**************************
    P6SEL |= 0x01;        //使用A/D通道 A0
    ADC12CTL0 = ADC12ON ; //开ADC12内核,设SHT0=2 (N=4)
    ADC12CTL1 = SHP ;     //SAMPCON信号选为采样定时器输出
    //ADC12内部参考电压设置
    ADC12CTL0 |= REF2_5V; //选用内部参考电压为2.5V
    ADC12CTL0 |= REFON;   //内部参考电压打开
    ADC12MCTL0 |= SREF_1; //R+=2.5V R-=VSS
    //转换允许
    ADC12CTL0 |= ENC ;    //转换允许(上升沿)
}
void Data_do(uint temp_d)
{
    uint temp_1,temp_2;
    ulong temp_3;

    temp_3=(ulong)(temp_d)*250;            //转换公式，ADC数字量转换为电压大小，注意数据类型
    temp_d=temp_3/4095;                    //12位精度，除以4095
#if 0
    A1=temp_d/100;                        //分出百，十，和个位
    temp_1=temp_d%100;
    A2=temp_1/10;
    temp_2=temp_1%10;
    A3=temp_2;
#endif
}

