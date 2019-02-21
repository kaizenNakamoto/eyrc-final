#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>

/*



* Function Name: 	port_init()



* Input: 			None



* Output: 			None



* Logic: 			Function to initialize all the ports



* Example Call:		port_init();



*/

void port_init()

{   DDRJ= DDRJ | 0xF7;

	PORTJ=0xFF;

	buzzer_pin_config(); //Buzzer pin configuration

	lcd_port_config();  //LCD pin configuration

	

	adc_pin_config();  //ADC pin configuration

	motion_pin_config(); //Motion pin configuration

	left_encoder_pin_config(); //Left Encoder pin configuration

	right_encoder_pin_config(); //Right Encoder pin configuration
	
	servo1_pin_config();
	
	servo2_pin_config();
	
	servo3_pin_config();

}



/*



* Function Name: 	init_devices



* Input: 			None



* Output: 			None



* Logic: 			Function to initiate all the sensors, actuators and interrupts



* Example Call:		init_devices();



*/

void init_devices(void)

{

	cli(); //Clears the global interrupts

	

	port_init(); //Initiate all ports

	adc_init(); //Initiate ADC registers
	
	uart2_init();


	//Initiate timers

	timer1_init();

	timer5_init();

	timer3_init();

	timer4_init();

	

	//Initiate position encoder interrupts

	left_position_encoder_interrupt_init();

	right_position_encoder_interrupt_init();

	

	sei(); //Enables the global interrupts

}
