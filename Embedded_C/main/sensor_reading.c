#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>


unsigned char ADC_Conversion(unsigned char);

unsigned char ADC_Value;

volatile int t4_sec = 0;

volatile int t4_count;

volatile int t4_flag = 0;


/*



* Function Name: 	adc_pin_config



* Input: 			None



* Output: 			None



* Logic: 			PORTS F AND K are configured to enable ADC conversion



* Example Call:		adc_pin_config ();



*/

void adc_pin_config(void)

{

    DDRF = 0x00;

    PORTF = 0x00;

    DDRK = 0x00;

    PORTK = 0x00;

}



/*



* Function Name: 	adc_init



* Input: 			None



* Output: 			None



* Logic: 			The registers required for ADC conversion are configured



* Example Call:		adc_init();



*/

void adc_init() 

{

	ADCSRA = 0x00;

	ADCSRB = 0x00; //MUX5 = 0

	ADMUX = 0x20; //Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000

	ACSR = 0x80;

	ADCSRA = 0x86; //ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0

}



/*



* Function Name: 	ADC_Conversion	



* Input: 			Ch (unsigned char) -> Channel Number







					ADC CH.     PORT        Sensor
					
					
                  	1 			PF1 		White line sensor 3



                  	2 			PF2 		White line sensor 2



                  	3 			PF3 		White line sensor 1


* Output: 			a (unsigned char) -> Returns corresponding ADC value



* Logic: 			Reads data from the sensors and performs analog to digital



 					conversion by using two registers ADCSRA and ADCSRB



* Example Call:		value = ADC_Conversion(11);



*/

unsigned char ADC_Conversion(unsigned char Ch) 

{

	unsigned char a;

	if (Ch > 7)

	{

		ADCSRB = 0x08;

	}

	Ch = Ch & 0x07;

	ADMUX = 0x20 | Ch;

	ADCSRA = ADCSRA | 0x40; //Set start conversion bit

	while ((ADCSRA & 0x10) == 0); //Wait for conversion to complete

	a = ADCH;

	ADCSRA = ADCSRA | 0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it

	ADCSRB = 0x00;

	return a;

}



/*



* Function Name: 	print_sensor	



* Input: 			row (char) -> LCD row



					column(char) -> LCD column



					channel (unsigned char) -> ADC Channel Number



* Output: 			None



* Logic: 			Function To Print Sensor Values At Desired Row And Column Location on LCD



* Example Call:		print_sensor(1, 1, 11);



*/

void print_sensor(char row, char coloumn, unsigned char channel)

 {



	ADC_Value = ADC_Conversion(channel);

	lcd_print(row, coloumn, ADC_Value, 3);

}

/*



* Function Name:	ISR 	



* Input: 			TIMER3_OVF_vect			



* Output: 			None



* Logic:			- Timer 3 ISR is used to update Line Sensor Readings at 100Hz



					- Based on readings, determine the line condition



* Example Call:		Called automatically by timer interrupt



*/

ISR(TIMER3_OVF_vect)

{

    read_line();

    line_conditions();

    TCNT3 = 0xFDC0;

}



/*



* Function Name: 	ISR	



* Input: 			TIMER4_OVF_vect	



* Output: 			None



* Logic:			- Timer 4 ISR is used for timing operations at a resolution of 10Hz



					- t4_sec is incremented every 0.1 sec until it is equal to required t4_count



* Example Call:		Called automatically by timer interrupt



*/

ISR(TIMER4_OVF_vect)

{

    TCNT4 = 0xE980;

    t4_sec++; //Increment every 0.1 sec



    if (t4_sec == t4_count && t4_flag == 1)

        stop_timer4();

}



/*



* Function Name: 	start_timer4	



* Input: 			count (int) -> Required number of time units in 0.1 sec to count



* Output: 			None



* Logic:			- Enable Timer 4 overflow interrupt



					- Initialize Timer 4 flags (global variables)



* Example Call:		start_timer4(20); -> This will count for 2 sec and stop



*/

void start_timer4(int count)

{

    t4_flag = 1; //Indicate timer 4 is enabled

    t4_sec = 0;

    t4_count = count + 1;

    TIMSK4 = 0x01; //Timer 4 overflow interrupt enable

}



/*



* Function Name: 	stop_timer4	



* Input: 			None



* Output: 			None



* Logic:			- If t4_sec reaches required t4_count, stop timer 4



					- Disable Timer 4 overflow interrupt



					- Also used for turning off buzzer if it is on



* Example Call:		stop_timer4();



*/

void stop_timer4()

{

	t4_flag = 0;

	t4_sec = 0;

	TIMSK4 = 0x00; //Timer4 overflow interrupt disable

}











