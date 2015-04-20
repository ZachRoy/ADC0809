/*
 * AtoD0809.c
 *
 * Created: 3/12/2015 3:01:38 PM
 *  Author: Zach Roy
 */ 
#include <avr/io.h>
#define F_CPU 4000000ul
#define BAUD 9600
#define MYUBRR F_CPU/BAUD/16-1
#include <util/delay.h>
#define phantom 0xA000
#define CTRL_WD 0x8B
#define CTRL_REG 0x4003
#define EXTPortA 0x4000
#define EXTPortB 0x4001
#define EXTPortC 0x4002
#define EOC PB2
#define SOC PB1
#define OE PB3

static char asctab[] = "0123456789ABCDEF";

void Setup()
{
	UBRRH = ((MYUBRR)>>8);
	UBRRL = MYUBRR;
	//	UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1);
	//UCSRC = (1<<UCSZ1)|(1<<UCSZ0)|(1<<URSEL);
	UCSRB = (1<<RXEN)|(1<<TXEN);
}

void USART_TX(unsigned char data)
{
	while(!(UCSRA & (1<<UDRE)))
		PORTB = PINB^(1<<PB0);
	UDR = data;
}
void Hex2Ascii(unsigned char c1,unsigned char *c2,unsigned char *c3)
{
	(*c2) = asctab[(c1>>4)&0x0f];
	(*c3) = asctab[(c1)& 0x0f];
}

int main(void)
{
	void Setup();
	unsigned char x = 0;
	unsigned char c2 = 0;
	unsigned char c3 = 0;
	//PORTB=PORTB|(1<<PB1);
	PORTB = (1<<OE)| (1<<SOC);
	DDRB = DDRB | (1<<OE)| (1<<SOC)|(1<<PB0);
	PORTB = 0;
	MCUCR=MCUCR|(1<<SRE);
	

	
	unsigned char *ptr;
//	unsigned char *ptr2;
//	ptr2 = (unsigned char *)0x2000;
	ptr = (unsigned char *)CTRL_REG;
	(*ptr) = CTRL_WD;
	
	unsigned char *ale;
	ale = (unsigned char*)phantom;
	
	while(1){
		PORTB = PORTB|(1<<SOC);
		//_delay_ms(10);
		PORTB = PORTB&(~(1<<SOC));
	while((PINB & (1<<EOC))){
			(*ale)  = 0x00;
		}
	
	//	_delay_ms(5);
		while(!(PINB & (1<<EOC))){
			(*ale)  = 0x00;
			//_delay_ms(1);
		}
		//_delay_ms(200);
		PORTB = PORTB|(1<<OE);
		ptr = (unsigned char*)EXTPortB;
		//(*ptr2) = 0x00;
		x = (*ptr);
		//Hex2Ascii(x, &c2, &c3);
		//USART_TX(c2);
		//USART_TX(c3);
		ptr = (unsigned char *)EXTPortA;
		(*ptr) = ~x;
		//delay_ms(300);
		//(*ptr2) = 0xF0;
		
		PORTB = PORTB&(~(1<<OE));
	
		//while(1)
		//;
	}
}