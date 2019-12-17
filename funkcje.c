#include<avr/io.h>
#include<util/delay.h>
#include <avr/eeprom.h>
#include"funkcje.h"
#include<stdio.h>
#include"HD44780.h"
#include<avr/interrupt.h>
#include"ds18b20.h"


volatile uint8_t menu_flag=0;
volatile int8_t step_flag=0;


extern parametry p_t EEMEM;
extern parametry menu_p;




ISR(TIMER1_COMPA_vect)
{
	static uint8_t m;

	if(menu_flag!=0)
	{
	LCD_GoTo(tab_x[menu_flag-1], tab_y[menu_flag-1]);

			if(m!=1)
				{
				if(menu_flag==1 || menu_flag==2) LCD_WriteText("   ");
				if(menu_flag==3 || menu_flag==4)LCD_WriteText("    ");
				if(menu_flag==5)LCD_WriteText("  ");
				m=1;
				}
			else
				{
				LCD_ZM(tab_t[menu_flag-1],tab_x[menu_flag-1], tab_y[menu_flag-1]);
				m=0;
				}
	}
}

ISR(INT0_vect)
	{
	if(!(PIND & (1<<PD2)))
	{
		menu_flag=(menu_flag+1)%6;
		for(uint8_t i=1;i<=5;i++)
		 	 {
			 	 if(i!=menu_flag) LCD_ZM(tab_t[i-1],tab_x[i-1], tab_y[i-1]);
		 	 }
		if(menu_flag==0)
		{
			for(uint8_t i=0;i<=4;i++)
				{
					 LCD_ZM(tab_t[i],tab_x[i], tab_y[i]);
				}
		}


		_delay_ms(500);

	}
	}

ISR(INT1_vect)
	{
	if(PINC & (1<<PC4))
	{
		step_flag=step_flag+1;
	}
	else
	{
		step_flag=step_flag-1;
	}
	}

void temp(void)
{
	static uint8_t tt;
	unsigned char ds18b20_pad[9];
	ds18b20_ConvertT();
	ds18b20_Read(ds18b20_pad);
	t = ((ds18b20_pad[1] << 8) + ds18b20_pad[0]) / 16.0 ;

	if(t!=tt)
	{
	LCD_GoTo(6, 3);
	LCD_WriteText("  ");
	LCD_ZM(t,6,3);

	if(t>=tab_t[4]) y=0;
	else y=1;

	if(t>=30)
		{
		PORTB |= (1<<PB7);
		LCD_GoTo(17, 3);
		LCD_WriteText("   ");
		LCD_GoTo(17, 3);
		LCD_WriteText("ON");
		}
	if(t<30)
		{
		PORTB &= ~(1<<PB7);
		LCD_GoTo(17, 3);
		LCD_WriteText("OFF");
		}
	}
	tt=t;
}

void menu(void)
{
	GICR |= (0b10000000); //wl . przerwania zewn. od obrotu enkodera
	TIMSK |= (1<<OCIE1A); //zewolenie na przerwanie compare match, dla migania

	// odczyt z eprom/////////// chyba nie potrzeba bo sie nie zmieni¹

	step_flag=0;
	while(menu_flag!=0)
	{
		//k=tab_t[menu_flag-1];
		if(step_flag==-1)
			{
			if(tab_t[menu_flag-1]>0 && menu_flag==4) tab_t[menu_flag-1]=tab_t[menu_flag-1]-100;
			else if(tab_t[menu_flag-1]>0 && menu_flag==5) tab_t[menu_flag-1]=tab_t[menu_flag-1]-5;
			//else if(tab_t[menu_flag-1]>0 && (menu_flag==1 || menu_flag==2|| menu_flag==3)) tab_t[menu_flag-1]=tab_t[menu_flag-1]-10;
			else if(tab_t[menu_flag-1]>0 && (menu_flag==1 || menu_flag==2)) tab_t[menu_flag-1]=tab_t[menu_flag-1]-10;
			else if(tab_t[menu_flag-1]>0 && tab_t[menu_flag-1]<=1000 && (menu_flag==3)) tab_t[menu_flag-1]=tab_t[menu_flag-1]-10;
			else if(tab_t[menu_flag-1]>0 && tab_t[menu_flag-1]>1000 && (menu_flag==3)) tab_t[menu_flag-1]=tab_t[menu_flag-1]-100;

			if(tab_t[menu_flag-1]<0)tab_t[menu_flag-1]=0;

			step_flag=0;
			LCD_GoTo(tab_x[menu_flag-1], tab_y[menu_flag-1]);
				if(menu_flag==1 || menu_flag==2) LCD_WriteText("   ");
				if(menu_flag==3 || menu_flag==4)LCD_WriteText("    ");
				if(menu_flag==5)LCD_WriteText("  ");
			LCD_ZM(tab_t[menu_flag-1],tab_x[menu_flag-1], tab_y[menu_flag-1]);
			}
		if(step_flag==1)
		{
			if(tab_t[menu_flag-1]<9900 && menu_flag==4) tab_t[menu_flag-1]=tab_t[menu_flag-1]+100;
			else if(tab_t[menu_flag-1]<95 && menu_flag==5) tab_t[menu_flag-1]=tab_t[menu_flag-1]+5;
			//else if(tab_t[menu_flag-1]<990 && (menu_flag==1 || menu_flag==2|| menu_flag==3)) tab_t[menu_flag-1]=tab_t[menu_flag-1]+10;
			else if(tab_t[menu_flag-1]<990 && (menu_flag==1 || menu_flag==2)) tab_t[menu_flag-1]=tab_t[menu_flag-1]+10;
			else if(tab_t[menu_flag-1]<1000 && (menu_flag==3)) tab_t[menu_flag-1]=tab_t[menu_flag-1]+10;
			else if(tab_t[menu_flag-1]>=1000 && (menu_flag==3)) tab_t[menu_flag-1]=tab_t[menu_flag-1]+100;

			step_flag=0;
			LCD_GoTo(tab_x[menu_flag-1], tab_y[menu_flag-1]);
				if(menu_flag==1 || menu_flag==2) LCD_WriteText("   ");
				if(menu_flag==3 || menu_flag==4)LCD_WriteText("    ");
				if(menu_flag==5)LCD_WriteText("  ");
			LCD_ZM(tab_t[menu_flag-1],tab_x[menu_flag-1], tab_y[menu_flag-1]);
		}

	}
		if(menu_flag==0)
		{
			TIMSK &= ~(1<<OCIE1A); //wy³ migania
			GICR &= (0b01111111); //wy³ obrotu enkodera

			menu_p.p_t_0=~tab_t[0];
			menu_p.p_t_1=~tab_t[1];
			menu_p.p_t_2=~tab_t[2];
			menu_p.p_t_3=~tab_t[3];
			menu_p.p_T=~tab_t[4];

			eeprom_busy_wait();
			eeprom_write_block(&menu_p, &p_t, sizeof(menu_p));


		}

}


void LCD_ZM(uint16_t a,uint8_t x,uint8_t z)
{
	char data[5];
	sprintf(data, "%d", a);
	LCD_GoTo(x, z);
	LCD_WriteText(data);
}

void impuls10ms(uint8_t x)
{
	for(uint8_t i=0; i<x;i++)
	{
		_delay_ms(10);
	}
}

void buzzer(void)
{
	PORTD |= (1<<PD7);
}

void przycisk(void)
{
	if(!(PINB & (1<<PB0)))
	{
		_delay_ms(10);
		if((!(PINB & (1<<PB0)))&& y==1)
		{
			zgrzew();
		}
	}
}


void zgrzew(void)
{
	PORTB |=(1<<PB1);
	impuls10ms(tab_t[0]/10);
	PORTB &= ~(1<<PB1);

	impuls10ms(tab_t[1]/10);
	PORTB |=(1<<PB1);
	impuls10ms(tab_t[2]/10);
	PORTB &= ~(1<<PB1);
	PORTD |= (1<<PD7);
	impuls10ms(tab_t[3]/10);

	uint8_t k=1;
	while(k==1)
	{
		if(PINB & (1<<PB0))
		{
			_delay_ms(100);
					if(PINB & (1<<PB0))
					{
						k=0;
					}
		}
	}
	PORTD &= ~(1<<PD7);

}

