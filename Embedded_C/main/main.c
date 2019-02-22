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

void pick_assign(int nxt_node){
int left[6]={7,13,19,25,99,99};
int right[6]={12,18,24,30,99,99};
int back[6]={2,3,4,5,99,99};
int front[6]={32,33,34,35,99,99};
int temp_var=0;
for(temp_var=0;temp_var<6;temp_var++){
    if(left[temp_var]==nxt_node)
        {
        strcat(master_path,"1");
        return; }
    else if(right[temp_var]==nxt_node)
              {
        strcat(master_path,"2");
        return; }
    else if(back[temp_var]==nxt_node)
              {
        strcat(master_path,"3");
        return; }
    else if(nxt_node==1)
              {
        strcat(master_path,"4");// first node
        return; }
    else if(nxt_node==6)
              {
        strcat(master_path,"5");// first row last col
        return; }
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
    return; }
}

}
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
	char animal[]="A1, A2, F1";
	char habit[]="1, 8, 25";
	path_fin=0;
	halt=0;
	defaulty();
	while(!complete)
	{	
		if((PINE & 0x80) == 0x80) //when BOOT switch is not pressed
		{	
			if(switch_pressed==1)//after press
			{
				if(!mapped)
				{	mapped=1;
					node=0;
					node_map(animal,0,&lena);
					node_map(habit,1,&lenb);
					size=lena;
					size=size*2-1;
					    p= dijkstra(1,aniloc[0],&lenp);
					    strcat(master_path,p);
					    pick_assign(aniloc[0]);
					    free(p);
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
						    strcat(master_path,p);
						    free(p);
						    if(var%2==0)
						    strcat(master_path,"D");
						    else{
							    pick_assign(next_node);
						    }

					    }
					    strcat(master_path,"o");
					
				lcd_cursor(1,1);
				lcd_wr_char(master_path[18]);
				}//end of mapping
				travel_path(master_path[0]);
				TIMSK3=1;
				while(!path_fin){
					if(!halt){
						line_follow();
					_delay_ms(50);}
					//display_line();
					//_delay_ms(20);
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
	_delay_ms(2000);
	buzzer_off();
	_delay_ms(2000);
	
}								