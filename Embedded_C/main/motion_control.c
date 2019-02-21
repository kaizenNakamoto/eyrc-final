#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>

volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder

volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder

volatile unsigned int Degrees; //to accept angle in degrees for turning

volatile unsigned char Left_white_line = 0;

volatile unsigned char Center_white_line = 0;

volatile unsigned char Right_white_line = 0;

/*



* Function Name: 	motion_pin_config



* Input: 			None



* Output: 			None



* Logic: 			Function to configure PORT A and PORT L to enable robot's motion



* Example Call:		motion_pin_config();



*/

void motion_pin_config(void)

 {

    DDRA = DDRA | 0x0F;

    PORTA = PORTA & 0xF0;

    DDRL = DDRL | 0x18; //Setting PL3 and PL4 pins as output for PWM generation

    PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.

}



/*



* Function Name: 	left_encoder_pin_config



* Input: 			None



* Output: 			None



* Logic: 			Function to configure INT4 (PORTE 4) pin as input for the left position encoder



* Example Call:		left_encoder_pin_config();



*/

void left_encoder_pin_config (void)

{

	DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input

	PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin

}



/*



* Function Name: 	right_encoder_pin_config



* Input: 			None



* Output: 			None



* Logic: 			Function to configure INT5 (PORTE 5) pin as input for the right position encoder



* Example Call:		right_encoder_pin_config();



*/

void right_encoder_pin_config (void)

{

	DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input

	PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin

}



/*



* Function Name: 	velocity



* Input: 			left_motor (unsigned char) -> Left motor velocity



                    right_motor (unsigned char) -> Right motor velocity



* Output: 			None



* Logic: 			PWM is used. 255 (0xFF)is set as the maximum value of the timer. When the count of the timer exceeds



255 the timer overflows. The TCNT and OCR5_n registers are compared and on match the timer overflows.



For 100% Duty cycle, the value is 255. Hence to vary the duty cycle we can give any value in the range 0-255.



* Example Call:		velocity(255,255);



*/

void velocity(unsigned char left_motor, unsigned char right_motor) 

{

    OCR5AL = (unsigned char) left_motor;

    OCR5BL = (unsigned char) right_motor;

}



/*



* Function Name: 	left_position_encoder_interrupt_init



* Input: 			None



* Output: 			None



* Logic: 			Interrupt 4 enable



* Example Call:		left_position_encoder_interrupt_init();



*/

void left_position_encoder_interrupt_init (void) //Interrupt 4 enable

{

	cli(); //Clears the global interrupt

	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge

	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder

	sei();   // Enables the global interrupt

}



/*



* Function Name: 	right_position_encoder_interrupt_init



* Input: 			None



* Output: 			None



* Logic: 			Interrupt 5 enable



* Example Call:		right_position_encoder_interrupt_init();



*/

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable

{

	cli(); //Clears the global interrupt

	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge

	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder

	sei();   // Enables the global interrupt

}



/*



* Function Name: 	ISR



* Input: 			INT5_vect



* Output: 			None



* Logic: 			ISR for right position encoder, increment right shaft position count



* Example Call:		Called automatically by interrupt



*/

ISR(INT5_vect)

{

	ShaftCountRight++;  //increment right shaft position count

}



/*



* Function Name: 	ISR



* Input: 			INT4_vect



* Output: 			None



* Logic: 			ISR for left position encoder, increment left shaft position count



* Example Call:		Called automatically by interrupt



*/

ISR(INT4_vect)

{

	ShaftCountLeft++;  //increment left shaft position count

}



/*



* Function Name: 	motion_set	



* Input: 			Direction (unsigned char) -> Direction HEX



					- 0x06 --> Forward	



                    - 0x09 --> Backward



                    - 0x05 --> Left



                    - 0x0A --> Right



                    - 0x04 --> Soft Left



                    - 0x02 --> Soft right



                    - 0x01 --> Soft Left 2 (Reverse logic of soft left)



                    - 0x08 --> Soft Right 2 (Reverse logic of soft right)



                    - 0x00 --> Stop



* Output: 			None



* Logic: 			Assigns the motor to move in specified direction. The motor consists of two inputs.



					Based on polarity of the inputs, it rotates clockwise or anticlockwise.



* Example Call:		motion_set(0x01);



*/

void motion_set(unsigned char Direction)

 {

    unsigned char PortARestore = 0;



    Direction &= 0x0F; // removing upper nibble for protection

    PortARestore = PORTA; // reading PORTA's original status

    PortARestore &= 0xF0; // making lower direction nibble to 0

    PortARestore |= Direction; // adding lower nibble for forward command and restoring PORTA's status

    PORTA = PortARestore; // executing the command

}



/*



* Function Name: 	forward



* Input: 			None



* Output: 			None



* Logic: 			Both wheels forward



* Example Call:		forward();



*/

void forward(void)

 {

    motion_set(0x06);

}



/*



* Function Name: 	back



* Input: 			None



* Output: 			None



* Logic: 			Both wheels backward



* Example Call:		back();



*/

void back (void)

{

	motion_set(0x09);

}



/*



* Function Name: 	left



* Input: 			None



* Output: 			None



* Logic: 			Left wheel backward, Right wheel forward



* Example Call:		left();



*/

void left (void)

{

	motion_set(0x05);

}



/*



* Function Name: 	right



* Input: 			None



* Output: 			None



* Logic: 			Left wheel forward, Right wheel backward



* Example Call:		right();



*/

void right (void)

{

	motion_set(0x0A);

}



/*



* Function Name: 	soft_left



* Input: 			None



* Output: 			None



* Logic: 			Left wheel stationary, Right wheel forward



* Example Call:		soft_left();



*/

void soft_left (void)

{

	motion_set(0x04);

}



/*



* Function Name: 	soft_right



* Input: 			None



* Output: 			None



* Logic: 			Left wheel forward, Right wheel is stationary



* Example Call:		soft_right();



*/

void soft_right (void) 

{

	motion_set(0x02);

}



/*



* Function Name: 	soft_left_2



* Input: 			None



* Output: 			None



* Logic: 			Left wheel backward, right wheel stationary



* Example Call:		soft_left_2();



*/

void soft_left_2 (void) 

{

	motion_set(0x01);

}



/*



* Function Name: 	soft_right_2



* Input: 			None



* Output: 			None



* Logic: 			Left wheel stationary, Right wheel backward



* Example Call:		soft_right_2();



*/

void soft_right_2 (void)

{

	motion_set(0x08);

}



/*



* Function Name: 	stop



* Input: 			None



* Output: 			None



* Logic: 			Both wheels stop



* Example Call:		stop():



*/

void stop(void)

{

	motion_set(0x00);

}



/*



* Function Name: 	angle_rotate	



* Input: 			Degrees (unsigned int) -> Rotate by specified degrees



* Output: 			None



* Logic: 			The robot moves 4.090 degrees per count. Hence, it calculates required shaft count by 



					dividing by 4.090 for moving by specified degrees.



* Example Call:		angle_rotate(180);



*/

void angle_rotate(unsigned int Degrees)

{

	float ReqdShaftCount = 0;

	unsigned long int ReqdShaftCountInt = 0;



	ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count

	ReqdShaftCountInt = (unsigned int) ReqdShaftCount;

	ShaftCountRight = 0;

	ShaftCountLeft = 0;



	while (1)

	{

		if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))

		break;

	}

	stop(); //Stop robot

}



/*



* Function Name: 	linear_distance_mm	



* Input: 			DistanceInMM (unsigned int) -> Traverse by specified distance



* Output: 			None



* Logic: 			The robot moves 5.338mm per pulse. Hence,it calculates required shaft count by dividing by 



					5.338 to move the specified distance.



* Example Call:		linear_distance_mm(100);



*/

void linear_distance_mm(unsigned int DistanceInMM)

{

	float ReqdShaftCount = 0;

	unsigned long int ReqdShaftCountInt = 0;



	ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count

	ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;

	

	ShaftCountRight = 0;

	while(1)

	{

		if(ShaftCountRight > ReqdShaftCountInt)

		{

			break;

		}

	}

	stop(); //Stop robot

}



/*



* Function Name: 	forward_mm



* Input: 			DistanceInMM (unsigned int) -> Traverse by specified distance



* Output: 			None



* Logic: 			The robot moves forward for the specified distance



* Example Call:		forward_mm(100);



*/

void forward_mm(unsigned int DistanceInMM)

{

	forward();

	linear_distance_mm(DistanceInMM);

}



/*



* Function Name: 	back_mm



* Input: 			DistanceInMM (unsigned int) -> Traverse by specified distance



* Output: 			None



* Logic: 			The robot moves backward for the specified distance



* Example Call:		back_mm(100);



*/

void back_mm(unsigned int DistanceInMM)

{

	back();

	linear_distance_mm(DistanceInMM);

}



/*



* Function Name: 	left_degrees



* Input: 			Degrees (unsigned int) -> Rotate by specified degrees



* Output: 			None



* Logic: 			The robot rotates left for the specified degrees



* Example Call:		left_degrees(90);



*/

void left_degrees(unsigned int Degrees)

{

	// 88 pulses for 360 degrees rotation 4.090 degrees per count

	left(); //Turn left

	angle_rotate(Degrees);

}



/*



* Function Name: 	right_degrees



* Input: 			Degrees (unsigned int) -> Rotate by specified degrees



* Output: 			None



* Logic: 			The robot rotates right for the specified degrees



* Example Call:		right_degrees(90);



*/

void right_degrees(unsigned int Degrees)

{

	// 88 pulses for 360 degrees rotation 4.090 degrees per count

	right(); //Turn right

	angle_rotate(Degrees);

}



/*



* Function Name: 	soft_left_degrees



* Input: 			Degrees (unsigned int) -> Rotate by specified degrees



* Output: 			None



* Logic: 			The robot takes a soft left for the specified degrees



* Example Call:		soft_left_degrees(90);



*/

void soft_left_degrees(unsigned int Degrees)

{

	// 176 pulses for 360 degrees rotation 2.045 degrees per count

	soft_left(); //Turn soft left

	Degrees=Degrees*2;

	angle_rotate(Degrees);

}



/*



* Function Name: 	soft_right_degrees



* Input: 			Degrees (unsigned int) -> Rotate by specified degrees



* Output: 			None



* Logic: 			The robot takes a soft right for the specified degrees



* Example Call:		soft_right_degrees(90);



*/

void soft_right_degrees(unsigned int Degrees)

{

	// 176 pulses for 360 degrees rotation 2.045 degrees per count

	soft_right();  //Turn soft right

	Degrees=Degrees*2;

	angle_rotate(Degrees);

}



/*



* Function Name: 	soft_left_2_degrees



* Input: 			Degrees (unsigned int) -> Rotate by specified degrees



* Output: 			None



* Logic: 			The robot takes a reverse soft left for the specified degrees



* Example Call:		soft_left_2_degrees(90);



*/

void soft_left_2_degrees(unsigned int Degrees)

{

	// 176 pulses for 360 degrees rotation 2.045 degrees per count

	soft_left_2(); //Turn reverse soft left

	Degrees=Degrees*2;

	angle_rotate(Degrees);

}



/*



* Function Name: 	soft_right_2_degrees



* Input: 			Degrees (unsigned int) -> Rotate by specified degrees



* Output: 			None



* Logic: 			The robot takes a reverse soft right for the specified degrees



* Example Call:		soft_right_2_degrees(90);



*/

void soft_right_2_degrees(unsigned int Degrees)

{

	// 176 pulses for 360 degrees rotation 2.045 degrees per count

	soft_right_2();  //Turn reverse soft right

	Degrees=Degrees*2;

	angle_rotate(Degrees);

}



/*



* Function Name:   	correct_right	



* Input: 			None			



* Output: 			None	



* Logic:			Keep rotating right till center sensor see line



					Keep rotating right till right sensor see line

					

* Example Call:		correct_right();



*/

void correct_right()

{   stop();
	_delay_ms(20);
	soft_right_degrees(35);
	soft_right();
	while(1)
	{
		read_line();    
		if((Center_white_line > 14 && Left_white_line > 14)||(Center_white_line>70))
		{   
			stop();
			_delay_ms(50);
			break;
		}
	}

}



/*



* Function Name:   	correct_right2	



* Input: 			None			



* Output: 			None	



* Logic:			Keep rotating right till center sensor see line



					Keep rotating right till right sensor see line

					

* Example Call:		correct_right2();



*/

void correct_right2()

{   stop();
	_delay_ms(50);
	right_degrees(35);
	right();
	while(1)

	{

		read_line();

		if((Center_white_line > 14 && Left_white_line > 14)||(Center_white_line>71))

		{

			stop();

			_delay_ms(50);

			break;

		}

	}
}



/*



* Function Name:   	correct_left	



* Input: 			None			



* Output: 			None	



* Logic:			Keep rotating left till center sensor sees line



					Keep rotating left till left sensor sees line



* Example Call:		correct_left();



*/

void correct_left()

{   
	     stop();
		 _delay_ms(20);
	     soft_left_degrees(35);
		 soft_left();
	     while(1)
	     {
		     read_line();
			 if((Center_white_line > 14 && Right_white_line > 14)||(Center_white_line > 80))
		     {
			     stop();
			     _delay_ms(50);
			     break;
		     }
	     }
}

void spl_left(){
	stop();
	_delay_ms(20);
	left_degrees(35);
	left();
	while(1){
		read_line();
		if(Center_white_line>=25)
		{
		stop();
		_delay_ms(20);
		break;	
		}
	}
	_delay_ms(20);
}
void spl_right(){
	right_degrees(25);
	right();
	while(1){
		read_line();
		if(Center_white_line>=25)
		{
			stop();
			_delay_ms(20);
			break;
		}
	}
	_delay_ms(20);
}


/*



* Function Name:   	correct_left2	



* Input: 			None			



* Output: 			None	



* Logic:			Keep rotating left till center sensor see line



					Keep rotating left till right sensor see line

					

* Example Call:		correct_left2();



*/
void spl_left2(){// FOR FRONT PICK FROM LEFT DIRECTION
	stop();
	_delay_ms(20);
	soft_left_2_degrees(30);
	soft_left_2();
	while(1){
		read_line();
		if(Center_white_line>25){
			stop();
			_delay_ms(20);
			break;
		}
	}
}
void spl_right2(){//FOR FRONT WHEN BOT IS COMING FROM RIGHT
	stop();
	_delay_ms(20);
	soft_right_2_degrees(30);
	soft_right_2();
	while(1){
		read_line();
		if(Center_white_line>25){
			stop();
			_delay_ms(20);
			break;
		}
	}
}
void correct_left2()

{   

    stop();
	_delay_ms(50);
	left_degrees(35);
	left();
	while(1)

	{   
		read_line();
		if((Center_white_line > 14 && Right_white_line > 14)||(Center_white_line > 70))

		{

			stop();

			_delay_ms(50);

			break;

		}

	}


}

void correct_right3()

{   stop();
	_delay_ms(20);
	right_degrees(160);
	right();
	while(1){
		read_line();
		if(Center_white_line>25){
			stop();
		break;}
	}
	_delay_ms(40);
	back_mm(40);
}

