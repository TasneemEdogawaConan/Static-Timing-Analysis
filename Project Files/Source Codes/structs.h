#ifndef structs
#define structs
#include<string>
#include<vector>
using namespace std;
struct cell
{
	string node;
	string type;
	int level;
	string pin;
	vector<pair<string,string>> edges;

};
struct module{
	string name;
	string type;
	string output;
	vector<pair<string,string>> inputs;
};

struct FFSkew
{
	string FFName;
	float skew;
};

struct wire
{
	string name;
	float cap;
};

struct InDelay
{
	string input;
	float delay;
};
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct TimingPath
{
	string pathlist;
	vector<string> gate_name;
	vector<float> gate_delay;
	float comb_delay;
	float capture_skew;
	float capture_setup;
	float out_constraints;
	float input_arrival;         // Or release skew + release Tcq
	float arrival;
	float required;
	float slack;
};
#endif 