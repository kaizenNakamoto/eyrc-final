#define F_CPU 14745600
#define TRUE 1
#define FALSE 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h> //included to support power function

volatile unsigned char data;
volatile unsigned char hab[25];
volatile unsigned char ani[25];

volatile unsigned char tmp1[25];
volatile unsigned char tmp2[25];

unsigned char count = 0;
char pos = -20;

volatile unsigned char flag = TRUE;

unsigned char a = 0;
unsigned char h = 0;

void uart2_init(void)
{
	UCSR2B = 0x00; //disable while setting baud rate
	UCSR2A = 0x00;
	UCSR2C = 0x06;
	UBRR2L = 0x5F; //set baud rate lo
	UBRR2H = 0x00; //set baud rate hi
	UCSR2B = 0x98;
}


ISR(USART2_RX_vect)		// ISR for receive complete interrupt
{
	data= UDR2;
	if(data == 0x0A)
	{
		pos = count;
		count++;
		flag = FALSE;
	}
	else if(data == 0x23)
	{
		flag = 2;
		strcpy(hab, tmp1);
		strcpy(ani, tmp2);
	}
	else if(count > pos && flag == FALSE)
	{
		tmp2[a]=data;
		a++;
		count++;
		flag = FALSE;
	}
	else if(flag==TRUE)
	{
		tmp1[h]=data;
		h++;
		count++;
		flag = TRUE;
	}
}

