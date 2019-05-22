#include <msp430.h> 
#include "global.h"
#include "main.h"


typedef struct diplay_t {

	float mag;
	float freq_aim;
	float freq_cur;

} DS_FORMAT ;

DS_FORMAT display_d;

extern bool Flag;
unsigned int result;

#define DACCS0          P5OUT &= ~BIT4          //DAC片选信号置低
#define DACCS1          P5OUT |=  BIT4          //DAC片选信号置高

uint16_t set_phase = 0, a_key_value = 0, b_key_value = 0, c_key_value = 0, d_key_value = 0;
uint16_t current_phase = 0;
uint16_t out_da_voltage = 0;

void WDT_Init()
{
	WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
}
int main(void)
{

	display_d.mag = 0.0;
	display_d.freq_aim = 0.0;
	display_d.freq_cur = 0.0;
	SYSTEM_INIT();
	WDT_Init();
	GPIO_INIT();
	key_init();
	ADC_Init();
	//UART_INIT();
	SPI_Init();
	//_EINT();                          //使能中断
	Flag=1;                             //标志位先置1
	do_tast();

	return 0;
}
// display format:
// AM: 			CM:
// Freq:  0.0Hz
void    do_tast( void )
{
	lcd1602 = lcd1602_new_dev();

	while(1) {

		key_scan();
		if(key!=0xff){

			switch(key) {
			case 1:
				a_key_value = (a_key_value + 1);
				da_out(0x03ff- (0x03ff/1024)*a_key_value);
				display_d.freq_aim = a_key_value*1000;
				if (display_d.freq_aim < 1) {
					display_d.freq_aim = 300;
				}
				display(display_d);
				if (a_key_value >= 1024) {
					a_key_value = 1024;
				}
				key=0xff;
				break;
			case 2:
				a_key_value = (a_key_value - 1);
				da_out(0x03ff- (0x03ff/1024)*a_key_value);
				display_d.freq_aim = a_key_value*1000;
				if (display_d.freq_aim < 1) {
					display_d.freq_aim = 300;
				}
				display(display_d);
				if (a_key_value == 0) {
					a_key_value = 1;
				}
				key=0xff;
				break;
			case 3:
				b_key_value = (b_key_value + 1);
				display_d.mag = b_key_value;
				display(display_d);
				if (b_key_value >= 500) {
					b_key_value = 500;
				}
				key=0xff;
				break;
			case 4:
				b_key_value = (b_key_value - 1);
				display_d.mag = b_key_value;
				display(display_d);
				if (b_key_value == 0) {
					b_key_value = 1;
				}
				key=0xff;
				break;
			}
		}

		display_d.mag = (float)adc_get()/4096.0f * 3.3;
		display(display_d);

	}
}

void display(DS_FORMAT d)
{
	char line_1[60], line_2[60];
	sprintf(line_1, "MAG:%2.2fV CF:%2.1fHz", d.mag , d.freq_cur );
	sprintf(line_2, "AF:%2.1fHz ", d.freq_aim);
	lcd1602->write_str(lcd1602, 0,0,line_1);
	lcd1602->write_str(lcd1602, 0,1,line_2);
}

uint16_t adc_get()
{
	uint16_t res;
	ADC12CTL0 |= ADC12SC;                     //sampling open,AD转换完成后(ADC12BUSY=0),ADC12SC自动复位;
	while((ADC12IFG & BIT0) == 0);            //等转换结束
	res = ADC12MEM0;
	return res;
}

void    SYSTEM_INIT( void )
{
	uint16_t i;
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
	BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
	BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
	do{
		IFG1&=~OFIFG;                   //清楚振荡器错误标志
		for(i=0;i<100;i++)
			_NOP();
	}
	while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
	IFG1&=~OFIFG;
}

void    UART_INIT( void )
{
	// UART0
	ME1     |=  UTXE0 + URXE0 ;                         // 使能USART0+1收发
	UCTL0   |=  CHAR;                                   // 8-bit character
	UTCTL0 |=   SSEL0;                                  // UCLK = ACLK
	UBR00   =   0x03;                                   // 9600波特率,最高波特率
	UBR10   =   0x00;                                   //
	UMCTL0 =    0x4A;                                   // Modulation
	UCTL0   &=  ~SWRST;                                 // 初始化UART0状态机
	IE1     |=  URXIE0;                                 // 使能接收中断

	// UART1
	ME2     |=  UTXE1 + URXE1;                          // 使能USART1的TXD和RXD
	UCTL1   |=  CHAR;                                   // 选择8-bit字符
	UTCTL1 |=   SSEL0;                                  // 驱动时钟选择ACLK
	UBR01   =   0x03;                                   // 波特率2400
	UBR11   =   0x00;
	UMCTL1 =    0x4A;                                   // 调整
	UCTL1   &= ~SWRST;                                  // 初始化UART状态机
	_EINT();
}

void uart_0_send_str( unsigned char *ptr )
{
	while( (*ptr != '\0') ) {
		while ( !( IFG1 & UTXIFG0 ) );              // TX缓存空闲？
		TXBUF0 = *ptr++;                                // 发送数据
	}
}

void    uart_0_put_char( unsigned char ch )
{
	while ( !( IFG1 & UTXIFG0 ) );              // TX缓存空闲？
	TXBUF0 = ch;                                // 发送数据
}

void    GPIO_INIT( void )
{
	// 关闭按所有的IO口
	P1DIR = 0xFF;P1OUT = 0xFF;
	P2DIR = 0xFF;P2OUT = 0xFF;
	P3DIR = 0xFF;P3OUT = 0xFF;
	P4DIR = 0xFF;P4OUT = 0xFF;
	P5DIR = 0xFF;P5OUT = 0xFF;
	P6DIR = 0xFF;P6OUT = 0xFF;
#if 0
	// LED
	P2DIR = 0x0f;                             // P2端口设置为输出
	P2OUT=0x0e;//给初始化值
	// 启动IIC占用的引脚
	// P12   <------> SCL       P13 <-----> SDA
	P1DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;                           // set SCL interface and set SDA interface.
	P1OUT |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;                           // set SCL and SDA is out.
	// 设定P21 22 23 24 为4个EEPROM的片选信号
	//P2DIR |= BIT1 + BIT2 + BIT3 + BIT4;
	//P2OUT &= ~BIT1 + ~BIT2 + ~BIT3 + ~BIT4;         // 输出全都为高电平
	// 启用SCI引脚
	P3DIR |= BIT4 + BIT5 + BIT6 + BIT7;
	P3OUT |= BIT4 + BIT5 + BIT6 + BIT7;
	P3SEL |= 0xF0;
#endif
}
extern unsigned char TEMP;
#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR(void)
{
	unsigned char j;
	while((ADC12CTL0&0x01)==1);           //如果ADC忙，则等待，否则读取ADC转换数值
	Flag = 1 ;
	TEMP = ADC12MEM0;                     //读取ADC转换值
	//Data_do(TEMP);                        //处理ADC值，用于显示
	//for(j=0;j<50;j++) {
	//Display(A1,A2,A3,A4);               //显示ADC的数据
	//}
}


