/************************************************************************/
/*Team ID : HC#681
  Theme   : Homecoming eYRC2k18
  Author List : Kiran RS, Karthik S Nayak, Chinmai R, Mithun M R 
  File Name : main.c                                                           */
/************************************************************************/


#define F_CPU 14745600
//Header files

# include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
# include <math.h> //included to support power function

#include "lcd.c"
#include "motion_control.c"
#include "sensor_reading.c"
#include "line_follower.c"
#include "utils.c"
#include "timers.c"
#include "init.c"
#include "Dijkstras.c"
#include "node.c"
#include "uart.c"

//Function to configure Interrupt switch
void interrupt_switch_config (void)
{
	DDRE = DDRE & 0x7F;  //PORTE 7 pin set as input
	PORTE = PORTE | 0x80; //PORTE7 internal pull-up enabled
}
/************************************************************************/
/* This Functions is used to insert pick and drop identifiers at the end of each path
   various identifiers are used for different cases throughout the run.
   
                                                                        */
/************************************************************************/
void pick_assign(int nxt_node)
{
	int left[6]={7,13,19,25,99,99}; // Left Pick
	int right[6]={12,18,24,30,99,99}; // Right Pick
	int back[6]={2,3,4,5,99,99};  // Back Pick
	int front[6]={32,33,34,35,99,99}; // Front Pick
	int temp_var=0;
	for(temp_var=0;temp_var<6;temp_var++)
	{
		if(left[temp_var]==nxt_node)
        {
			strcat(master_path,"1");
			return;
		}
		else if(right[temp_var]==nxt_node)
       {
			strcat(master_path,"2");
			return;
	   }
		else if(back[temp_var]==nxt_node)
       {
			strcat(master_path,"3");
			return;
	   }
		else if(nxt_node==1)
        {
			strcat(master_path,"4");// first node
			return;
		}
		else if(nxt_node==6)
        {
			strcat(master_path,"5");// first row last col
			return;
		}
		else if(nxt_node==31)//
		{
			strcat(master_path,"6");//last row first col
			return;
		}
		else if(nxt_node==36)
		{
			strcat(master_path,"7");//last row last col
			return;
		}
		else if(front[temp_var]==nxt_node)
		{
			strcat(master_path,"8");
			return;
		}
	}
}

/************************************************************************/
/* This Program initializes all init devices and port configures throughout the program and
   integrates everything together.                                                                     */
/************************************************************************/
int main()
{
	lcd_port_config();
	init_devices();
	lcd_set_4bit();
	lcd_init();
	int lena,lenb,lenp;
	int present_node=0, next_node=0,t_count=0;
	int switch_pressed = 0, mapped=0,size=0,an_count=0,hab_count=0,var=0,complete=0;
	char *p;
	path_fin=0;
	halt=0;
	// Setting Servos to initial positions
	defaulty();
	while(!complete)// Checking for complete flag
	{	
		if((PINE & 0x80) == 0x80) //when BOOT switch is not pressed
		{	
			if(switch_pressed==1)//after press
			{
				if(!mapped) // To generate path only once during the lifetime of the run.
				{	
					mapped=1;
					node=0;
					// Calling mapping functions to convert locations from the Serial data to node numbers for the Dijkstra Algorithm 
					node_map(ani,0,&lena);
					node_map(hab,1,&lenb);
					//Setting size to iterate to generate all paths required
					size=lena;
					size=size*2-1;
					//p holds temp path for each cycle
					p= dijkstra(1,aniloc[0],&lenp);
					strcat(master_path,p);
					//To align the bot to initial direction
					pick_assign(aniloc[0]);
					//Freeing data to prevent OUT_OF_MEMORY
					free(p);
					//Generating path
					for(var=0;var<size;var++)
					{
						if(var%2==0)
						{
							present_node=aniloc[an_count];
							next_node=habloc[hab_count];
							an_count++;
							hab_count++;
						}
						else
						{
							present_node=habloc[hab_count-1];
							next_node=aniloc[an_count];
						}
						p=dijkstra(present_node,next_node,&lenp);
						// Appending to master_path array
						strcat(master_path,p);
						free(p);
						if(var%2==0)
						strcat(master_path,"D");
						else
						{
							pick_assign(next_node);
						}
					}
					// appending o to indicate END of Program
					strcat(master_path,"o");
				}//end of mapping
				
				travel_path(master_path[0]);
				// To Initialize Timer to read Line sensing ADC sensors at equal intervals
				TIMSK3=1;
				while(!path_fin)// Checking if END has arrived
				{
					if(!halt)// If bot is not halted to travel
					{
						line_follow();// This calls line conditions which sets the line following motion of the bot and calls travel path during encountering a node.
						_delay_ms(50);
					}
				}
			stop();
			complete=1;
			}
		}							  	
		else//BOOT switch is pressed
		{
			lcd_cursor(1,1);
			lcd_string(ani);
			lcd_cursor(2,1);
			lcd_string(hab);
			switch_pressed=1;
		}
	}
	buzzer_on();
	_delay_ms(5000);
	buzzer_off();
}								