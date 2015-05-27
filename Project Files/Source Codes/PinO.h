#include"PinI.h"
#include<string>
using namespace std;
#ifndef PINO_H
#define PINO_H
class PinO
	{
	public:
		string nameO;
		float capacitance;
		float rise_capacitance;
		float fall_capacitance;
		float max_capacitance;
	};
#endif