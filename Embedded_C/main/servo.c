
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char ii = 0;//Counter Variable
signed char jj = 0;//Counter Variable

/*
* Function Name: 	servo1_pin_config
* Input: 			None
* Output: 			None
* Logic: 			Configure PORTB 5 pin for servo motor 1 operation
* Example Call:		servo1_pin_config();
*/
void servo1_pin_config (void)
{
 DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
 PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

/*
* Function Name: 	servo2_pin_config
* Input: 			None
* Output: 			None
* Logic: 			Configure PORTB 6 pin for servo motor 2 operation
* Example Call:		servo2_pin_config();
*/
void servo2_pin_config (void)
{
 DDRB  = DDRB | 0x40;  //making PORTB 6 pin output
 PORTB = PORTB | 0x40; //setting PORTB 6 pin to logic 1
}

/*
* Function Name: 	servo3_pin_config
* Input: 			None
* Output: 			None
* Logic: 			Configure PORTB 7 pin for servo motor 3 operation
* Example Call:		servo3_pin_config();
*/
void servo3_pin_config (void)
{
 DDRB  = DDRB | 0x80;  //making PORTB 7 pin output
 PORTB = PORTB | 0x80; //setting PORTB 7 pin to logic 1
}

/*
* Function Name: 	servo_1
* Input: 			degrees (unsigned char) -> The angle by which the servo should rotate
* Output: 			None
* Logic: 			Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
* Example Call:		servo_1(60);
*/
void servo_1(unsigned char degrees)  
{
 float PositionPanServo = 0;
 PositionPanServo = ((float)degrees * 0.512) + 34.56;
 OCR1AH = 0x00;
 OCR1AL = (unsigned char) PositionPanServo;
}



/*
* Function Name: 	servo_2
* Input: 			degrees (unsigned char) -> The angle by which the servo should rotate
* Output: 			None
* Logic: 			Function to rotate Servo 2 by a specified angle in the multiples of 1.86 degrees
* Example Call:		servo_2(100);
*/
void servo_2(unsigned char degrees)
{
 float PositionTiltServo = 0;
 PositionTiltServo = ((float)degrees * 0.512) + 34.56;
 OCR1BH = 0x00;
 OCR1BL = (unsigned char) PositionTiltServo;
}


/*
* Function Name: 	servo_3
* Input: 			degrees (unsigned char) -> The angle by which the servo should rotate
* Output: 			None
* Logic: 			Function to rotate Servo 3 by a specified angle in the multiples of 1.86 degrees
* Example Call:		servo_3(160);
*/
void servo_3(unsigned char degrees)
{
 float PositionServo = 0;
 PositionServo = ((float)degrees * 0.512) + 34.56;
 OCR1CH = 0x00;
 OCR1CL = (unsigned char) PositionServo;
}

//servo_free functions unlocks the servo motors from the any angle 
//and make them free by giving 100% duty cycle at the PWM. This function can be used to 
//reduce the power consumption of the motor if it is holding load against the gravity.


/*
* Function Name: 	servo_1_free
* Input: 			None
* Output: 			None
* Logic: 			Makes servo 1 free rotating
* Example Call:		servo_1_free():
*/
void servo_1_free (void) //makes servo 1 free rotating
{
 OCR1AH = 0x03; 
 OCR1AL = 0xFF; //Servo 1 off
}

/*
* Function Name: 	servo_2_free
* Input: 			None
* Output: 			None
* Logic: 			Makes servo 2 free rotating
* Example Call:		servo_2_free();
*/
void servo_2_free (void) //makes servo 2 free rotating
{
 OCR1BH = 0x03;
 OCR1BL = 0xFF; //Servo 2 off
}

/*
* Function Name: 	servo_3_free
* Input: 			None
* Output: 			None
* Logic: 			Makes servo 3 free rotating
* Example Call:		servo_3_free();
*/
void servo_3_free (void) //makes servo 3 free rotating
{
 OCR1CH = 0x03;
 OCR1CL = 0xFF; //Servo 3 off
} 

/*
* Function Name:     gripper_open
* Input:             None
*Output:             None
*Logic:              To open the gripper 
*Example Call:       gripper_open();
*/
void gripper_open()
{
	for(jj=10;jj<55;jj++)
	{
		servo_3(jj);
		_delay_ms(10);
	}
	_delay_ms(200);
}

/*
* Function Name:     gripper_close
* Input:             None
*Output:             None
*Logic:              To close the gripper 
*Example Call:       gripper_close();
*/
void gripper_close()
{
	for(ii=55;ii>10;ii--)
	{
		servo_3(ii);
		_delay_ms(10);
	}
	
	_delay_ms(200);
}

/*
* Function Name:     arm_rotate_left
* Input:             None
*Output:             None
*Logic:              To rotate the arm towards left
*Example Call:       arm_rotate_left();
*/
void arm_rotate_left()
{
	for(ii=93;ii>0;ii--)
	{
		servo_1(ii);
		_delay_ms(10);
	}
	_delay_ms(300);
}

/*
* Function Name:     arm_rotate_right
* Input:             None
*Output:             None
*Logic:              To rotate the arm towards right
*Example Call:       arm_rotate_right();
*/
void arm_rotate_right()
{
	for(ii=95;ii<180;ii++)
	{
		servo_1(ii);
		_delay_ms(10);
	}
	_delay_ms(300);
}

/*
* Function Name:     arm_straight
* Input:             None
*Output:             None
*Logic:              Make sure the arm remains upright
*Example Call:       arm_straight();
*/
void arm_straight()
{
	for(ii=0;ii<95;ii++)
	{
		servo_1(ii);
		_delay_ms(10);
	}
	_delay_ms(300);
}

/*
* Function Name:     arm_forward
* Input:             None
*Output:             None
*Logic:              To bend the arm in the forward direction
*Example Call:       arm_forward();
*/
void arm_forward()
{
	for(ii=0;ii<55;ii++)
	{
		servo_2(ii);
		_delay_ms(10);
	}
	_delay_ms(300);
	
}

/*
* Function Name:     arm_backward
* Input:             None
*Output:             None
*Logic:              To bend the arm backward
*Example Call:       arm_backward();
*/
void arm_backward()
{
	for(ii=60;ii>0;ii--)
	{
		servo_2(ii);
		_delay_ms(10);
	}
	
}

/*
* Function Name:     reset1
* Input:             None
*Output:             None
*Logic:              To free all the servo motors used in the arm
*Example Call:       reset1();
*/
void reset1()
{
	servo_1_free();
	servo_2_free();
	servo_3_free();
}

/*
* Function Name:     defaulty
* Input:             None
*Output:             None
*Logic:              To bring the arm to its default position
*Example Call:       defaulty();
*/
void defaulty()
{
	servo_1(95);
	_delay_ms(300);
	servo_2(0);
	_delay_ms(300);
}


/*
* Function Name:     drop
* Input:             None
*Output:             None
*Logic:              To place the animals in their respective habitats
*Example Call:       drop();
*/
void drop()
{   
	for(ii=95;ii<165;ii++)
	{
		servo_1(ii);
		_delay_ms(10);
	}
	_delay_ms(500);
	for(ii=0;ii<40;ii++)
	{
		servo_2(ii);
		_delay_ms(10);
	}
	_delay_ms(500);
	gripper_open();
	_delay_ms(500);
	servo_2(0);
	_delay_ms(500);
	
}

/*
* Function Name:     front
* Input:             None
*Output:             None
*Logic:              To pick the animals
*Example Call:       front();
*/
void front()
{
	gripper_open();
	_delay_ms(300);
	arm_forward();
	_delay_ms(300);
	gripper_close();
	_delay_ms(300);
	arm_backward();
	_delay_ms(300);
}