#ifndef STA
#define STA
#include<string>
#include<vector>
#include<stack>
#include"structs.h"
#include"Parser.h"

using namespace std;
class StaticTiAnalysis
{
public:
						//Method members
	StaticTiAnalysis();
	~StaticTiAnalysis();
						//NELIST METHODS
	void ReadNetlist(string filelocation);
	void writeJsonfile(string filelocation);
	void CreateGraphy();

						//liberatiy Method
	void ReadLib(string loc);
						//DAG,PATHS METHODS
	void printPaths();
	void topologicalSortUtil(int s, int v, vector<cell> &adj, stack<string> &Stack, bool tillReg, bool& WE, bool&  reg_flag, double input_net_trans); //part with makes the DFS
	void addEdge(string v, string t, string w, string p); //to add a directed edge
	int get_index(string v, vector<cell> adj); //to get a specific index of a node since there is no find dunction in vectors
	void topologicalSort(); //this function loops on all the nodes and do DFS (using topological logic)to get the correct level
						//SKEW ,Delat, & Capacitance METHODS


	void StaticTiAnalysis::ReadSkewFile(string filelocation);
	float StaticTiAnalysis::GetSkew(string ffname);
	void StaticTiAnalysis::ReadWireCapFile(string filelocation);
	float StaticTiAnalysis::GetWireCap(string wirename);
	void StaticTiAnalysis::ReadConstraints(string filelocation);
	int StaticTiAnalysis::GetInputDelay(string input);
						////
	void analyzeAndSort();
	void outputTable();
						//variable members
	vector<FFSkew> SkewsArray;
	int clockperiod;
	vector<InDelay> inputdelay;
	vector<wire> WireCap;
	vector<TimingPath> paths;
	vector<module> nodes;
	vector<string>p; //for paths
	vector<int>wires;
	int highest=0;
	Parser extract;
	vector<cell> adj; //this is a vector of a struct which will carry each node's info : outwards edges , name, type, etc
};

#endif