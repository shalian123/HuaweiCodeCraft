#ifndef PLACE_H
#define PLACE_H
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <math.h>
#include "data_pr.h"
#include <algorithm>
using namespace std;

double calcFlaPro(vector<VM>Fla);  //计算CPU和MEM总量
vector<int> sortVM(vector<VM>Fla, double ser_pro);   //返回选择几号虚拟机
int sortSer(vector<PHYSER>ser, double fla_pro);  //返回选择几号服务器
void calcPri(vector<vector<vector<int>> > all_SerFlaNum, vector<int> &priVM);
double calcSerPro(PHYSER ser, vector<int> pFlaNum, vector<VM>Fla);

bool end_cy(vector<VM> Fla, vector<PHYSER>serSet, vector<vector<vector<int>>> &all_SerFlaNum); //判断是否有虚拟机剩余
bool calcUt(vector<VM> Fla, vector<PHYSER>serSet, vector<vector<vector<int>>> all_SerFlaNum, vector<int>lastFla, int ser_se); //判断是否应该减掉最后一台服务器

double calc_per_ut(vector<VM> Fla, PHYSER ser, vector<int> pFlaNum); //计算一台服务器的利用率

vector<int> per_ser(vector<int> pFlaNum, vector<VM>&new_Fla, PHYSER ser);  //优化一台服务器中的放置

int sort_serby_ut(vector<double> ser_ut);
int place_ser(vector<VM>&new_Fla, vector<PHYSER>ser, vector<int> &pFlaNum);


bool haveRem(vector<VM>Fla, vector<int> pFlaNum);
double calc_fin_ut(vector<int>priNum, vector<vector<vector<int>>> all_SerFlaNum, vector<VM>Fla, vector<PHYSER>ser);




vector<int> sortVM_new(vector<VM>Fla, double ser_pro, bool aaa);  //改变其排序方式



vector<int> per_ser_place5(vector<int> pFlaNum, vector<VM>&Fla, PHYSER ser);
int place5_ser(vector<VM>&new_Fla, vector<PHYSER>ser, vector<int> &pFlaNum);

void Fill(vector<VM> Fla, vector<PHYSER>serSet, vector<vector<vector<int>>> all_SerFlaNum, vector<int>&lastFla, int ser_se);
bool calcUt_add(vector<VM> Fla, vector<PHYSER>serSet, vector<vector<vector<int>>> all_SerFlaNum, vector<int>lastFla, int ser_se, int min_ser);
void change_pri(vector<vector<vector<int>>> &all_SerFlaNum, vector<VM>Fla, vector<PHYSER>ser);

//place6
vector<int> place_three(vector<VM>Fla, PHYSER &ser);
int find_bestser(vector<VM>&Fla, vector<PHYSER>ser, vector<int> &pFlaNum);

vector<VM> se_to_fla(vector<VM> Fla, vector<int> three_Fla_se); //将序号转化为虚拟机
int sort_serby_ut(vector<double> ser_ut, vector<vector<int>> ser_pFla);
double calc_ut(vector<int>Flanum, PHYSER ser, vector<VM>Fla);
vector<int> sort_fla(vector<int>Fla); //虚拟机按照规格号排序
vector<int> find_three_VM(vector<int>&fla_ch);  //服务器按照虚拟机种类和利用率排序

void push_last_ser(vector<VM> Fla, vector<double> ut, vector<vector<vector<int>>> &all_SerFlaNum);



vector<int> place1(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum);
vector<int> place2(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum);
vector<int> place3(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum);
vector<int> place4(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum);
vector<int> place5(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum);
vector<int> place6(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum);
void place_compare(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum);
void place_compare4(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum);
#endif
