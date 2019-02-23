#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>

#include <string.h>

#include "servo.c"
//#include "lcd.c"
volatile int line_cond = 0;

volatile int en_left = 0, en_right = 0, en_node = 1;

volatile int left_flag = 0, right_flag = 0, node_flag;

volatile int left_turn = 0, right_turn = 0;

volatile int rot=0,rotl=0,uturns=0;

volatile int node = 0,halt=0;

char master_path[200]="";

unsigned char prev;

int path_fin = 0;

/*



* Function Name: 	read_line



* Input: 			None



* Output: 			None



* Logic:			Read left, center and right white line sensor values and store in global variables



* Example Call:		read_line();



*/

void read_line()

{

    Left_white_line = (int) ADC_Conversion(3); //Getting data of Left WL Sensor

    Center_white_line = (int) ADC_Conversion(2); //Getting data of Center WL Sensor

    Right_white_line = (int) ADC_Conversion(1); //Getting data of Right WL Sensor

}



/*



* Function Name: 	display_line



* Input: 			None



* Output: 			None



* Logic:			Display line sensor readings on LCD



* Example Call:		display_line();



*/

void display_line()

 {

    lcd_print(1,1,Left_white_line,3);

    lcd_print(1,5,Center_white_line,3);

    lcd_print(1,9,Right_white_line,3);

    lcd_print(1,1,line_cond,1);

}



/*



* Function Name: 	clear_count



* Input: 			None



* Output: 			None



* Logic:			Clear all line follower flags and turn/node counts



* Example Call:		clear_count();



*/

void clear_count()

{

    node = 0;

    node_flag = 0;

    left_flag = 0;

    right_flag = 0;

}



/*



* Function Name:	enable_flags 	



* Input: 			l_flag (int) -> 0 - Disable / 1 -> Enable



					n_flag (int) -> 0 - Disable / 1 -> Enable



					r_flag (int) -> 0 - Disable / 1 -> Enable 



* Output: 			None



* Logic:			- Function to enable/disable left/right turn or node detection



* Example Call:		enable_flags(0,1,0); -> Detect only nodes, ignore left and right turns



*/

void enable_flags(int l_flag, int n_flag, int r_flag)

{

    en_left = l_flag;

    en_right = r_flag;

    en_node = n_flag;

}

/*



* Function Name: 	line_conditions	



* Input: 			None



* Output: 			None



* Logic:			- Based on the position of robot on line, the line condition flag is set



					- NOTE: This functions only checks the condition and sets the line follower flags, doesn't follow the line



					- Line following and Line detection are done separately, in order to increase the sampling rate of the 



					  sensors so that the sensors do not miss out nodes and turns when the robot is moving at higher velocities



* Example Call:		line_conditions();	



*/

void line_conditions()
{
	// Nodes Condition
	if (Center_white_line >= 115 || Left_white_line >= 120 || Right_white_line >= 115 || (Center_white_line >=120 && (Left_white_line >= 110 || Right_white_line >= 110))) // 1 1 1
	{
		line_cond = 1;
		node_flag = 1;
	}
	
	//Straight Line Condition
	else if (Left_white_line < 11  && Center_white_line >= 14 && Right_white_line < 11) // 0 1 0
	{
		line_cond = 2;
	}
	
	//Straight Line Condition (Robot moved slightly towards right)
	else if (Left_white_line > 20 && Center_white_line < 11 && Right_white_line < 11) // 1 0 0
	{
		line_cond = 3;
	}

        else if (Left_white_line > 10 && Center_white_line > 10 && Right_white_line < 11) // 1 1 0
	{
		line_cond = 6;//slighty_L
	}

        
	//Straight Line Condition (Robot moved slightly towards left)
	else if (Left_white_line < 11 && Center_white_line < 11 && Right_white_line > 20) // 0 0 1
	{
		line_cond = 4;
	}

        else if (Left_white_line < 11 && Center_white_line > 11 && Right_white_line > 11) // 0 1 1
	{
		line_cond = 7;//slighty_R
	}

	//No Line Condition
	else if (Left_white_line < 11 && Center_white_line < 11 && Right_white_line < 11) // 0 0 0
	{
		line_cond = 5;
	}

}
/*



* Function Name: 	line_follow	



* Input: 			None



* Output: 			None



* Logic:			- Follow the line based on the line conditions and turn flags set



					- Line following and Line detection are done separately, in order to increase the 



					  sampling rate of the sensors so that the sensors do not miss out nodes and turns



					  when the robot is moving at higher velocities



					- To avoid duplicate detection of nodes and turns, after the first detection, timer 4 is enabled for



					  a short duration (0.5 sec). As long as the timer 4 is running, duplicate detections will be discarded.



* Example Call:		line_follow();



*/

void line_follow() 

{

    if (t4_flag == 1) 

	{

        node_flag = 0;

        left_flag = 0;

        right_flag = 0;

    }



    if (node_flag == 1)

	 {

        start_timer4(10);

		buzz(50);

		node++;		

		forward();

		velocity(255,255);

        node_flag = 0;

		travel_path(master_path[node]);

	 }



    else if (line_cond == 2)

	{

        forward();

        velocity(255, 255);

    } 

	else if (line_cond == 3)

	{

        stop();

        _delay_ms(10);

        forward();

        velocity(180, 255);

    } 

	else if (line_cond == 4)

	{

        stop();

        _delay_ms(10);

        forward();

        velocity(255, 180);

    } 

	else if (line_cond == 5)

	{

        forward();

        velocity(255, 255);

        }

       else if (line_cond == 6)

	{
        stop();_delay_ms(10);

        forward();

        velocity(250, 255);

        }

        else if (line_cond == 7)

	{
        stop();
		_delay_ms(10);

        forward();

        velocity(255, 250);

        }
	else
	{
		forward();
		velocity(254,255);
	}
}


void pick_drop(char pd)
{
	int pdback=125;
	defaulty();
	stop();
	_delay_ms(20);
	if(master_path[node-1]=='L'||master_path[node-1]=='R'||master_path[node-1]=='U')
	{
	if(pd=='7'||pd=='6')
	{
		back_mm(30);//mod
		_delay_ms(20);//mod
	}
	}
	
	if(pd=='D'){
		if(master_path[node-1]!='N'){
		back_mm(70);
		_delay_ms(20);}
		drop();
		_delay_ms(100);
	}
		if(pd == '1' )//left animals
		{
			if(master_path[node-1]=='U'){
			forward_mm(40);
			_delay_ms(20);
			spl_left2();
			_delay_ms(50);
			front();
			_delay_ms(100);
			correct_right();
			back_mm(pdback);
			_delay_ms(20);
			}
			else if(master_path[node-1] == 'N')
			{
					//left_cycle();
					forward_mm(140);
					_delay_ms(20);
					spl_left2();
					_delay_ms(50);
					front();
					_delay_ms(100);
					correct_right();
					back_mm(pdback);
					_delay_ms(20);
				}
				else if(master_path[node-1] == 'R')
				{
					//spl_left2();
					_delay_ms(50);
					front();
					_delay_ms(100);
					correct_right();
					back_mm(70);
					_delay_ms(20);
				}
		}
		if(pd == '2')//right animals
		{
			if(master_path[node-1]=='U')
			{
			forward_mm(40);
			_delay_ms(20);
			spl_right2();
			_delay_ms(50);
			front();
			_delay_ms(100);
			correct_left();
			back_mm(pdback);
			_delay_ms(20);
			}
			else if(master_path[node-1] == 'N')
			{
				//right_cycle();
				forward_mm(140);
				_delay_ms(20);
				spl_right2();
				_delay_ms(50);
				front();
				_delay_ms(100);
				correct_left();
				back_mm(pdback);
				_delay_ms(20);
			}
			
			else if(master_path[node-1] == 'L')
			{
				//spl_right2();
				_delay_ms(50);
				front();
				_delay_ms(100);
				correct_left();
				back_mm(70);
				_delay_ms(20);
			}
		}
					
	else if(pd=='3')
	{//back array
		stop();
		_delay_ms(20);
		front();
		_delay_ms(200);
		forward_mm(60);
		_delay_ms(20);
		correct_right3();
		_delay_ms(20);
		stop();
		_delay_ms(20);
		forward_mm(20);
		_delay_ms(20);
	}
	else if(pd=='4')
	{	// first node
		if(master_path[node-1]=='N'){
		forward_mm(35);
		spl_left();
		back_mm(50);
		_delay_ms(20);
		front();
		forward_mm(50);
		_delay_ms(20);
		spl_right();
		_delay_ms(20);
		back_mm(25);
		_delay_ms(300);
		}		
	}
	else if(pd=='5')//F1
	{// 1X6 
			if(master_path[node-1]=='U')
			{
			back_mm(20);
			_delay_ms(20);
			spl_right();
			stop();
			_delay_ms(20);
			back_mm(50);
			_delay_ms(20);
			front();
			_delay_ms(100);
			forward_mm(50);
			_delay_ms(20);
			spl_left();
			}
			else
			{
				_delay_ms(20);
				forward_mm(55);
				_delay_ms(20);
				spl_right();
				_delay_ms(20);
				back_mm(20);
				_delay_ms(20);
				front();
				_delay_ms(100);
				forward_mm(20);
				_delay_ms(20);
				spl_left();
				back_mm(20);
			}			
	}
	else if(pd=='6'){// 6X1 A6
		if(master_path[node-1]=='L'||master_path[node-1]=='R'){
			forward_mm(20);
			_delay_ms(20);
			front();
			_delay_ms(10);
			back_mm(20);
		} 
		else{
			forward_mm(35);
			_delay_ms(20);
			spl_left();
			_delay_ms(20);
			front();
			_delay_ms(100);
			right_degrees(50);
			_delay_ms(20);
			back_mm(55);
			_delay_ms(20);
		}       
	}
		else if(pd=='7'){//6X6 F6
				if(master_path[node-1]=='L'||master_path[node-1]=='R'){
					stop();
					_delay_ms(20);
					front();
					_delay_ms(100);
				}
				else{
					forward_mm(35);
					_delay_ms(20);
					spl_right();
					front();
					_delay_ms(100);
					left_degrees(50);
					_delay_ms(20);
					back_mm(55);
					_delay_ms(20);
				}
		}
			else if(pd=='8'){
				front();
				_delay_ms(100);
				forward_mm(60);
				_delay_ms(20);
			}
defaulty();
}
void travel_path(char pos_val)
{   TIMSK3=0;
	halt=1;
	stop();
	_delay_ms(20);
	if(pos_val=='n'){
	forward();
	}
	else if(pos_val=='r')
	correct_right();
	else if(pos_val=='l')
	correct_left();
	else if(pos_val=='R')
	{
		if(node==0)
		correct_right();
		else if(master_path[node+1]=='1'||master_path[node+1]=='2'||master_path[node+1]=='3'||master_path[node+1]=='4'||master_path[node+1]=='5'||master_path[node+1]=='6'||master_path[node+1]=='7'||master_path[node+1]=='8')
		{
			stop();
			if(master_path[node+1]=='7'||master_path[node+1]=='6')// TO ALIGN TO NORTH SLANT LINE ONLY
			{
				forward_mm(25);
				_delay_ms(20);
				correct_right();
			}
			else if(master_path[node+1]=='8'){//if bot comes from right
				stop();
				_delay_ms(20);
				forward_mm(160);
				_delay_ms(20);
				 spl_right2();
				_delay_ms(20);
			}
			else if(master_path[node+1] == '3')
			{
				stop();
				_delay_ms(20);
				forward_mm(160);
				_delay_ms(20);
				spl_left2();
				_delay_ms(20);
			}

			else
			{
				stop();
				_delay_ms(20);
			}
			node++;
			pick_drop(master_path[node]);
			node++;
			travel_path(master_path[node]);
		}
		else if(master_path[node+1]=='D')//mod
		{
			correct_right();
			_delay_ms(20);
			node++;
			pick_drop(master_path[node]);
			node++;
			travel_path(master_path[node]);
		}

		else{
			node--;
			if(master_path[node-1]=='L'||master_path[node-1]=='R'||master_path[node-1]=='U')
			correct_right2();
			else 
			correct_right();
			node++;		
		}
	}
		else if(pos_val=='L')
		{
			if(node==0)
			correct_left();
			else if(master_path[node+1]=='1'||master_path[node+1]=='2'||master_path[node+1]=='3'||master_path[node+1]=='4'||master_path[node+1]=='5'||master_path[node+1]=='6'||master_path[node+1]=='7'||master_path[node+1]=='8')
			{
				if(master_path[node+1]=='7'||master_path[node+1]=='6')
				{
					forward_mm(25);
					_delay_ms(20);
					stop();
					_delay_ms(25);
					correct_left();
				}
						else if(master_path[node+1]=='8'){
							stop();
							_delay_ms(20);
							forward_mm(160);
							_delay_ms(20);
							spl_left2();
							_delay_ms(20);
						}
						else if(master_path[node+1]=='3')
						{
						stop();
						_delay_ms(20);
						forward_mm(160);
						_delay_ms(20);
						spl_right2();
						_delay_ms(20);
						}						
				else
				{
					stop();
					_delay_ms(20);
				}					
				node++;
				pick_drop(master_path[node]);
				node++;
				travel_path(master_path[node]);
			}
			else if(master_path[node+1]=='D')//mod
			{
				correct_left();
				_delay_ms(20);
				node++;
				pick_drop(master_path[node]);
				node++;
				travel_path(master_path[node]);
			}
			else{
				node=node-2;
				if(master_path[node]=='R'||master_path[node]=='L'||master_path[node]=='U')
				correct_left2();
				else
				correct_left();
				node=node+2;
			}
		}
		else if(pos_val=='N'){
		if(node==0){
		if(master_path[node+1]=='D'||master_path[node+1]=='1'||master_path[node+1]=='2'||master_path[node+1]=='3'||master_path[node+1]=='4'||master_path[node+1]=='5'||master_path[node+1]=='6'||master_path[node+1]=='7'||master_path[node+1]=='8')
		{
						stop();
						_delay_ms(20);
						node++;
						pick_drop(master_path[node]);
						node++;
						travel_path(master_path[node]);
		}
		else
		forward();
		}
		else if(master_path[node-1]=='D'||master_path[node-1]=='1'||master_path[node-1]=='2'||master_path[node-1]=='3'||master_path[node-1]=='4'||master_path[node-1]=='5'||master_path[node-1]=='6'||master_path[node-1]=='7'||master_path[node-1]=='8')
		{  		if(master_path[node+1]=='D'||master_path[node+1]=='1'||master_path[node+1]=='2'||master_path[node+1]=='3'||master_path[node+1]=='4'||master_path[node+1]=='5'||master_path[node+1]=='6'||master_path[node+1]=='7'||master_path[node+1]=='8')
			{
				stop();
				_delay_ms(20);
				node++;
				pick_drop(master_path[node]);
				node++;
				travel_path(master_path[node]);
			}
			else
			 forward();
		}
		else
		{
			stop();
			_delay_ms(20);
			forward_mm(30);
			node++;
			pick_drop(master_path[node]);
			node++;
			travel_path(master_path[node]);
		}
		}
	else if(pos_val=='U'){
		if(master_path[node+1]=='D'||master_path[node+1]=='3'||master_path[node+1]=='4'||master_path[node+1]=='5'||master_path[node+1]=='6'||master_path[node+1]=='7'||master_path[node+1]=='8')
		{
			stop();
			_delay_ms(20);
			correct_right3();
			node++;
			pick_drop(master_path[node]);
			node++;
			travel_path(master_path[node]);
		}
		else if(master_path[node+1]=='1')
		{
			stop();
			_delay_ms(20);
			correct_left3();
			_delay_ms(20);
			node++;
			pick_drop(master_path[node]);
			node++;
			travel_path(master_path[node]);
		}
		else if(master_path[node+1]=='2')
		{
			stop();
			_delay_ms(20);
			correct_right3();
			_delay_ms(20);
			node++;
			pick_drop(master_path[node]);
			node++;
			travel_path(master_path[node]);
		}
		else
		correct_right3();
	}
	else if(pos_val=='o')
	{
		stop();
		path_fin=1;
	}
halt=0;	
TIMSK3=1;
}

/*
* Function Name: 	line_following

* Input: 			None

* Output: 			None

* Logic:			Enabling Timer3 Register to initiate Line following of the Bot

* Example Call:		line_following();
*/

void line_following()

{

	TIMSK3 = 0x01;

}





