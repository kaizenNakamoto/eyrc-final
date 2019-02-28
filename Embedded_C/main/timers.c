#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>


/*



* Function Name: 	timer1_init	



* Input: 			None



* Output: 			None



* Logic: 			Function to initiate timer 1 for Servo control



					- TIMER1 initialization in 10 bit fast PWM mode  



					- Pre-scaler:256



					- WGM: 7) PWM 10bit fast, TOP=0x03FF



					- Actual value: 52.25Hz 



* Example Call:		timer1_init();



*/

void timer1_init()

{

	TCCR1A = 0X00;

	ICR1 = 1023;

	TCNT1H = 0XFC;

	TCNT1L = 0X01;

	OCR1AH = 0x03;	//Output compare Register high value for servo 1

	OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1

	OCR1BH = 0x03;	//Output compare Register high value for servo 2

	OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2

	OCR1CH = 0x03;	//Output compare Register high value for servo 3

	OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3

	ICR1H  = 0x03;

	ICR1L  = 0xFF;

	TCCR1A = 0XAB;

	TCCR1B = 0X0C;

	TCCR1C = 0x00;

}



/*



* Function Name: 	timer3_init	



* Input: 			None



* Output: 			None



* Logic: 			Function to initiate timer 3 for Line Sensor Readings



					- TIMER3 initialize -> pre-scaler:256



					- WGM: 0) Normal, TOP=0xFFFF



					- Desired value: 100Hz



					- Actual value:  100.000Hz (0.0%)



* Example Call:		timer3_init();



*/

void timer3_init(void)

{

	TCCR3B = 0x00; //stop

	TCNT3 = 0xFDC0; // 0.01s

	OCR3AH = 0x00; //Output Compare Register (OCR)- Not used

	OCR3AL = 0x00; //Output Compare Register (OCR)- Not used

	OCR3BH = 0x00; //Output Compare Register (OCR)- Not used

	OCR3BL = 0x00; //Output Compare Register (OCR)- Not used

	OCR3CH = 0x00; //Output Compare Register (OCR)- Not used

	OCR3CL = 0x00; //Output Compare Register (OCR)- Not used

	ICR3H = 0x00; //Input Capture Register (ICR)- Not used

	ICR3L = 0x00; //Input Capture Register (ICR)- Not used

	TCCR3A = 0x00;

	TCCR3C = 0x00;

	TCCR3B = 0x04; //Pre-scaler 256 1-0-0

}



/*



* Function Name: 	timer4_init	



* Input: 			None



* Output: 			None



* Logic: 			Function to initiate timer 4 for Timing Operations



					- TIMER4 initialize -> Pre-scaler:256



					- WGM: 0 Normal, TOP=0xFFFF



					- Desired value: 10Hz



					- Actual value:  10.000Hz (0.0%)



* Example Call:		timer4_init();



*/

void timer4_init(void) 

{

	TCCR4B = 0x00; //stop

	TCNT4 = 0xE980; // 0.1s

	OCR4AH = 0x00; //Output Compare Register (OCR)- Not used

	OCR4AL = 0x00; //Output Compare Register (OCR)- Not used

	OCR4BH = 0x00; //Output Compare Register (OCR)- Not used

	OCR4BL = 0x00; //Output Compare Register (OCR)- Not used

	OCR4CH = 0x00; //Output Compare Register (OCR)- Not used

	OCR4CL = 0x00; //Output Compare Register (OCR)- Not used

	ICR4H = 0x00; //Input Capture Register (ICR)- Not used

	ICR4L = 0x00; //Input Capture Register (ICR)- Not used

	TCCR4A = 0x00;

	TCCR4C = 0x00;

	TCCR4B = 0x04; //Pre-scaler 256 1-0-0

}



/*



* Function Name: 	timer5_init	



* Input: 			None



* Output: 			None



* Logic: 			Function to initiate timer 5



					- Timer 5 initialized in PWM mode for velocity control



					- Pre-scaler:256



					- PWM 8bit fast, TOP=0x00FF



					- Timer Frequency:225.000Hz



* Example Call:		timer5_init();



*/

void timer5_init()

{

    TCCR5B = 0x00; //Stop

    TCNT5H = 0xFF; //Counter higher 8-bit value to which OCR5xH value is compared with

    TCNT5L = 0x01; //Counter lower 8-bit value to which OCR5xH value is compared with

    OCR5AH = 0x00; //Output compare register high value for Left Motor

    OCR5AL = 0xFF; //Output compare register low value for Left Motor

    OCR5BH = 0x00; //Output compare register high value for Right Motor

    OCR5BL = 0xFF; //Output compare register low value for Right Motor

    OCR5CH = 0x00; //Output compare register high value for Motor C1

    OCR5CL = 0xFF; //Output compare register low value for Motor C1

    TCCR5A = 0xA9;

    /*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}

For Overriding normal port functionality to OCRnA outputs.

				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/



    TCCR5B = 0x0B; //WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)

}
