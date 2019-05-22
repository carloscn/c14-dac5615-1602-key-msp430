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

#define DACCS0          P5OUT &= ~BIT4          //DACƬѡ�ź��õ�
#define DACCS1          P5OUT |=  BIT4          //DACƬѡ�ź��ø�

uint16_t set_phase = 0, a_key_value = 0, b_key_value = 0, c_key_value = 0, d_key_value = 0;
uint16_t current_phase = 0;
uint16_t out_da_voltage = 0;

void WDT_Init()
{
	WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
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
	//_EINT();                          //ʹ���ж�
	Flag=1;                             //��־λ����1
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
	ADC12CTL0 |= ADC12SC;                     //sampling open,ADת����ɺ�(ADC12BUSY=0),ADC12SC�Զ���λ;
	while((ADC12IFG & BIT0) == 0);            //��ת������
	res = ADC12MEM0;
	return res;
}

void    SYSTEM_INIT( void )
{
	uint16_t i;
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
	BCSCTL1&=~XT2OFF;                 //��XT2����
	BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
	do{
		IFG1&=~OFIFG;                   //������������־
		for(i=0;i<100;i++)
			_NOP();
	}
	while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
	IFG1&=~OFIFG;
}

void    UART_INIT( void )
{
	// UART0
	ME1     |=  UTXE0 + URXE0 ;                         // ʹ��USART0+1�շ�
	UCTL0   |=  CHAR;                                   // 8-bit character
	UTCTL0 |=   SSEL0;                                  // UCLK = ACLK
	UBR00   =   0x03;                                   // 9600������,��߲�����
	UBR10   =   0x00;                                   //
	UMCTL0 =    0x4A;                                   // Modulation
	UCTL0   &=  ~SWRST;                                 // ��ʼ��UART0״̬��
	IE1     |=  URXIE0;                                 // ʹ�ܽ����ж�

	// UART1
	ME2     |=  UTXE1 + URXE1;                          // ʹ��USART1��TXD��RXD
	UCTL1   |=  CHAR;                                   // ѡ��8-bit�ַ�
	UTCTL1 |=   SSEL0;                                  // ����ʱ��ѡ��ACLK
	UBR01   =   0x03;                                   // ������2400
	UBR11   =   0x00;
	UMCTL1 =    0x4A;                                   // ����
	UCTL1   &= ~SWRST;                                  // ��ʼ��UART״̬��
	_EINT();
}

void uart_0_send_str( unsigned char *ptr )
{
	while( (*ptr != '\0') ) {
		while ( !( IFG1 & UTXIFG0 ) );              // TX������У�
		TXBUF0 = *ptr++;                                // ��������
	}
}

void    uart_0_put_char( unsigned char ch )
{
	while ( !( IFG1 & UTXIFG0 ) );              // TX������У�
	TXBUF0 = ch;                                // ��������
}

void    GPIO_INIT( void )
{
	// �رհ����е�IO��
	P1DIR = 0xFF;P1OUT = 0xFF;
	P2DIR = 0xFF;P2OUT = 0xFF;
	P3DIR = 0xFF;P3OUT = 0xFF;
	P4DIR = 0xFF;P4OUT = 0xFF;
	P5DIR = 0xFF;P5OUT = 0xFF;
	P6DIR = 0xFF;P6OUT = 0xFF;
#if 0
	// LED
	P2DIR = 0x0f;                             // P2�˿�����Ϊ���
	P2OUT=0x0e;//����ʼ��ֵ
	// ����IICռ�õ�����
	// P12   <------> SCL       P13 <-----> SDA
	P1DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;                           // set SCL interface and set SDA interface.
	P1OUT |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;                           // set SCL and SDA is out.
	// �趨P21 22 23 24 Ϊ4��EEPROM��Ƭѡ�ź�
	//P2DIR |= BIT1 + BIT2 + BIT3 + BIT4;
	//P2OUT &= ~BIT1 + ~BIT2 + ~BIT3 + ~BIT4;         // ���ȫ��Ϊ�ߵ�ƽ
	// ����SCI����
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
	while((ADC12CTL0&0x01)==1);           //���ADCæ����ȴ��������ȡADCת����ֵ
	Flag = 1 ;
	TEMP = ADC12MEM0;                     //��ȡADCת��ֵ
	//Data_do(TEMP);                        //����ADCֵ��������ʾ
	//for(j=0;j<50;j++) {
	//Display(A1,A2,A3,A4);               //��ʾADC������
	//}
}


