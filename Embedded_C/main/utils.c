#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>


/*



* Function Name: 	lcd_port_config



* Input: 			None



* Output: 			None



* Logic: 			Function to configure LCD port (PORT C)



* Example Call:		lcd_port_config();



*/

void lcd_port_config(void)

{

	DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output

	PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7

}



/*



* Function Name: 	buzzer_pin_config



* Input: 			None



* Output: 			None



* Logic: 			Function to initialize Buzzer connected to PORTC 3



* Example Call:		buzzer_pin_config();



*/

void buzzer_pin_config(void)

{

	DDRC = DDRC | 0x08; //Setting PORTC 3 as output

	PORTC = PORTC & 0xF7; //Setting PORTC 3 logic low to turnoff buzzer

}



/*



* Function Name: 	buzzer_on



* Input: 			None



* Output: 			None



* Logic: 			Function to switch buzzer on, PORTC 3 pin is set high



* Example Call:		buzzer_on();



*/

void buzzer_on(void)

{

	unsigned char port_restore = 0;

	port_restore = PINC;

	port_restore = port_restore | 0x08;

	PORTC = port_restore;

}



/*



* Function Name: 	buzzer_off



* Input: 			None



* Output: 			None



* Logic: 			Function to switch buzzer off, PORTC 3 pin is set low



* Example Call:		buzzer_off();



*/

void buzzer_off(void)

{

	unsigned char port_restore = 0;

	port_restore = PINC;

	port_restore = port_restore & 0xF7;

	PORTC = port_restore;

}

/*



* Function Name: 	buzz



* Input: 			time_delay (int) -> Time duration between buzzer on and off



* Output: 			None



* Logic: 			Buzzer is set to on for required time period and then switched off



* Example Call:		buzz(1000);



*/

void buzz(unsigned long mst)

{

	buzzer_on();

	_delay_ms(50);

	buzzer_off();

}
