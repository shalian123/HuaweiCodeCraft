#include<stdio.h>
#include "rm_noise.h"
#include "data_pr.h"
#include<math.h>

#define PI 3.1416 

void rm_noise1(int TD[30][1000],int row,int colu,int N)
{
    printf("in\n");
    int i=0,j=0,avera=0;
    float ave=0.0;
 
    for(i=0;i<=row;i++)
        for(j=0;j<colu;j++)
        {
            ave=0.0;
            avera=0;

            if(j==0)
				ave=TD[i][1];
			else if(j==colu-1)
				ave=TD[i][j-1];
            else
            //ave=(TD[i][j-2 ]+TD[i][j-1]+TD[i][j+1]+TD[i][j+2])/(float)4;
            ave=(TD[i][j-1]+TD[i][j+1])/(float)2;
           // avera=(int)(ave+0.5)>(int)ave?(int)ave+1:(int)ave;
           avera=round(ave);
            if(TD[i][j]>ave*N)
			{
                if(avera==0)
                    avera=1;
                TD[i][j]=avera;
			}
        }
        // printf("out\n");
}





void get_gau_ker(int size,float* kernel,float sigma)
{
    int i=0;
    float sum=0.0;
    for(i=0;i<=(size-1)/2;i++)
    {
        kernel[i+(size-1)/2]= (1/(sqrt(2*PI)*sigma)*exp(-1*(i*i/(2*sigma*sigma))));
        kernel[(size-1)/2-i]= (1/(sqrt(2*PI)*sigma)*exp(-1*(i*i/(2*sigma*sigma))));
        
    }
    for(i=0;i<size;i++)
    {
        sum+=kernel[i];
        printf("sum=%f\n",sum);
    }
    for(i=0;i<size;i++)
    {
        kernel[i]=kernel[i]/sum;
        printf("kernel[%d]=%f\n",i,kernel[i]);
    }  
}




void rm_noise2(int TD[30][1000],int row,int colu,int N)
{
    int i,j,k;
    float a[5]={0},temp=0.0;
    get_gau_ker(5,a,1);

    for(i=0;i<=row;i++)
        for(j=2;j<colu-2;j++)
        {
            
            temp=0;
            for(k=0;k<5;k++)
            {
                 temp+=TD[i][j-(2-k)]*(a[k]);
            }
           // temp=temp+1;
            TD[i][j]=(int)(temp+0.5)>(int)temp?(int)temp+1:(int)temp;
    
        } 
    printf("out\n");
}

