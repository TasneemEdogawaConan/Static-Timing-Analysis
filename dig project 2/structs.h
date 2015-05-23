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



#endif 