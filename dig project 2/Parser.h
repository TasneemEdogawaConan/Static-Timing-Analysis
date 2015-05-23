#include<iostream>
#include<vector>
#include<string>
#include"Cell.h"
using namespace std;
#ifndef PARSER_H
#define PARSER_H

class Parser
{
public:
	void ReadLib(string lib_name);

	float getCellDelay(string cell, string pin, float outputcap, float inputtrans);
	float getCellTransition(string cell, string pin, float outputcap, float inputtrans);

	
	float Cell_Set_Hold(string cell, string pin, float index2, float index1, int b);
	float input_capac(string cell, string pin);
	
	
private:
	Cell Return_Cell(string name);
	PinI Return_Pin(string cell,string name);
	int char_int(char c);
	float Cell_Delay(string celltype, string pin, float index1, float index2, int b);
	float Cell_Transition(string celltype, string pin, float index1, float index2, int b);
	vector<Cell> library;
};
#endif 