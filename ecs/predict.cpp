#pragma GCC diagnostic error "-std=c++11"  
#include "predict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_pr.h"
#include <math.h>
#include "place.h"
#include "rm_noise.h"

void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{

	/**********************读取训练数据文件**************************/
	int TD[30][1000]={0};
	int data_num1=0;
	Extract_Data(data,TD,data_num1);//数据存到TD数组里，TD[15][0]=2；表示规格15第一天请求2台
	//printf("%d\n\n",TD[1][0]);	
	
	//rm_noise1(TD,15,data_num1,3);
	rm_noise2(TD,18,data_num1,3);
/*******************读取输入文件***************************/
	int HS[6]={0};//HS[0]~HS[2]表示输入文件中第一行物理服务器三个整数，HS[3]要预测几组，HS[4]要预测未来几天
	
	Input_Data(info,HS);//全局变量，结构体指针数组**p_fla存有具体虚拟机规格、cpu、内存信息
	
	/*************************去噪***************************/
	int* pTotalFlaNum = new int[fla_num];//预测出来各个规格的数量
	for(int i = 0;i<fla_num;i++)
		pTotalFlaNum[i] = 0;
	int* pVMName = new int[fla_num];//读出的各个VM规格
	for (int i = 0; i < fla_num; i++)
		pVMName[i] = p_fla[i]->fla;
	
	
	
/*********************************以下是算法部分****************************/

	/*************训练主代码部分**********************/
    
	/*********开始赋值*******/

	for(int i=0;i<fla_num;i++)
	{
		for(int j=0;j<data_num1;j++)
			pTotalFlaNum[i]+=TD[pVMName[i]][j];
		//pTotalFlaNum[i]=pTotalFlaNum[i]*2.1*HS[4]/data_num1;
	}	

	int sevplu1=0,sevplu2=0;
	//double proportion=0.0; //比例
	for(int i=0;i<fla_num;i++)
	{
		sevplu1=0;
		sevplu2=0;
		for(int j=data_num1-7;j<data_num1;j++)
			sevplu1+=TD[pVMName[i]][j];
		for(int j=data_num1-14;j<data_num1-7;j++)
			sevplu2+=TD[pVMName[i]][j];
		if(sevplu1>sevplu2)
			pTotalFlaNum[i]=pTotalFlaNum[i]*3*HS[4]/data_num1;
		else if(sevplu1<sevplu2)  //测出存在这种情况的
			pTotalFlaNum[i]=pTotalFlaNum[i]*1.45*HS[4]/data_num1;
			else pTotalFlaNum[i]=pTotalFlaNum[i]*2*HS[4]/data_num1; //测出存在七天值相同的情况
	    //proportion=(double)(HS[5]/HS[4]);
		//if(HS[5]<7)
			//pTotalFlaNum[i]-=pTotalFlaNum[i]*HS[5]/12;
	  //  if(HS[5]>=7)//测试出有案例间隔为1；有案例大于7的时候，预测14天
		//pTotalFlaNum[i]=pTotalFlaNum[i]*1.5;
	   //  pTotalFlaNum[i]=pTotalFlaNum[i]*(HS[4]+HS[5])/13;
		//if(HS[5]>=12)
		//	pTotalFlaNum[i]= pTotalFlaNum[i]*2;
		//pTotalFlaNum[i]= pTotalFlaNum[i]*(1+proportion);
	}
	

	
    //输出调试
	/**************以上预测主代码**************************/
	vector<VM> vFla;
	VM vm;
	for(int i=0;i<fla_num;i++)
	{	
		vm.fla=p_fla[i]->fla;
		vm.cpu_num=p_fla[i]->cpu_num;
		vm.mem_num=p_fla[i]->mem_num/1024;
		vm.pri_num=pTotalFlaNum[i];
		vFla.push_back(vm);
	}	

	vector<PHYSER> serSet;
	for(int i=0;i<HS[0];i++)
	{
		serSet.push_back(phy[i]);
	}
	for(auto &a:serSet)
	{	
		a.pro_num = a.cpu_num / (double)a.mem_num;
	}
	for (auto &a : vFla)
		a.pro_num = a.cpu_num / (double)a.mem_num;
	
	vector<vector<vector<int>>> best_all_SerFlaNum(serSet.size());
	vector<int> best_priNum(vFla.size());
	
	//六种放置place1-6;place_compare取六种最高利用率放置
	place_compare(vFla, serSet, best_all_SerFlaNum);
	change_pri(best_all_SerFlaNum, vFla, serSet);
	calcPri(best_all_SerFlaNum, best_priNum);
	/*----------------------输出部分------------------*/
	
	
	int SumVM=0;   //预测虚拟机总数
	char ppp[1000000]={'\0'};
	char p2[1000]={'\0'};
	for(int i =0;i<fla_num;i++)
	{
		SumVM += best_priNum[i];
	}
	sprintf(p2,"%d\n",SumVM);
	strcat(ppp,p2);
	for(int i =0;i<fla_num;i++)
	{
		for(int j = 0;j<1000;j++)
				p2[j] = '\0';
		sprintf(p2,"flavor%d %d\n",vFla[i].fla, best_priNum[i]);
		strcat(ppp,p2);
	}
	strcat(ppp,"\n");
	
	for(int serNum=0;serNum<(int)serSet.size();serNum++)
	{
		sprintf(p2,"%s",serSet[serNum].sort);
		strcat(ppp,p2);
		for(int i = 0;i<20;i++)
			p2[i] = '\0';
		sprintf(p2," %d\n",(int)best_all_SerFlaNum[serNum].size());
		strcat(ppp,p2);
		for (int a = 0; a < (int)best_all_SerFlaNum[serNum].size(); a++)
		{
			for(int i = 0;i<30;i++)
				p2[i] = '\0';
			sprintf(p2,"%s",serSet[serNum].sort);
			strcat(ppp,p2);
			for(int i = 0;i<20;i++)
				p2[i] = '\0';
			sprintf(p2,"-%d ",a + 1);
				strcat(ppp,p2);
		
			for (int b = 0; b < fla_num; b++)
			{
				if(best_all_SerFlaNum[serNum][a][b]==0)
					continue;
				for(int i = 0;i<20;i++)
					p2[i] = '\0';
				sprintf(p2,"flavor%d %d ", vFla[b].fla, best_all_SerFlaNum[serNum][a][b]);
				strcat(ppp,p2);

			}
			for(int i = 0;i<1000;i++)
					p2[i] = '\0';
			strcat(ppp,"\n");
		}
		if(serNum!=(int)serSet.size()-1)
			strcat(ppp,"\n");
	}

	
	
/***********释放结构体指针申请的堆内存，包括二级指针和一级指针*************/
	
	delete []pVMName;
	pVMName = NULL;
	delete []pTotalFlaNum;
   	 pTotalFlaNum = NULL;

/*********************************以上是算法部分****************************/

/****************************打印字符串到输出文件***************************/
	char * result_file = ppp;//ppp;
	write_result(result_file, filename);
}
