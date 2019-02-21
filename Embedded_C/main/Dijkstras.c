 #include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define INF 999
#define TRUE 1
#define FALSE 0
void adjacent(int x1,int y1,int adj[4][2]);
int min_dis(int dist[], int included[]);
char * sh_path(int start, int end,int *);
char* dijkstra(int start,int end,int *count);

int grid[6][6]={
    {1,2,3,4,5,6},
    {7,8,9,10,11,12},
    {13,14,15,16,17,18},
    {19,20,21,22,23,24},
    {25,26,27,28,29,30},
    {31,32,33,34,35,36}};

int transf[37]={99,30,31,32,33,34,35,24,25,26,27,28,29,18,19,20,21,22,23,12,13,14,15,16,17,6,7,8,9,10,11,0,1,2,3,4,5};
int p_nx=0,p_ny=0,n_nx=0,n_ny=0;


void adjacent(int x1,int y1,int adj[4][2])
{
    int i=0,x=0,y=0;
    int key_x[4]={-1,0,1,0};//N E S W
    int key_y[4]={0,1,0,-1};
    for(i=0;i<4;i++)
	{
        adj[i][0]=-1;
        adj[i][1]=-1;
        x=x1+key_x[i];
        y=y1+key_y[i];
        if(x>-1 && y>-1 && x<6 && y<6)
		{
            adj[i][0]=x;
            adj[i][1]=y;
        }
    }
}

int min_dis(int dist[], int included[])
{
    int min=INF,min_index=0,i=0;
    for(i=0;i<36;i++)
    {
        if((included[i]==FALSE)&&dist[i]<min)
        {
            min=dist[i];
            min_index=i;
        }
    }
    return min_index;
}

char align(int turn_count)
{
turn_count=turn_count%3;
if(turn_count==2||turn_count==-2)
	return 'U';
else if(turn_count==1||turn_count==-3)
	return 'L';
else if(turn_count==-1||turn_count==3)
	return 'R';
else
	return 'N';
}

char rotate(int current,int next,int *turn_count)
{
     int dir_x,dir_y;
     dir_x=(current/6)-(next/6);
     dir_y=(current%6)-(next%6);
     if(dir_y==0)
	 {
        if(dir_x<0)
		{
		// printf("Uturn\n");
			*turn_count+=2;
			return 'U';
			p_nx=1;
			p_ny=0;
        }
        else
        {
        //printf("NC\n");
			return 'N';
        }
     }
     else if(dir_x==0)
     {
      if(dir_y<0)
	  {
      //printf("Right\n");
		*turn_count=*turn_count+1;
		p_nx=0;
		p_ny=1;
		return 'R';
      }
      else
	  {
      //printf("Left\n");
		p_nx=0;
		p_ny=-1;
		*turn_count=*turn_count-1;
		return 'L';
      }
    }
}

char direction(int current,int next,int *turn_count)
{
     n_nx=(next/6)-(current/6);
     n_ny=(next%6)-(current%6);
     char out='o';
     if(n_nx==p_nx && n_ny==p_ny)
	 {
		out='n';
     }
     else if(n_nx==(-p_nx) && n_ny==(-p_ny))
	 {
		//*turn_count+=2;
		out='n';
     }
     else if(p_nx!=0)
	 {
		if((p_nx==n_ny ))
		{
			*turn_count+=-1;
			out= 'l';
		}
		else
		{
			*turn_count+=1;
			out= 'r';
		}
    }
    else if(p_ny!=0)
	{
		if(p_ny==n_nx)
		{
			*turn_count+=1;
			out= 'r';
		}
		else
		{
		*turn_count-=1;
		out= 'l';
		}
    }
    p_nx=n_nx;
    p_ny=n_ny;
    return out;
}

char* sh_path(int start, int end,int *count1)
{
    int dist[36],parent[36];
    int count=0,turn_count=0,current=0,length=0,weight=1,u=0,v=0,i=0,j=0;
    int included[36];
    int *p;
    int adj[4][2]={{0,0},{0,0},{0,0},{0,0}};
    char dir,*path;
    p_nx=0,p_ny=0,n_nx=0,n_ny=0;
    for(i=0;i<36;i++)
	{
        dist[i]=INF;
        included[i]=FALSE;
        parent[i]=INF;
    }
    dist[start]=0;
    parent[start]=start;
    for(i=0;i<35;i++)
	{
        u=min_dis(dist,included);
    //    printf("%d\n",u);
        included[u]=TRUE;
        p_nx=u/6;
        p_ny=u%6;
        adjacent(p_nx,p_ny,adj);
        for(j=0;j<4;j++)
		{
            if(adj[j][0]!=-1)
			{

                n_nx=adj[j][0];
                n_ny=adj[j][1];
                v=grid[n_nx][n_ny]-1;
                //printf("%d  %d\n",n_nx,n_ny);
                if((included[v]==FALSE)&&(dist[v]>(dist[u]+weight)))   // d[u,v] > d[u,i]+d[i,v] here d[i,v] is always 1 for a grid
                    {
						dist[v]=dist[u]+weight;
						parent[v]=u;
                    }
            }
        }
    }

    p=(int*)malloc(sizeof(int));
    current=end;
    p[0]=current;
    while(current!=start)
	{
		length++;
		current=parent[current];
		p=(int*)realloc(p,((length+1)*sizeof(int)));
		p[length]=current;
    }


    p_nx=-1,p_ny=0,n_nx=0,n_ny=0;// Starting with facing north

    dir=rotate(p[length],p[length-1],&turn_count);

    path=(char*)malloc((length+1)*sizeof(char *));
    path[0]=dir;
    *count1=0;
    for(i=length-1;i>0;i--)
	{
		*count1+=1;
		dir=direction(p[i],p[i-1],&turn_count);
		path[*count1]=dir;
    }
    *count1+=1;
    path[*count1]=align(turn_count);
	*count1+=1;
	path[*count1]='\0';
    free(p);
    return path;
}

//int main(){
//int i=0,j=0,count=0;
//int *p;
//int start=1;
//int end=25;
//char *path;
//for(i=0;i<5;i++){
//    for(j=0;j<5;j++)
//    printf("%*d  ",4,grid[i][j]-1);
//    printf("\n");}
//path=dijkstra(1,25,&count);
//for(i=0;i<=count;i++)
//printf("%c\n",path[i]);
//return 0;
//}

char* dijkstra(int start,int end,int *count)
{
	char *path;
	int i=0,j=0,m=31;
		if(start==end)
    {
        path=(char*)malloc(2*sizeof(char));
        path[0]='N';
        path[1]='\0';
    }
    else{
	start=transf[start];
	end=transf[end];
	path=sh_path(start,end,count);}
	return path;
}
