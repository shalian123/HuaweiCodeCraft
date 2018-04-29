#pragma GCC diagnostic error "-std=c++11"
#include "place.h"

double calcFlaPro(vector<VM>Fla)
{
	double pro = 0.0;
	int totalC = 0;
	int totalM = 0;

	for (int i = 0; i < (int)Fla.size(); i++)
	{
		totalC += Fla[i].cpu_num*Fla[i].pri_num;
		totalM += Fla[i].mem_num*Fla[i].pri_num;
	}
	pro = totalC / (double)totalM;
	return pro;
}
vector<int> sortVM(vector<VM>Fla, double ser_pro)
{
	int flaNum = Fla.size();

	vector<int> proSe;

	for (int i = 0; i < flaNum; i++)
		proSe.push_back(i);
	for (unsigned int i = 0; i < Fla.size(); i++)
	{
		for (unsigned int j = 0; j < Fla.size() - 1 - i; j++)
		{
			if (fabs(Fla[j].pro_num - ser_pro) > fabs(Fla[j + 1].pro_num - ser_pro))
			{
				swap(Fla[j], Fla[j + 1]);
				swap(proSe[j], proSe[j + 1]);
			}
			if (fabs(Fla[j].pro_num - ser_pro) == fabs(Fla[j + 1].pro_num - ser_pro))
				if (Fla[j].fla < Fla[j + 1].fla)
				{
				swap(Fla[j], Fla[j + 1]);
				swap(proSe[j], proSe[j + 1]);
				}
		}
	}
	return proSe;
}
int sortSer(vector<PHYSER>ser, double fla_pro)
{
	int minSer = 0;
	double minPro = 0.0;
	minPro = fabs(ser[0].pro_num - fla_pro);
	for (unsigned int i = 1; i < ser.size(); i++)
	{
		if (fabs(ser[i].pro_num - fla_pro) < minPro)
		{
			minPro = fabs(ser[i].pro_num - fla_pro);
			minSer = i;
		}
	}
	return minSer;
}
bool end_cy(vector<VM> Fla, vector<PHYSER>serSet, vector<vector<vector<int>>> &all_SerFlaNum)
{
	int totalCpu = 0;
	int totalMem = 0;
	for (auto a : Fla)
	{
		totalCpu += a.cpu_num*a.pri_num;
		totalMem += a.mem_num*a.pri_num;
	}
	vector<double> ut;
	for (int i = 0; i < (int)serSet.size(); i++)
	{
		double ut_c = totalCpu / (double)serSet[i].cpu_num;
		double ut_m = totalMem / (double)serSet[i].mem_num;
		double ut_c_m;
		if (ut_c>1 || ut_m > 1)
			ut_c_m = 100;
		else
		{
			ut_c_m = ut_c + ut_m;
		}
		ut.push_back(ut_c_m);
	}
	int num_less = 0;
	for (auto a : ut)
		if (a <= 2)
			num_less++;
	if (num_less > 0)
	{
		push_last_ser(Fla, ut, all_SerFlaNum); 
		return true;
	}
	return false;
	/*for (int i = 0; i < (int)serSet.size(); i++)
	{
		if (totalCpu == 0 && totalMem == 0)
			return true;
		if (totalCpu <= serSet[i].cpu_num&&totalMem <= serSet[i].mem_num)
		{
			vector<int> lastFla;
			for (int j = 0; j < (int)Fla.size(); j++)
				lastFla.push_back(Fla[j].pri_num);
		
			all_SerFlaNum[i].push_back(lastFla);

			return true;
		}
	}
	return false;*/
}

void push_last_ser(vector<VM> Fla,vector<double> ut, vector<vector<vector<int>>> &all_SerFlaNum)
{
	vector<int> ser_ut;
	vector<double> pre_ut = ut;
	for (int i = 0; i < (int)ut.size(); i++)
		ser_ut.push_back(i);
	for (int i = 0; i < (int)ut.size(); i++)
	{
		for (unsigned int j = 0; j < ut.size() - 1 - i; j++)
		{
			if (ut[j] < ut[j + 1])
			{
				swap(ut[j], ut[j + 1]);
				swap(ser_ut[j], ser_ut[j + 1]);
			}
		}
	}

	for (int i = 0; i < (int)ut.size(); i++)
	{

		if (pre_ut[ser_ut[i]] > 2)
			continue;
		vector<int> lastFla;

		for (int j = 0; j < (int)Fla.size(); j++)
			lastFla.push_back(Fla[j].pri_num);
		all_SerFlaNum[ser_ut[i]].push_back(lastFla);

		break;
	}
}

bool calcUt(vector<VM> Fla, vector<PHYSER>serSet, vector<vector<vector<int>>> all_SerFlaNum, vector<int>lastFla, int ser_se)
{
	int fla_num = Fla.size();
	vector<int> priVM(fla_num);
	calcPri(all_SerFlaNum, priVM);
	double totalFin = 0.0;
	double totalPri = 0.0;
	double totalNum = 0.0;
	int totalSerNum = 0;
	for (auto a : all_SerFlaNum)
		totalSerNum += a.size();
	totalSerNum++;  //加最后一台
	for (int i = 0; i < fla_num; i++)
	{
		totalFin += lastFla[i] * lastFla[i];
		totalPri += (lastFla[i] + priVM[i]) * (lastFla[i] + priVM[i]);
		totalNum += priVM[i] * priVM[i];
	}
	totalFin /= (double)fla_num;
	totalPri /= (double)fla_num;
	totalNum /= (double)fla_num;

	int totalC = 0;
	int totalM = 0;
	int totalC_new = 0;
	int totalM_new = 0;
	for (int i = 0; i < (int)Fla.size(); i++)
	{
		totalC += Fla[i].cpu_num*(lastFla[i] + priVM[i]);
		totalM += Fla[i].mem_num*(lastFla[i] + priVM[i]);
		totalC_new += Fla[i].cpu_num*priVM[i];
		totalM_new += Fla[i].mem_num*priVM[i];
	}
	vector<int> Ser_Num;
	for (int i = 0; i < (int)all_SerFlaNum.size(); i++)
		Ser_Num.push_back(all_SerFlaNum[i].size());
	Ser_Num[ser_se]++;
	int total_ser_C = 0;
	int total_ser_M = 0;
	int total_ser_C_new = 0;
	int total_ser_M_new = 0;
	for (int i = 0; i < (int)all_SerFlaNum.size(); i++)
	{
		total_ser_C += serSet[i].cpu_num*Ser_Num[i];
		total_ser_M += serSet[i].mem_num*Ser_Num[i];
		total_ser_C_new += serSet[i].cpu_num*(int)all_SerFlaNum[i].size();
		total_ser_M_new += serSet[i].mem_num*(int)all_SerFlaNum[i].size();
	}

	double pri_ut = sqrt(totalFin) / (sqrt(totalPri) + sqrt(totalNum));
	double ser_ut = totalC / (double)total_ser_C + totalM / (double)total_ser_M;
	double ser_ut_new = totalC_new / (double)total_ser_C_new + totalM_new / (double)total_ser_M_new;

	double ut_change = ser_ut_new - ser_ut;
	ut_change /= 2;
	if (pri_ut < ut_change)
		return false;
	return true;
}

double calcSerPro(PHYSER ser, vector<int> pFlaNum, vector<VM>Fla)
{
	double pro = 0.0;
	int sumC = 0;
	int sumM = 0;
	for (int i = 0; i < (int)pFlaNum.size(); i++)
	{
		sumC += pFlaNum[i] * Fla[i].cpu_num;
		sumM += pFlaNum[i] * Fla[i].mem_num;
	}
	sumC = ser.cpu_num - sumC;
	sumM = ser.mem_num - sumM;
	pro = sumC / (double)sumM;
	return pro;
}

vector<int> place2(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum)
{
	vector<VM> new_Fla = Fla;
	int fla_num = Fla.size();
	//vector<vector<int> >perSerSet;  //一种服务器的放置情况
	vector<int> pFlaNum(fla_num); //一台服务器放置情况

	vector<int> proSe;
	int TotalCpuNum = 0;//服务器中CPU数量
	int TotalMemNum = 0; //服务器中内存占用数
	double SerPro = 0.0;
	int SerSe = 0;

	for (;;)
	{
		if (end_cy(new_Fla, ser, all_SerFlaNum))
			break;
		SerSe = sortSer(ser, calcFlaPro(new_Fla));
		int j;
		for (;;)
		{
			j = 0;
			SerPro = calcSerPro(ser[SerSe], pFlaNum, Fla);
			proSe = sortVM(new_Fla, SerPro);
			for (; j < fla_num; j++)
			{
				if (new_Fla[proSe[j]].pri_num == 0)
					continue;
				pFlaNum[proSe[j]]++;
				new_Fla[proSe[j]].pri_num--;
				TotalCpuNum += new_Fla[proSe[j]].cpu_num;
				TotalMemNum += new_Fla[proSe[j]].mem_num;

				if (TotalCpuNum > ser[SerSe].cpu_num || TotalMemNum > ser[SerSe].mem_num)
				{
					pFlaNum[proSe[j]]--;
					new_Fla[proSe[j]].pri_num++;
					TotalCpuNum -= new_Fla[proSe[j]].cpu_num;
					TotalMemNum -= new_Fla[proSe[j]].mem_num;
					continue;
				}
				break;
			}
			if (j == fla_num)
				break;
		}
		pFlaNum = per_ser(pFlaNum, new_Fla, ser[SerSe]);
		all_SerFlaNum[SerSe].push_back(pFlaNum);
		TotalCpuNum = 0;
		TotalMemNum = 0;
		for (auto &a : pFlaNum)
			a = 0;
	}
	vector<int> priNum(fla_num);
	calcPri(all_SerFlaNum, priNum);
	return priNum;
}
void calcPri(vector<vector<vector<int>> > all_SerFlaNum, vector<int> &priVM)
{
	for (auto &a : priVM)
		a = 0;
	for (int i = 0; i < (int)all_SerFlaNum.size(); i++)
		for (int j = 0; j < (int)priVM.size(); j++)
			for (int k = 0; k <(int)all_SerFlaNum[i].size(); k++)
				priVM[j] += all_SerFlaNum[i][k][j];
}

double calc_per_ut(vector<VM> Fla, PHYSER ser, vector<int> pFlaNum)
{
	int totalC = 0;
	int totalM = 0;
	for (int i = 0; i < (int)Fla.size(); i++)
	{
		totalC += Fla[i].cpu_num * pFlaNum[i];
		totalM += Fla[i].mem_num * pFlaNum[i];
	}
	double ut = totalC / (double)ser.cpu_num + totalM / (double)ser.mem_num;
	ut /= 2;
	if (totalC>ser.cpu_num || totalM > ser.mem_num)
		ut = 0;
	return ut;
}


vector<int> per_ser(vector<int> pFlaNum, vector<VM>&new_Fla, PHYSER ser)
{
	vector<int> new_pFlaNum = pFlaNum;
	double ut, new_ut;
	ut = calc_per_ut(new_Fla, ser, pFlaNum);
	for (int i = 0; i < (int)new_Fla.size(); i++)
	{
		for (int j = 0; j < (int)new_Fla.size(); j++)
		{
			if (i == j)
				continue;
			if (new_Fla[i].pri_num != 0 && new_pFlaNum[j] != 0)
			{
				new_pFlaNum[i]++;
				new_pFlaNum[j]--;
				new_ut = calc_per_ut(new_Fla, ser, new_pFlaNum);
				if (new_ut > ut)
				{
					j--;
					ut = new_ut;
					continue;
				}
				new_pFlaNum[i]--;
				new_pFlaNum[j]++;
			}

		}
	}
	return new_pFlaNum;
}
int sort_serby_ut(vector<double> ser_ut)
{
	int max_ser = 0;
	double max_ut = ser_ut[0];

	for (unsigned int i = 1; i < ser_ut.size(); i++)
	{
		if (ser_ut[i]> max_ut)
		{
			max_ut = ser_ut[i];
			max_ser = i;
		}
	}
	return max_ser;
}
int place_ser(vector<VM>&new_Fla, vector<PHYSER>ser, vector<int> &pFlaNum)
{
	int fla_num = new_Fla.size();
	vector<vector<int>> ser_pFla;
	vector<double> ser_ut;
	double ut;
	vector<int> proSe;
	int TotalCpuNum = 0;//服务器中CPU数量
	int TotalMemNum = 0; //服务器中内存占用数
	double SerPro = 0.0;
	vector<VM> pre_Fla = new_Fla;
	for (int i = 0; i < (int)ser.size(); i++)
	{
		int j;
		for (;;)
		{
			j = 0;
			SerPro = calcSerPro(ser[i], pFlaNum, new_Fla);
			proSe = sortVM(new_Fla, SerPro);
			for (; j < fla_num; j++)
			{
				if (new_Fla[proSe[j]].pri_num == 0)
					continue;
				pFlaNum[proSe[j]]++;
				new_Fla[proSe[j]].pri_num--;
				TotalCpuNum += new_Fla[proSe[j]].cpu_num;
				TotalMemNum += new_Fla[proSe[j]].mem_num;

				if (TotalCpuNum > ser[i].cpu_num || TotalMemNum > ser[i].mem_num)
				{
					pFlaNum[proSe[j]]--;
					new_Fla[proSe[j]].pri_num++;
					TotalCpuNum -= new_Fla[proSe[j]].cpu_num;
					TotalMemNum -= new_Fla[proSe[j]].mem_num;
					continue;
				}
				break;
			}
			if (j == fla_num)
				break;
		}
		pFlaNum = per_ser(pFlaNum, new_Fla, ser[i]);
	
		new_Fla = pre_Fla;

		ut = calc_per_ut(new_Fla, ser[i], pFlaNum);
		ser_ut.push_back(ut);
		ser_pFla.push_back(pFlaNum);
		TotalCpuNum = 0;
		TotalMemNum = 0;
		for (auto &a : pFlaNum)
			a = 0;
	}
	int choose_se = sort_serby_ut(ser_ut);

	pFlaNum = ser_pFla[choose_se];
	for (int i = 0; i < fla_num; i++)
		new_Fla[i].pri_num -= pFlaNum[i];
	return choose_se;
}

vector<int> place4(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum)
{
	vector<VM> new_Fla = Fla;
	int fla_num = Fla.size();
	vector<int> pFlaNum(fla_num); //一台服务器放置情况
	int SerSe = 0;
	int i = 0;
	vector<int> pFlaBest;
	for (;;)
	{
		if (end_cy(new_Fla, ser, all_SerFlaNum))
			break;
		if (i == 0)
		{
			for (auto &a : pFlaNum)
				a = 0;
			SerSe = place_ser(new_Fla, ser, pFlaNum);
			all_SerFlaNum[SerSe].push_back(pFlaNum);
			pFlaBest = pFlaNum;
		}
		else
		{
			bool isRem = haveRem(new_Fla, pFlaBest);
			if (isRem)
			{
				for (int j = 0; j < (int)new_Fla.size(); j++)
					new_Fla[j].pri_num -= pFlaBest[j];
			}
			else
			{
				for (auto &a : pFlaNum)
					a = 0;
				SerSe = place_ser(new_Fla, ser, pFlaNum);
				pFlaBest = pFlaNum;
			}
			all_SerFlaNum[SerSe].push_back(pFlaBest);

		}

		i++;
	}
	vector<int> priNum(fla_num);
	calcPri(all_SerFlaNum, priNum);
	return priNum;
}

bool haveRem(vector<VM>Fla, vector<int> pFlaNum)
{
	int num = 0;

	for (int i = 0; i<(int)Fla.size(); i++)
		if (Fla[i].pri_num - pFlaNum[i] < 0)
			num++;
	if (num > 0)
		return false;
	return true;
}

double calc_fin_ut(vector<int>priNum, vector<vector<vector<int>>> all_SerFlaNum, vector<VM>Fla, vector<PHYSER>ser)
{
	int sumC = 0;
	int sumM = 0;
	int totC = 0;
	int totM = 0;

	for (int i = 0; i < (int)Fla.size(); i++)
	{
		sumC += Fla[i].cpu_num*priNum[i];
		sumM += Fla[i].mem_num*priNum[i];
	}
	for (int i = 0; i < (int)all_SerFlaNum.size(); i++)
	{
		totC += all_SerFlaNum[i].size()*ser[i].cpu_num;
		totM += all_SerFlaNum[i].size()*ser[i].mem_num;
	}
	double ut = 0.0;
	ut = sumC / (double)totC + sumM / (double)totM;
	ut /= 2;
	//cout << ut << endl;
	return ut;
}

vector<int> place1(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum)
{
	vector<VM> new_Fla = Fla;
	int fla_num = Fla.size();
	//vector<vector<int> >perSerSet;  //一种服务器的放置情况
	vector<int> pFlaNum(fla_num); //一台服务器放置情况
	int SerSe = 0;
	vector<int> proSe;
	int TotalCpuNum = 0;//服务器中CPU数量
	int TotalMemNum = 0; //服务器中内存占用数
	double SerPro = 0.0;

	for (;;)
	{
		if (end_cy(new_Fla, ser, all_SerFlaNum))
			break;
		SerSe = sortSer(ser, calcFlaPro(new_Fla));
		int j;
		for (;;)
		{
			j = 0;
			SerPro = calcSerPro(ser[SerSe], pFlaNum, Fla);
			proSe = sortVM(new_Fla, SerPro);
			for (; j < fla_num; j++)
			{
				if (new_Fla[proSe[j]].pri_num == 0)
					continue;
				pFlaNum[proSe[j]]++;
				new_Fla[proSe[j]].pri_num--;
				TotalCpuNum += new_Fla[proSe[j]].cpu_num;
				TotalMemNum += new_Fla[proSe[j]].mem_num;

				if (TotalCpuNum > ser[SerSe].cpu_num || TotalMemNum > ser[SerSe].mem_num)
				{
					pFlaNum[proSe[j]]--;
					new_Fla[proSe[j]].pri_num++;
					TotalCpuNum -= new_Fla[proSe[j]].cpu_num;
					TotalMemNum -= new_Fla[proSe[j]].mem_num;
					continue;
				}
				break;
			}
			if (j == fla_num)
				break;
		}
		all_SerFlaNum[SerSe].push_back(pFlaNum);
		TotalCpuNum = 0;
		TotalMemNum = 0;
		for (auto &a : pFlaNum)
			a = 0;
	}
	vector<int> priNum(fla_num);
	calcPri(all_SerFlaNum, priNum);
	return priNum;
}

vector<int> place3(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum)
{
	vector<VM> new_Fla = Fla;
	int fla_num = Fla.size();
	
	int SerSe = 0;
	for (;;)
	{
		
		if (end_cy(new_Fla, ser, all_SerFlaNum))
			break;
		vector<int> pFlaNum(fla_num); //一台服务器放置情况
		SerSe = place_ser(new_Fla, ser, pFlaNum);
		all_SerFlaNum[SerSe].push_back(pFlaNum);

	}
	vector<int> priNum(fla_num);
	calcPri(all_SerFlaNum, priNum);
	return priNum;
}



vector<int> place5(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum)
{
	vector<VM> new_Fla = Fla;
	int fla_num = Fla.size();
	vector<int> pFlaNum(fla_num); //一台服务器放置情况

	int SerSe = 0;
	int i = 0;
	for (;;)
	{
		if (end_cy(new_Fla, ser, all_SerFlaNum))
			break;
		if (i == 0)
		{
			SerSe = place5_ser(new_Fla, ser, pFlaNum);
			all_SerFlaNum[SerSe].push_back(pFlaNum);
		}
		else
		{
			bool isRem = haveRem(new_Fla, pFlaNum);
			if (isRem)
			{
				for (int j = 0; j < (int)new_Fla.size(); j++)
					new_Fla[j].pri_num -= pFlaNum[j];
			}
			else
			{
				SerSe = place5_ser(new_Fla, ser, pFlaNum);
			}
			all_SerFlaNum[SerSe].push_back(pFlaNum);
		}
		i++;
	}
	vector<int> priNum(fla_num);
	calcPri(all_SerFlaNum, priNum);
	return priNum;
}
vector<int> per_ser_place5(vector<int> pFlaNum, vector<VM>&Fla, PHYSER ser)
{
	double SerPro = 0.0;
	double ut, new_ut;
	ut = calc_per_ut(Fla, ser, pFlaNum);
	//cout << ut << endl;
	if (ut == 1)
		return pFlaNum;
	vector<int> new_pFlaNum = pFlaNum;
	vector<int> old_pFlaNum = new_pFlaNum;
	vector<VM> new_Fla = Fla;
	vector<int> proSe;


	for (int i = 0; i < (int)new_Fla.size(); i++)
	{
		if (new_pFlaNum[i] != 0)
		{
			new_pFlaNum = old_pFlaNum;
			new_Fla = Fla;
			new_Fla[i].pri_num++;
			new_pFlaNum[i]--;
			/*for (auto a : new_pFlaNum)
			cout << a << " ";
			cout << endl;*/

			int k;
			int TotalCpuNum = 0;//服务器中CPU数量
			int TotalMemNum = 0; //服务器中内存占用数
			for (int j = 0; j < (int)new_Fla.size(); j++)
			{
				TotalCpuNum += new_Fla[j].cpu_num*new_pFlaNum[j];
				TotalMemNum += new_Fla[j].mem_num*new_pFlaNum[j];
			}
			//cout << TotalCpuNum << " " << TotalMemNum << endl;
			for (;;)
			{
				k = 0;
				SerPro = calcSerPro(ser, new_pFlaNum, new_Fla);

				proSe = sortVM_new(new_Fla, SerPro, 0);
				for (; k < (int)new_Fla.size(); k++)
				{
					if (new_Fla[proSe[k]].pri_num == 0)
						continue;
					new_pFlaNum[proSe[k]]++;
					new_Fla[proSe[k]].pri_num--;
					TotalCpuNum += new_Fla[proSe[k]].cpu_num;
					TotalMemNum += new_Fla[proSe[k]].mem_num;

					if (TotalCpuNum > ser.cpu_num || TotalMemNum > ser.mem_num)
					{
						new_pFlaNum[proSe[k]]--;
						new_Fla[proSe[k]].pri_num++;
						TotalCpuNum -= new_Fla[proSe[k]].cpu_num;
						TotalMemNum -= new_Fla[proSe[k]].mem_num;
						continue;
					}
					break;
				}
				if (k == (int)new_Fla.size())
					break;
			}

			new_ut = calc_per_ut(new_Fla, ser, new_pFlaNum);

			if (new_ut > ut)
			{
				i--;
				ut = new_ut;
				old_pFlaNum = new_pFlaNum;
				Fla = new_Fla;
				continue;
			}
		}
	}
	for (int i = 0; i < (int)new_Fla.size(); i++)
	{
		if (new_pFlaNum[i] != 0)
		{
			new_pFlaNum = old_pFlaNum;
			new_Fla = Fla;
			new_Fla[i].pri_num++;
			new_pFlaNum[i]--;

			int k;
			int TotalCpuNum = 0;//服务器中CPU数量
			int TotalMemNum = 0; //服务器中内存占用数
			for (int j = 0; j < (int)new_Fla.size(); j++)
			{
				TotalCpuNum += new_Fla[j].cpu_num*new_pFlaNum[j];
				TotalMemNum += new_Fla[j].mem_num*new_pFlaNum[j];
			}

			for (;;)
			{
				k = 0;
				SerPro = calcSerPro(ser, new_pFlaNum, new_Fla);

				proSe = sortVM_new(new_Fla, SerPro, 1);
				for (; k < (int)new_Fla.size(); k++)
				{
					if (new_Fla[proSe[k]].pri_num == 0)
						continue;
					new_pFlaNum[proSe[k]]++;
					new_Fla[proSe[k]].pri_num--;
					TotalCpuNum += new_Fla[proSe[k]].cpu_num;
					TotalMemNum += new_Fla[proSe[k]].mem_num;

					if (TotalCpuNum > ser.cpu_num || TotalMemNum > ser.mem_num)
					{
						new_pFlaNum[proSe[k]]--;
						new_Fla[proSe[k]].pri_num++;
						TotalCpuNum -= new_Fla[proSe[k]].cpu_num;
						TotalMemNum -= new_Fla[proSe[k]].mem_num;
						continue;
					}
					break;
				}
				if (k == (int)new_Fla.size())
					break;
			}

			new_ut = calc_per_ut(new_Fla, ser, new_pFlaNum);

			if (new_ut > ut)
			{
				i--;
				ut = new_ut;
				old_pFlaNum = new_pFlaNum;
				Fla = new_Fla;
				continue;
			}
		}
	}
	return new_pFlaNum;
}
int place5_ser(vector<VM>&new_Fla, vector<PHYSER>ser, vector<int> &pFlaNum)
{

	int fla_num = new_Fla.size();
	vector<vector<int>> ser_pFla;
	vector<double> ser_ut;
	double ut;
	vector<int> proSe;
	int TotalCpuNum = 0;//服务器中CPU数量
	int TotalMemNum = 0; //服务器中内存占用数
	double SerPro = 0.0;
	vector<VM> pre_Fla = new_Fla;

	for (int i = 0; i < (int)ser.size(); i++)
	{
		for (auto &a : pFlaNum)
			a = 0;
		new_Fla = pre_Fla;
		int j;
		for (;;)
		{
			j = 0;
			SerPro = calcSerPro(ser[i], pFlaNum, new_Fla);
			proSe = sortVM(new_Fla, SerPro);
	
			for (; j < fla_num; j++)
			{
				//cout << j << endl;
				if (new_Fla[proSe[j]].pri_num == 0)
					continue;
				pFlaNum[proSe[j]]++;
				new_Fla[proSe[j]].pri_num--;
				TotalCpuNum += new_Fla[proSe[j]].cpu_num;
				TotalMemNum += new_Fla[proSe[j]].mem_num;

				if (TotalCpuNum > ser[i].cpu_num || TotalMemNum > ser[i].mem_num)
				{
					pFlaNum[proSe[j]]--;
					new_Fla[proSe[j]].pri_num++;
					TotalCpuNum -= new_Fla[proSe[j]].cpu_num;
					TotalMemNum -= new_Fla[proSe[j]].mem_num;
					continue;
				}
				break;
			}
			if (j == fla_num)
				break;
		}

		pFlaNum = per_ser_place5(pFlaNum, new_Fla, ser[i]);


		ut = calc_per_ut(new_Fla, ser[i], pFlaNum);

		ser_ut.push_back(ut);
		ser_pFla.push_back(pFlaNum);
		TotalCpuNum = 0;
		TotalMemNum = 0;

	}
	int choose_se = sort_serby_ut(ser_ut);

	pFlaNum = ser_pFla[choose_se];
	for (int i = 0; i < fla_num; i++)
		new_Fla[i].pri_num = pre_Fla[i].pri_num - pFlaNum[i];


	return choose_se;
}

vector<int> sortVM_new(vector<VM>Fla, double ser_pro, bool aaa)
{
	if (ser_pro >= 0.75)
	{
		if (aaa)
			ser_pro = 0.5;
		else
		{
			ser_pro = 0.25;
		}
	}
	if (ser_pro < 0.75&&ser_pro >= 0.375)
	{
		if (aaa)
			ser_pro = 0.25;
		else
		{
			ser_pro = 0.75;
		}
	}
	if (ser_pro < 0.375)
	{
		if (aaa)
			ser_pro = 0.5;
		else
		{
			ser_pro = 0.75;
		}
	}

	int flaNum = Fla.size();
	vector<int> proSe;

	for (int i = 0; i < flaNum; i++)
		proSe.push_back(i);
	for (unsigned int i = 0; i < Fla.size(); i++)
	{
		for (unsigned int j = 0; j < Fla.size() - 1 - i; j++)
		{
			if (fabs(Fla[j].pro_num - ser_pro) >= fabs(Fla[j + 1].pro_num - ser_pro))
			{
				swap(Fla[j], Fla[j + 1]);
				swap(proSe[j], proSe[j + 1]);
			}
			if (fabs(Fla[j].pro_num - ser_pro) == fabs(Fla[j + 1].pro_num - ser_pro))
				if (Fla[j].fla < Fla[j + 1].fla)
				{
				swap(Fla[j], Fla[j + 1]);
				swap(proSe[j], proSe[j + 1]);
				}
		}
	}
	return proSe;
}


vector<int> place_three(vector<VM>Fla, PHYSER &ser)
{
	int flanum = Fla.size();
	vector<int> max_num;
	int count = 0;

	for (int i = 0; i < flanum; i++)
	{
		count = min(ser.cpu_num / Fla[i].cpu_num, ser.mem_num / Fla[i].mem_num);
		count = min(count, Fla[i].pri_num);
		max_num.push_back(count);
	}
	vector<int> all_flanum;
	double max_ut = 0;
	double temp_ut = 0.0;
	vector<int> best_flanum;


	switch (flanum)
	{
	case 1:
		best_flanum.push_back(max_num[0]);
		break;
	case 2:
		for (int i = 0; i <= max_num[0]; i++)
		{
			for (int j = 0; j <= max_num[1]; j++)
			{

				all_flanum.push_back(i);
				all_flanum.push_back(j);

				temp_ut = calc_ut(all_flanum, ser, Fla);
				if (temp_ut < 0)
				{
					all_flanum.clear();
					break;
				}
				if (temp_ut >= max_ut)
				{
					max_ut = temp_ut;
					best_flanum = all_flanum;
				}
				all_flanum.clear();
			}
		}
		break;
	case 3:
		for (int i = 0; i <= max_num[0]; i++)
		{
			for (int j = 0; j <= max_num[1]; j++)
			{
				for (int k = 0; k <= max_num[2]; k++)
				{
					all_flanum.push_back(i);
					all_flanum.push_back(j);
					all_flanum.push_back(k);
					temp_ut = calc_ut(all_flanum, ser, Fla);

					if (temp_ut < 0)
					{
						all_flanum.clear();
						break;
					}
					if (temp_ut >= max_ut)
					{
						max_ut = temp_ut;
						best_flanum = all_flanum;
					}
					all_flanum.clear();
				}
			}
		}
		break;
	}
	for (int i = 0; i < (int)best_flanum.size(); i++)
	{
		ser.cpu_num -= best_flanum[i] * Fla[i].cpu_num;
		ser.mem_num -= best_flanum[i] * Fla[i].mem_num;
	}

	return best_flanum;
}

double calc_ut(vector<int>Flanum, PHYSER ser, vector<VM>Fla)
{
	int totalC = 0;
	int totalM = 0;
	for (int i = 0; i < (int)Fla.size(); i++)
	{
		totalC += Fla[i].cpu_num*Flanum[i];
		totalM += Fla[i].mem_num*Flanum[i];
	}
	if (totalC>ser.cpu_num || totalM > ser.mem_num)
		return -1;

	double ut = totalC / (double)ser.cpu_num + totalM / (double)ser.mem_num;
	return ut / 2;
}

vector<int> sort_fla(vector<int>Fla)
{
	vector<int> fla_se;
	for (int i = 0; i < (int)Fla.size(); i++)
		fla_se.push_back(i);
	for (int i = 0; i < (int)Fla.size(); i++)
	{
		for (int j = 0; j <(int)Fla.size() - 1 - i; j++)
		{
			if (Fla[j] < Fla[j + 1])
			{
				swap(Fla[j], Fla[j + 1]);
				swap(fla_se[j], fla_se[j + 1]);
			}
		}
	}
	return fla_se;
}
vector<int> find_three_VM(vector<int>&fla_ch)
{
	//cout << fla_ch.size() << endl;
	vector<int> three_Fla;
	vector<int> fla_se;
	if (fla_ch.size()>1)
		fla_se = sort_fla(fla_ch);
	else
	{
		fla_se.push_back(0);
	}

	for (int i = 0; i < (int)fla_ch.size(); i++)
	{
		if (fla_ch[fla_se[i]] % 3 == 1)
		{
			three_Fla.push_back(fla_ch[fla_se[i]]);
			break;
		}
	}


	for (int i = 0; i < (int)fla_ch.size(); i++)
	{
		if (fla_ch[fla_se[i]] % 3 == 2)
		{
			three_Fla.push_back(fla_ch[fla_se[i]]);
			break;
		}
	}
	for (int i = 0; i < (int)fla_ch.size(); i++)
	{
		if (fla_ch[fla_se[i]] % 3 == 0)
		{
			three_Fla.push_back(fla_ch[fla_se[i]]);
			break;
		}
	}

	for (int i = 0; i < (int)three_Fla.size(); i++)
	{
		auto it = find(fla_ch.begin(), fla_ch.end(), three_Fla[i]);
		fla_ch.erase(it);
	}

	return three_Fla;
}

int find_bestser(vector<VM> &Fla, vector<PHYSER>ser, vector<int> &pFlaNum)
{
	vector<vector<int>> ser_pFla;
	vector<double> ser_ut;
	double ut;
	for (int i = 0; i < (int)ser.size(); i++)
	{
		vector<int> new_Fla_ser;
		for (auto a : Fla)
		{
			if (a.pri_num == 0)
				continue;
			new_Fla_ser.push_back(a.fla);
		}
		vector<int> best_fla_num((int)Fla.size());
		PHYSER newSer = ser[i];
		for (;;)
		{
			if (new_Fla_ser.size() == 0)
			{
				break;
			}
			vector<int> three_Fla_se = find_three_VM(new_Fla_ser);


			vector<VM> three_Fla = se_to_fla(Fla, three_Fla_se);

			vector<int> best_three = place_three(three_Fla, newSer);


			for (int j = 0; j < (int)Fla.size(); j++)
			{
				for (int k = 0; k < (int)three_Fla.size(); k++)
				{
					if (Fla[j].fla == three_Fla[k].fla)
						best_fla_num[j] = best_three[k];
				}
			}
			if (newSer.cpu_num == 0 || newSer.mem_num == 0)
				break;
		}
		ut = calc_per_ut(Fla, ser[i], best_fla_num);
		ser_ut.push_back(ut);
		ser_pFla.push_back(best_fla_num);
	}
	int choose_se = sort_serby_ut(ser_ut, ser_pFla);


	pFlaNum = ser_pFla[choose_se];

	for (int i = 0; i < (int)Fla.size(); i++)
		Fla[i].pri_num -= pFlaNum[i];


	return choose_se;
}

vector<VM> se_to_fla(vector<VM> Fla, vector<int> three_Fla_se)
{
	vector<VM> three_Fla;
	for (auto a : Fla)
	{
		for (int i = 0; i <(int)three_Fla_se.size(); i++)
			if (a.fla == three_Fla_se[i])
				three_Fla.push_back(a);
	}
	return three_Fla;
}
int sort_serby_ut(vector<double> ser_ut, vector<vector<int>> ser_pFla)
{
	int max_ser = 0;
	double max_ut = ser_ut[0];
	vector<int> num_not_zero;
	int num;
	for (auto a : ser_pFla)
	{
		num = 0;
		for (auto c : a)
		{
			if (c != 0)
				num++;
		}
		num_not_zero.push_back(num);
	}

	int sizeNum = num_not_zero[0];

	for (unsigned int i = 1; i < ser_ut.size(); i++)
	{
		if (ser_ut[i]> max_ut)
		{
			sizeNum = ser_pFla[i].size();
			max_ut = ser_ut[i];
			max_ser = i;
		}
		if (ser_ut[i] == max_ut)
		{
			if (num_not_zero[i]> sizeNum)
			{
				sizeNum = num_not_zero[i];
				max_ut = ser_ut[i];
				max_ser = i;
			}
		}
	}

	return max_ser;
}

vector<int> place6(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum)
{
	vector<VM> new_Fla = Fla;
	int fla_num = Fla.size();
	vector<int> pFlaNum; //一台服务器放置情况

	int SerSe = 0;
	int i = 0;
	for (;;)
	{
		if (end_cy(new_Fla, ser, all_SerFlaNum))
			break;
		if (i == 0)
		{
			SerSe = find_bestser(new_Fla, ser, pFlaNum);
			all_SerFlaNum[SerSe].push_back(pFlaNum);
		}
		else
		{
			bool isRem = haveRem(new_Fla, pFlaNum);
			if (isRem)
			{
				for (int j = 0; j < (int)new_Fla.size(); j++)
					new_Fla[j].pri_num -= pFlaNum[j];
			}
			else
			{
				SerSe = find_bestser(new_Fla, ser, pFlaNum);
			}
			all_SerFlaNum[SerSe].push_back(pFlaNum);
		}

		i++;
	}
	vector<int> priNum(fla_num);
	calcPri(all_SerFlaNum, priNum);
	return priNum;
}

void place_compare4(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum)
{
	vector<vector<vector<int>>> all_SerFlaNum1(ser.size());
	vector<int> priNum1 = place1(Fla, ser, all_SerFlaNum1);
	double ut1 = calc_fin_ut(priNum1, all_SerFlaNum1, Fla, ser);

	vector<vector<vector<int>>> all_SerFlaNum2(ser.size());
	vector<int> priNum2 = place2(Fla, ser, all_SerFlaNum2);
	double ut2 = calc_fin_ut(priNum2, all_SerFlaNum2, Fla, ser);

	vector<vector<vector<int>>> all_SerFlaNum3(ser.size());
	vector<int> priNum3 = place3(Fla, ser, all_SerFlaNum3);
	double ut3 = calc_fin_ut(priNum3, all_SerFlaNum3, Fla, ser);


	vector<vector<vector<int>>> all_SerFlaNum4(ser.size());
	vector<int> priNum4 = place4(Fla, ser, all_SerFlaNum4);
	double ut4 = calc_fin_ut(priNum4, all_SerFlaNum4, Fla, ser);

	if (ut4 >= ut3&&ut4 >= ut2&&ut4 >= ut1)
	{
		all_SerFlaNum = all_SerFlaNum4;
	}
	if (ut3 >= ut4&&ut3 >= ut2&&ut3 >= ut1)
	{
		all_SerFlaNum = all_SerFlaNum3;
	}
	if (ut2 >= ut4&&ut2 >= ut3&&ut2 >= ut1)
	{
		all_SerFlaNum = all_SerFlaNum2;
	}
	if (ut1 >= ut4&&ut1 >= ut3&&ut1 >= ut2)
	{
		all_SerFlaNum = all_SerFlaNum1;
	}
}



void place_compare(vector<VM>Fla, vector<PHYSER>ser, vector<vector<vector<int>>> &all_SerFlaNum)
{
	vector<vector<vector<int>>> all_SerFlaNum1(ser.size());
	vector<int> priNum1 = place1(Fla, ser, all_SerFlaNum1);
	double ut1 = calc_fin_ut(priNum1, all_SerFlaNum1, Fla, ser);

	vector<vector<vector<int>>> all_SerFlaNum2(ser.size());
	vector<int> priNum2 = place2(Fla, ser, all_SerFlaNum2);
	double ut2 = calc_fin_ut(priNum2, all_SerFlaNum2, Fla, ser);

	vector<vector<vector<int>>> all_SerFlaNum3(ser.size());
	vector<int> priNum3 = place3(Fla, ser, all_SerFlaNum3);
	double ut3 = calc_fin_ut(priNum3, all_SerFlaNum3, Fla, ser);


	vector<vector<vector<int>>> all_SerFlaNum4(ser.size());
	vector<int> priNum4 = place4(Fla, ser, all_SerFlaNum4);
	double ut4 = calc_fin_ut(priNum4, all_SerFlaNum4, Fla, ser);

	vector<vector<vector<int>>> all_SerFlaNum5(ser.size());
	vector<int> priNum5 = place5(Fla, ser, all_SerFlaNum5);
	double ut5 = calc_fin_ut(priNum5, all_SerFlaNum5, Fla, ser);

	vector<vector<vector<int>>> all_SerFlaNum6(ser.size());
	vector<int> priNum6 = place6(Fla, ser, all_SerFlaNum6);
	double ut6 = calc_fin_ut(priNum6, all_SerFlaNum6, Fla, ser);

	cout << ut1 << endl;
	cout << ut2 << endl;
	cout << ut3 << endl;
	cout << ut4 << endl;
	cout << ut5 << endl;
	cout << ut6 << endl;

	if (ut6 >= ut5&&ut6 >= ut4&&ut6 >= ut3&&ut6 >= ut2&&ut6>=ut1)
	{
		all_SerFlaNum = all_SerFlaNum6;
	}

	if (ut5 >= ut4&&ut5 >= ut3&&ut5 >= ut2&&ut5 >= ut1&&ut5>=ut6)
	{
		all_SerFlaNum = all_SerFlaNum5;
	}
	if (ut4 >= ut3&&ut4 >= ut2&&ut4 >= ut1&&ut4 >= ut5&&ut4 >= ut6)
	{
		all_SerFlaNum = all_SerFlaNum4;
	}
	if (ut3 >= ut4&&ut3 >= ut2&&ut3 >= ut1&&ut3 >= ut5&&ut3 >= ut6)
	{
		all_SerFlaNum = all_SerFlaNum3;
	}
	if (ut2 >= ut4&&ut2 >= ut3&&ut2 >= ut1&&ut2 >= ut5&&ut2 >= ut6)
	{
		all_SerFlaNum = all_SerFlaNum2;
	}
	if (ut1 >= ut4&&ut1 >= ut3&&ut1 >= ut2&&ut1 >= ut5&&ut1 >= ut6)
	{
		all_SerFlaNum = all_SerFlaNum1;
	}
}

void change_pri(vector<vector<vector<int>>> &all_SerFlaNum, vector<VM>Fla, vector<PHYSER>ser)
{
	for (int i = 0; i < (int)ser.size(); i++)
	{
		int a = all_SerFlaNum[i].size();
		if (a == 0)
			continue;
		vector<int> lastSer = all_SerFlaNum[i][a - 1];
		all_SerFlaNum[i].pop_back();
		if (calcUt(Fla, ser, all_SerFlaNum, lastSer, i))
		{
			Fill(Fla, ser, all_SerFlaNum, lastSer, i);
			all_SerFlaNum[i].push_back(lastSer);
		}
	}
}

void Fill(vector<VM> Fla, vector<PHYSER>serSet, vector<vector<vector<int>>> all_SerFlaNum, vector<int>&lastFla, int ser_se)
{
	if (all_SerFlaNum[ser_se].size() == 0)
		return;
	int flanum = (int)lastFla.size();
	vector<int> minus;
	for (int i = 0; i < (int)all_SerFlaNum[ser_se].size(); i++)
	{
		int min_temp = 0;
		for (int j = 0; j < flanum; j++)
			min_temp += abs(all_SerFlaNum[ser_se][i][j] - lastFla[j]);
		minus.push_back(min_temp);
	}
	int min = minus[0];
	int min_ser = 0;
	for (int i = 0; i <(int)minus.size(); i++)
	{
		if (minus[i] < min)
		{
			min = minus[i];
			min_ser = i;
		}
	}
	bool sss = calcUt_add(Fla, serSet, all_SerFlaNum, lastFla, ser_se, min_ser);
	//all_SerFlaNum[ser_se].push_back(all_SerFlaNum[ser_se][min_ser]);
	if (sss)
		lastFla = all_SerFlaNum[ser_se][min_ser];

}
bool calcUt_add(vector<VM> Fla, vector<PHYSER>serSet, vector<vector<vector<int>>> all_SerFlaNum, vector<int>lastFla, int ser_se, int min_ser)
{
	int fla_num = Fla.size();
	all_SerFlaNum[ser_se].push_back(all_SerFlaNum[ser_se][min_ser]);
	vector<int> priVM1(fla_num);
	vector<int> priVM2(fla_num);
	calcPri(all_SerFlaNum, priVM1);
	all_SerFlaNum[ser_se].pop_back();
	all_SerFlaNum[ser_se].push_back(lastFla);
	calcPri(all_SerFlaNum, priVM2);
	double totalFin = 0.0;
	double totalPri = 0.0;
	double totalNum = 0.0;
	int totalSerNum = 0;
	for (auto a : all_SerFlaNum)
		totalSerNum += a.size();

	for (int i = 0; i < fla_num; i++)
	{
		totalFin += (priVM1[i] - priVM2[i]) * (priVM1[i] - priVM2[i]);
		totalPri += priVM1[i] * priVM1[i];
		totalNum += priVM2[i] * priVM2[i];
	}
	totalFin /= (double)fla_num;
	totalPri /= (double)fla_num;
	totalNum /= (double)fla_num;

	int totalC = 0;
	int totalM = 0;
	int totalC_new = 0;
	int totalM_new = 0;
	for (int i = 0; i < (int)Fla.size(); i++)
	{
		totalC += Fla[i].cpu_num* priVM2[i];
		totalM += Fla[i].mem_num* priVM2[i];
		totalC_new += Fla[i].cpu_num*priVM1[i];
		totalM_new += Fla[i].mem_num*priVM1[i];
	}
	vector<int> Ser_Num;
	for (int i = 0; i < (int)all_SerFlaNum.size(); i++)
		Ser_Num.push_back(all_SerFlaNum[i].size());

	int total_ser_C = 0;
	int total_ser_M = 0;
	int total_ser_C_new = 0;
	int total_ser_M_new = 0;
	for (int i = 0; i < (int)all_SerFlaNum.size(); i++)
	{
		total_ser_C += serSet[i].cpu_num*Ser_Num[i];
		total_ser_M += serSet[i].mem_num*Ser_Num[i];
		total_ser_C_new += serSet[i].cpu_num*(int)all_SerFlaNum[i].size();
		total_ser_M_new += serSet[i].mem_num*(int)all_SerFlaNum[i].size();
	}

	double pri_ut = sqrt(totalFin) / (sqrt(totalPri) + sqrt(totalNum));
	double ser_ut = totalC / (double)total_ser_C + totalM / (double)total_ser_M;
	double ser_ut_new = totalC_new / (double)total_ser_C_new + totalM_new / (double)total_ser_M_new;

	double ut_change = ser_ut_new - ser_ut;

	ut_change /= 2;
	//cout << pri_ut << " " << ut_change << endl;
	if (pri_ut < ut_change)
		return true;
	return false;
}