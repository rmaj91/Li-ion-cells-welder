/*
 * funkcje.h
 *
 *  Created on: Mar 7, 2018
 *      Author: Maju
 */

#ifndef FUNKCJE_H_
#define FUNKCJE_H

//#include <avr/eeprom.h>

extern uint8_t y;

/*
 * extern uint16_t tab_t[5];
 volatile extern uint_8t tab_x[5];
 volatile extern uint8_t tab_y[5];
 *
 *
 */
volatile extern uint16_t tab_t[5];
volatile extern uint8_t tab_x[5];
volatile extern uint8_t tab_y[5];
extern double t;


typedef struct  {
     uint16_t p_t_0;
     uint16_t p_t_1;
     uint16_t p_t_2;
     uint16_t p_t_3;
     uint16_t p_T;
} parametry;


void buzzer(void);
void temp(void);
void menu(void);
void LCD_ZM(uint16_t a,uint8_t x,uint8_t z);
void przycisk(void);
void zgrzew(void);
void impuls10ms(uint8_t x);

#endif /* FUNKCJE_H_ */
