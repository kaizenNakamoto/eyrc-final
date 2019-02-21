#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char ii = 0;
signed char jj = 0;

//Configure PORTB 5 pin for servo motor 1 operation
void servo1_pin_config (void)
{
 DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
 PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

//Configure PORTB 6 pin for servo motor 2 operation
void servo2_pin_config (void)
{
 DDRB  = DDRB | 0x40;  //making PORTB 6 pin output
 PORTB = PORTB | 0x40; //setting PORTB 6 pin to logic 1
}

//Configure PORTB 7 pin for servo motor 3 operation
void servo3_pin_config (void)
{
 DDRB  = DDRB | 0x80;  //making PORTB 7 pin output
 PORTB = PORTB | 0x80; //setting PORTB 7 pin to logic 1
}

//Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
void servo_1(unsigned char degrees)  
{
 float PositionPanServo = 0;
 PositionPanServo = ((float)degrees * 0.512) + 34.56;
 OCR1AH = 0x00;
 OCR1AL = (unsigned char) PositionPanServo;
}


//Function to rotate Servo 2 by a specified angle in the multiples of 1.86 degrees
void servo_2(unsigned char degrees)
{
 float PositionTiltServo = 0;
 PositionTiltServo = ((float)degrees * 0.512) + 34.56;
 OCR1BH = 0x00;
 OCR1BL = (unsigned char) PositionTiltServo;
}

//Function to rotate Servo 3 by a specified angle in the multiples of 1.86 degrees
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

void servo_1_free (void) //makes servo 1 free rotating
{
 OCR1AH = 0x03; 
 OCR1AL = 0xFF; //Servo 1 off
}

void servo_2_free (void) //makes servo 2 free rotating
{
 OCR1BH = 0x03;
 OCR1BL = 0xFF; //Servo 2 off
}

void servo_3_free (void) //makes servo 3 free rotating
{
 OCR1CH = 0x03;
 OCR1CL = 0xFF; //Servo 3 off
} 
void gripper_open()
{
	for(jj=0;jj<55;jj++)
	{
		servo_3(jj);
		_delay_ms(10);
	}
	_delay_ms(300);
}

void gripper_close()
{
	for(ii=55;ii>0;ii--)
	{
		servo_3(ii);
		_delay_ms(10);
	}
	
	_delay_ms(300);
}

void arm_rotate_left()
{
	for(ii=90;ii>0;ii--)
	{
		servo_1(ii);
		_delay_ms(10);
	}
	_delay_ms(300);
}

void arm_rotate_right()
{
	for(ii=90;ii<180;ii++)
	{
		servo_1(ii);
		_delay_ms(10);
	}
	_delay_ms(300);
}
void arm_straight()
{
	for(ii=0;ii<90;ii++)
	{
		servo_1(ii);
		_delay_ms(10);
	}
	_delay_ms(300);
}

void arm_forward()
{
	for(ii=0;ii<90;ii++)
	{
		servo_2(ii);
		_delay_ms(10);
	}
	_delay_ms(300);
	
}

void arm_backward()
{
	for(ii=90;ii>0;ii--)
	{
		servo_2(ii);
		_delay_ms(10);
	}
	_delay_ms(300);
}

void reset()
{
	_delay_ms(500);
	servo_1(0);
	_delay_ms(500);
	servo_2(0);
	_delay_ms(500);
	servo_3(0);
	_delay_ms(500);
}
void reset1()
{
	servo_1_free();
	servo_2_free();
	servo_3_free();
}


void defaulty()
{
	servo_1(90);
	_delay_ms(300);
	servo_2(0);
	_delay_ms(300);
}
void left_cycle()
{
	
	gripper_open();
	_delay_ms(300);
	arm_rotate_left();
	_delay_ms(300);
	arm_forward();
	_delay_ms(300);
	gripper_close();
	_delay_ms(300);
	arm_backward();
	_delay_ms(300);

}

void right_cycle()
{

	gripper_open();
	_delay_ms(300);
	arm_rotate_right();
	_delay_ms(300);
	arm_forward();
	_delay_ms(300);
	gripper_close();
	_delay_ms(300);
	arm_backward();
	_delay_ms(300);
	
}
void drop()
{
	servo_1(150);
	_delay_ms(300);
	
	for(ii=0;ii<70;ii++)
	{
		servo_2(ii);
		_delay_ms(10);

	}
	_delay_ms(300);
	gripper_open();
	_delay_ms(300);
	servo_2(0);
	_delay_ms(300);
	
}

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