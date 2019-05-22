/*
 * adc.c
 *
 *  Created on: 2019��4��3��
 *      Author: lifim
 */


#include "adc.h"

#define         uchar       unsigned char
#define         uint        unsigned int
#define         ulong       unsigned long

bool Flag=0;                 //��־����
uint TEMP=0;                         //ADCֵ�ݴ����

void ADC_Init()
{
    //ADC12����**************************
    P6SEL |= 0x01;        //ʹ��A/Dͨ�� A0
    ADC12CTL0 = ADC12ON ; //��ADC12�ں�,��SHT0=2 (N=4)
    ADC12CTL1 = SHP ;     //SAMPCON�ź�ѡΪ������ʱ�����
    //ADC12�ڲ��ο���ѹ����
    ADC12CTL0 |= REF2_5V; //ѡ���ڲ��ο���ѹΪ2.5V
    ADC12CTL0 |= REFON;   //�ڲ��ο���ѹ��
    ADC12MCTL0 |= SREF_1; //R+=2.5V R-=VSS
    //ת������
    ADC12CTL0 |= ENC ;    //ת������(������)
}
void Data_do(uint temp_d)
{
    uint temp_1,temp_2;
    ulong temp_3;

    temp_3=(ulong)(temp_d)*250;            //ת����ʽ��ADC������ת��Ϊ��ѹ��С��ע����������
    temp_d=temp_3/4095;                    //12λ���ȣ�����4095
#if 0
    A1=temp_d/100;                        //�ֳ��٣�ʮ���͸�λ
    temp_1=temp_d%100;
    A2=temp_1/10;
    temp_2=temp_1%10;
    A3=temp_2;
#endif
}

