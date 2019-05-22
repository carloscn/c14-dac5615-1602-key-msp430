/*
 * key.c
 *
 *  Created on: 2019��4��3��
 *      Author: lifim
 */

#include "key.h"

#define    uchar unsigned char
uchar key;


inline void key_init()
{
    KeyPort_init();
}

inline unsigned char key_scan()
{
    return Key_Scan();
}
//*************************************************************************
//  ��ʼ������IO���ӳ���
//*************************************************************************
void KeyPort_init()
{

    P1SEL = 0x00;                   //P1��ͨIO����
    P1DIR = 0x0F;                   //P14~P17����ģʽ���ⲿ��·�ѽ���������

}

//**********************************************************************
//  ����ɨ���ӳ��򣬲������ɨ��ķ�ʽ
//**********************************************************************

uchar Key_Scan(void)
{
    uchar key_check;
    uchar key_checkin;
    key_checkin=KeyPort;              //��ȡIO��״̬���ж��Ƿ��м�����
    key_checkin&= 0x0F;               //��ȡIO��״̬���ж��Ƿ��м�����
    if(key_checkin!=0xF0){
        DELAY_MS(20);                     //������������ʱ20MS
        key_checkin=KeyPort;
        if(key_checkin!=0xF0)
        {
            key_check=KeyPort;
            switch (key_check & 0xF0)
            {
            case 0xE0:key=1;break;
            case 0xD0:key=2;break;
            case 0xB0:key=3;break;
            case 0x70:key=4;break;
            }

        }

    }
    else
    {
        key=0xFF;
    }
    return key;
}
