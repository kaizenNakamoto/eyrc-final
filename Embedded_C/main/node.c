#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int graph[6][6]={
{1,2,3,4,5,6},
{7,8,9,10,11,12},
{13,14,15,16,17,18},
{19,20,21,22,23,24},
{25,26,27,28,29,30},
{31,32,33,34,35,36}};

	int aniloc[20];
	int habloc[20];

	int animals(char a[2]){
		//printf("-----%s\n",a);
		int animal=0,number=0,letter=0;
		if(a[0] >= 'A' && a[0] <= 'F')
		letter = a[0]+1 - 'A';
		if(a[1] >='1' && a[1] <= '6')
		number = a[1] - '0';

		animal = graph[number-1][letter-1];
		// if(strcmp("A1",a)==0)
		// animal++;
		return animal;
	}

	int habitats(char h[2]){
		int i=0,m=0,habitat=0;
		for ( i = 0; h[i] != '\0'; i++)
		m = m * 10 + h[i] - '0';

		if(m<=5)
		habitat = 1 + (m-1);
		else if(m>=6 && m<=10)
		habitat = 2 + (m-1);
		else if(m>=11 && m<=15)
		habitat = 3 + (m-1);
		else if(m>=16 && m<=20)
		habitat = 4 + (m-1);
		else if(m>=21 && m<=25)
		habitat = 5 + (m-1);
		return habitat;
	}

	void node_map(char *tokenstring,int type, int *len)
	{
		char a[2];
		char h[2];
		char seps[] = ", ";
		char* token;
		int var;
		int i = 0;
		token = strtok (tokenstring, seps);
		while (token != NULL)
		{
			if(type==0)
			{//Animal
				aniloc[i]=animals(token);
			}
			else
			{
				habloc[i]=habitats(token);
			}
			sscanf (token, "%d", &var);
			i++;
			token = strtok (NULL, seps);
		}
		*len=i;
	}
