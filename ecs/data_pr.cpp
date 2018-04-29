#include "predict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_pr.h"


VM** p_fla={NULL};
PHYSER phy[3]={'\0'};
int fla_num=0;

//从buf里面 得到第cx个空格的位置					  
char NMEA_Spa_Pos(char *buf, char cx)
{
	char* p = buf;
	while (cx)
	{
		if (*buf == ' ')
			cx--;
		buf++;
	}
	return buf - p;
}

//求m^n函数
double NMEA_Pow(int m, char n)
{
	double result = 1;
	while (n--)result *= m;
	return result;
}

//判断某个日期从年初（1月1日）到该天（y年m月d日）的天数
int days(int y,int m,int d)
{
	int days[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	int i;
	int sum = 0;
	for(i=0;i<m;i++)
	sum = sum+days[i];
	sum = sum+d-1;
	return sum;
}

//求绝对值函数
int abs(int a,int b)
{
	if(a>=b)
	return (a-b);
	else
	return (b-a);
}


//字符串转换成整形，从buf到第一个不是数字的字符
int Str2num(char* buf)
{
	int res=0;
	int i=0,len = 0;
	char* p = buf;
	while(1)
	{
		//if(*p == ' ' || *p == '\n')break;
		if(*p<'0'||*p>'9')break;
		len++;
		p++;
	}

	for(i=0;i<len;i++)
	{
		res += NMEA_Pow(10,len-1-i)*(buf[i]-'0');
	}
	return res;
}
int yea_end=0,dat_end=0;
//提取训练数据文件，通过二维数组TD返回结果
void Extract_Data(char* data[MAX_INFO_NUM],int TD[30][1000],int &data_num)
{
	int i=0,j=0,fla=0,dat=0,flag=0;
	int yeai=0,moni=0,dayi=0,dati=0;
	for(i=0;i<30;i++)
		for(j=0;j<100;j++)
		{
			TD[i][j]=0;
		}
	if(*(data[0]+21)>='0' && *(data[0]+21)<='9')//规格是两位数
		{
			flag=1;
		}
	int yea1 = Str2num(data[0]+22+flag);
	int mon1 = Str2num(data[0]+27+flag);
	int day1 = Str2num(data[0]+30+flag);
	int dat1 = days(yea1,mon1,day1);
	
	for(i=0;data[i];i++)
	{
		fla=0;
		dat=0;
		flag=0;
		if(*(data[i]+21)>='0' && *(data[i]+21)<='9')//规格是两位数
		{
			flag=1;
			fla=(*(data[i]+20)-'0')*10+(*(data[i]+21))-'0';
		}
		else
		{
			fla=*(data[i]+20)-'0';
		}

		yeai = Str2num(data[i]+22+flag);
		moni = Str2num(data[i]+27+flag);
		dayi = Str2num(data[i]+30+flag);
		dati = days(yeai,moni,dayi);

		if(yea1 == yeai)
		dat = dati-dat1;

		else
		dat = 365-(dat1-dati);
		TD[fla][dat]++;
	}
	dat_end=dati;
	yea_end=yeai;
	data_num = dat;
		
}


//提取输入文件里的数据
void Input_Data(char* info[MAX_INFO_NUM],int HS[6])
{
	int i=0,inp_time=0,ser_num=0;
	char posx = 0;
	char* p1={NULL};
	ser_num=Str2num(info[0]);
	HS[0]=ser_num;//几种物理服务器
	int flag=0;

	fla_num = Str2num(info[ser_num+2]);//预测的虚拟机有几种
	HS[3] = fla_num;

	for(i=0;i<ser_num;i++)
	{
		for(int j=0;*(info[i+1]+j)!=' ';j++)
		{
			phy[i].sort[j] = *(info[i+1]+j);
		}
		posx = NMEA_Spa_Pos(info[i+1],1);
		phy[i].cpu_num = Str2num(info[i+1]+posx);
		posx = NMEA_Spa_Pos(info[i+1],2);
		phy[i].mem_num = Str2num(info[i+1]+posx);
		posx = NMEA_Spa_Pos(info[i+1],3);
		phy[i].hd_num = Str2num(info[i+1]+posx);
	}
	
	p_fla = (VM**)malloc(fla_num*sizeof(VM*));//给指针本身分配内存

	for(i=0;i<fla_num;i++)			//提取那个
	{
		p_fla[i] = (VM*)malloc(sizeof(VM));
		p_fla[i]->fla = Str2num(info[ser_num+3+i]+6);//10单个虚拟机规格，cpu为1位数
		p1 = info[ser_num+3+i];
		posx=NMEA_Spa_Pos(p1,1);
		p_fla[i]->cpu_num = Str2num(info[ser_num+3+i]+posx);
		posx=NMEA_Spa_Pos(p1,2);
		p_fla[i]->mem_num = Str2num(info[ser_num+3+i]+posx);
	}

	//strcpy(c_or_m,info[4+fla_num]);
	//printf("%s\n",c_or_m);
    //起始日期
	int yea1=Str2num(info[ser_num+4+fla_num]);
	int mon1=Str2num(info[ser_num+4+fla_num]+5);
	int day1=Str2num(info[ser_num+4+fla_num]+8);
	int dat1=days(yea1,mon1,day1);
	//结束日期
	int yea2=Str2num(info[ser_num+5+fla_num]);
	int mon2=Str2num(info[ser_num+5+fla_num]+5);
	int day2=Str2num(info[ser_num+5+fla_num]+8);
	int dat2=days(yea2,mon2,day2);
	if(Str2num(info[ser_num+4+fla_num]+11) == Str2num(info[ser_num+5+fla_num]+11))
		flag=0;
	else
		flag=1;

	if(yea1==yea2)
		inp_time=dat2-dat1+flag;
	else
		inp_time=365-(dat1-dat2)+flag;
	HS[4] = inp_time;

	/**********训练结束日期与预测开始日期的间隔存在HS[5]***********/
	if(yea_end==yea1)
		HS[5]=dat1-dat_end;
	else
		HS[5]=365-(dat_end-dat1);//HS[5]=1表示挨着，中间没有间隔
}

