/*
 * main.h
 *
 *  Created on: 2018Äê4ÔÂ6ÈÕ
 *      Author: weihaochen
 */

#ifndef MAIN_H_
#define MAIN_H_

void    uart_0_send_str( unsigned char *ptr ) ;
void    GPIO_INIT( void );
void    UART_INIT( void );
void    do_tast( void );
void    SYSTEM_INIT( void );
void    uart_0_put_char( unsigned char ch );
void    uart_0_send_temp( void );


#endif /* MAIN_H_ */
