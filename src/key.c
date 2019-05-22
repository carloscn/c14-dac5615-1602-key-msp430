/*
 * key.c
 *
 *  Created on: 2019年4月3日
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
//  初始化按键IO口子程序
//*************************************************************************
void KeyPort_init()
{

    P1SEL = 0x00;                   //P1普通IO功能
    P1DIR = 0x0F;                   //P14~P17输入模式，外部电路已接上拉电阻

}

//**********************************************************************
//  键盘扫描子程序，采用逐键扫描的方式
//**********************************************************************

uchar Key_Scan(void)
{
    uchar key_check;
    uchar key_checkin;
    key_checkin=KeyPort;              //读取IO口状态，判断是否有键按下
    key_checkin&= 0x0F;               //读取IO口状态，判断是否有键按下
    if(key_checkin!=0xF0){
        DELAY_MS(20);                     //键盘消抖，延时20MS
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
