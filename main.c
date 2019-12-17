/*
 * main.c
 *
 *  Created on: Mar 7, 2018
 *      Author: Maju
 */
#include<avr/io.h>
#include<util/delay.h>
#include"funkcje.h"
#include"HD44780.h"
#include<stdio.h>
#include<avr/eeprom.h>
#include<avr/interrupt.h>
#include"ds18b20.h"



parametry p_t EEMEM;
parametry menu_p;



uint8_t y=0; //do zezwolenia zgrzewu od temp
volatile uint16_t tab_t[5]={10,10,50,1500,90};
volatile uint8_t tab_x[5]={12,12,16,16,9};
volatile uint8_t tab_y[5]={0,1,0,1,3};

double t;


int main(void)
{
	DDRD |= (1<<PD7); //buzzer
	DDRB |= ((0<<PB0) | (1<<PB1)); //przycisk wejœcie, wyjscie zgrz
	PORTB |= (1<<PB0); //przycisk do vcc
	DDRB |= (1<<PB7); //wentylator

	DDRD |= ((0<<PD2)|(0<<PD3));  //przerwania zewn jako wejœcia
	PORTD |= ((1<<PD2)|(1<<PD3)) ;
	PORTC |= (1<<PC4);
	GICR |= (0b01000000);
	MCUCR |= (0b00001100);

	TCCR1B |= (1<<WGM12); // tryb CTC
	TCCR1B |= ((1<<CS12)|(1<<CS10)); //preskaler 256
	OCR1A = 3905; //przerwanie co 0,33s

	LCD_Initalize();
	LCD_GoTo(4, 2);
	LCD_WriteText("Maju Ver.1.0");
	LCD_GoTo(5, 0);
	LCD_WriteText("Zgrzewarka");
	_delay_ms(3000);
	LCD_Clear();

	LCD_GoTo(0, 0);
	LCD_WriteText("Impulsy[ms]:   /");
	LCD_GoTo(0, 1);
	LCD_WriteText("Przerwy[ms]:   /");
	LCD_GoTo(0, 3);
	LCD_WriteText("T[C]:   /   WEN:");


/*
	p_t.p_t_0=10;
	p_t.p_t_1=50;
	p_t.p_t_2=50;
	p_t.p_t_3=1500;
	p_t.p_T=90;*/

	//menu_p.p_T=11;
	//eeprom_busy_wait();
	//eeprom_write_block(&menu_p, &p_t, sizeof(menu_p));


/*
		menu_p.p_t_0=0;
		menu_p.p_t_1=0;
		menu_p.p_t_2=0;
		menu_p.p_t_3=0;
		menu_p.p_T=0;
*/
	//eeprom_busy_wait();
	//eeprom_write_block(&menu_p, &p_t, sizeof(menu_p));
	//menu_p.p_T=0;
	eeprom_busy_wait();
	eeprom_read_block(&menu_p,&p_t, sizeof(menu_p));

	tab_t[0]= ~menu_p.p_t_0;
	tab_t[1]= ~menu_p.p_t_1;
	tab_t[2]= ~menu_p.p_t_2;
	tab_t[3]= ~menu_p.p_t_3;
	tab_t[4]= ~menu_p.p_T;

	LCD_ZM(tab_t[3],16,1);
	LCD_ZM(tab_t[0],12,0);
	LCD_ZM(tab_t[1],12,1);
	LCD_ZM(tab_t[2],16,0);
	LCD_ZM(tab_t[4],9,3);

	sei();
	while(1)
	{
		menu();
		temp();
		przycisk();
	}
}

