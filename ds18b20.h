/*
 * ds18b20.h
 *
 *  Created on: Mar 10, 2018
 *      Author: Maju
 */

#ifndef DS18B20_H_
#define DS18B20_H_

/* DS18B20 przy³¹czony do portu  PD7 AVRa  */
#define SET_ONEWIRE_PORT     PORTC  |=  _BV(1)
#define CLR_ONEWIRE_PORT     PORTC  &= ~_BV(1)
#define IS_SET_ONEWIRE_PIN   PINC   &   _BV(1)
#define SET_OUT_ONEWIRE_DDR  DDRC   |=  _BV(1)
#define SET_IN_ONEWIRE_DDR   DDRC   &= ~_BV(1)

unsigned char ds18b20_ConvertT(void);
int ds18b20_Read(unsigned char []);
void OneWireStrong(char);
unsigned char OneWireReset(void);
void OneWireWriteByte(unsigned char);
unsigned char OneWireReadByte(void);

#endif /* DS18B20_H_ */
