#include<string>
using namespace std;
#ifndef PINI_H
#define PINI_H
class PinI
	{
	public:
		string nameP;
		float capacitance;
		float rise_capacitance;
		float fall_capacitance;
		char time_sense;
		string timing_type;
		bool EN;
		bool CLK;
		vector<vector<float>> riseC; vector<float>riseC_index1; vector<float>riseC_index2;
		vector<vector<float>> fallC; vector<float>fallC_index1; vector<float>fallC_index2;
		vector<vector<float>> riseT; vector<float>riseT_index1; vector<float>riseT_index2;
		vector<vector<float>> fallT; vector<float>fallT_index1; vector<float>fallT_index2;

		vector<vector<float>> riseP; vector<float>riseP_index1; vector<float>riseP_index2;
		vector<vector<float>> fallP; vector<float>fallP_index1; vector<float>fallP_index2;
		//////if EN // clk
		vector<vector<float>> risePEN; vector<float>risePEN_index1; vector<float>risePEN_index2;
		vector<vector<float>> fallPEN; vector<float>fallPEN_index1; vector<float>fallPEN_index2;
		//////if C
		vector<vector<float>> riseCLK; vector<float>riseCLK_index1; vector<float>riseCLK_index2;
		vector<vector<float>> fallCLK; vector<float>fallCLK_index1; vector<float>fallCLK_index2;
		float MPWH; ///minimum_pulse_width_high
		float MPWL;///minimum_pulse_width_low
	};
#endif