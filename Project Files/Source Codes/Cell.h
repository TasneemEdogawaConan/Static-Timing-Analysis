#include<iostream>
#include<vector>
#include"PinI.h"
#include"PinO.h"
#include<string>
using namespace std;
#ifndef CELL_H
#define CELL_H
class Cell
	{
	public:
		string nameC;
		float area;
		vector <PinO> out;
		vector <PinI> pins;
		float C_Leakage_P;
	};
#endif