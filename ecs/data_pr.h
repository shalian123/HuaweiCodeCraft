#ifndef __DATAPR_H__
#define __DATAPR_H__

#include "lib_io.h"

typedef struct VM
{
	int fla;
	int cpu_num;
	int mem_num;
	int pri_num;
	double pro_num;
}VM;

typedef struct PHYSER
{
	char sort[20];
	int cpu_num;
	int mem_num;
	int hd_num;
	double pro_num;
}PHYSER;

extern VM** p_fla;
extern PHYSER phy[3];
extern int fla_num;

void Input_Data(char* info[MAX_INFO_NUM],int HS[5]);
void Extract_Data(char* data[MAX_INFO_NUM],int TD[30][1000],int &);

#endif
