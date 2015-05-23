#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include<vector>
#include <sstream>
#include"StatTiAn.h"
#include"Parser.h"

using namespace std;

string convert_to_string(int  i)
{ // convert from int to string
	stringstream out;
	out << i;
	return (out.str());
}
StaticTiAnalysis::StaticTiAnalysis()
{

}
StaticTiAnalysis::~StaticTiAnalysis()
{

}

void StaticTiAnalysis::ReadNetlist(string filelocation)
{
	ifstream myfile;
	//___________PLZ TAKE CARE OF THE LOCATION_____________//
	//myfile.open("D:\\Auc Courses\\Spring 2015\\Digital Design II\\Projects\\Static Timing Analysis\\Static Tiiming\\Beautifier\\Test Cases and Output file\\booth.g.v");              //source file to retrieve netlist from
	myfile.open(filelocation);
	string line;
	string line2 = "NOR2X4 U111(.A(n118), .B(n172), .Y(n116));";
	smatch m;

	//regular expressions for different line formats

	regex modulename("\\s*([[:w:]]+)\\s+([[:w:]]+)\\s*\\(");
	regex mline1("\\s*\\.([[:w:]]+)\\(([[:w:]]+\\[[[:d:]]+\\])\\s*\\)\\s*,\\s*"); //input with [] and with comma
	regex mline11("\\s*\\.([[:w:]]+)\\(([[:w:]]+)\\s*\\)\\s*,\\s*"); //correct ,input without  [] and with comma
	regex mline42("\\s*\\.([[:w:]]+)\\(([[:w:]]+\\[[[:d:]]+\\])\\s*\\)\\s*"); //output with []
	regex mline442("\\s*\\.([[:w:]]+)\\(([[:w:]]+)\\s*\\)\\s*"); //output without  []
	regex wire2("\\s*wire\\s+([[:w:]]+)\\s*\\;\\s*"); //WIRE without []
	regex wire("\\s*wire\\s+\\[\\s*([[:d:]]+)\\s*\\:\\s*([[:d:]]+)\\s*\\]\\s*([[:w:]]+)\\s*\\;\\s*"); //WIRE with []
	regex input("\\s*input\\s+\\[\\s*([[:d:]]+)\\s*\\:\\s*([[:d:]]+)\\s*\\]\\s*([[:w:]]+)\\s*\\;\\s*"); //input with []
	regex input2("\\s*input\\s+([[:w:]]+)\\s*\\;\\s*"); //input without []
	regex output("\\s*output\\s+\\[\\s*([[:d:]]+)\\s*\\:\\s*([[:d:]]+)\\s*\\]\\s*([[:w:]]+)\\s*\\;\\s*"); //output with []
	regex output2("\\s*output\\s+([[:w:]]+)\\s*\\;\\s*"); //output without []
	regex assign("\\s*assign\\s+([[:w:]]+)\\s*\\[\\s*([[:d:]]+)\\s*\\]\\s*\\=\\s*([[:w:]]+)\\s*\\[\\s*([[:d:]]+)\\s*\\]\\s*\\;\\s*"); // assign x[d]=y[d]
	regex assign2("\\s*assign\\s+([[:w:]]+)\\s*\\[\\s*([[:d:]]+)\\s*\\:\\s*([[:d:]]+)\\s*\\]\\s*\\=\\s*([[:w:]]+)\\s*\\[\\s*([[:d:]]+)\\s*\\]\\s*\\;\\s*"); // assign x[d:d]=y[d:d]
	regex assign3("\\s*assign\\s+([[:w:]]+)\\s*\\=\\s*([[:w:]]+)\\s*\\;\\s*"); //assign x=y
	regex endofnetlist("\\s*\\)\\;\\s*"); // to check the end of module

	//start reading file
	if (!myfile.fail())
	{
		while (!myfile.eof())
		{
			getline(myfile, line);
			if (regex_match(line, m, modulename))          //if line is module type and name, ex. "NOR2X2 _2_ ("
			{
				int incount;
				if (m.str(1) == "INVX1")
					incount = 1;
				else incount = 2;
				module temp;
				temp.name = m.str(2);
				temp.type = m.str(1);
				cout << "Name: " << temp.name << endl;

				///////////////////////////////////////////////////////////////////////////////////////////////////
				//inserting inputs and output
				bool o = false;
				while (!o)
				{
					getline(myfile, line);
					cout << "Line: " << line << endl;
					if (regex_match(line, m, mline1)) //.A(md[0]),
					{//input
						pair<string, string> x;
						x.first=m.str(1);
						x.second=m.str(2);
						temp.inputs.push_back(x);
						cout << "Input: " << temp.inputs.back().first << "on pin: " << temp.inputs.back().second<< endl;
					}
					else if (regex_match(line, m, mline11)) //.A(md),
					{//input
						pair<string, string> x;
						x.first = m.str(1);
						x.second = m.str(2);
						temp.inputs.push_back(x);					
						cout << "Input: " << temp.inputs.back().first << "on pin: " << temp.inputs.back().second << endl;
					}
					else if (regex_match(line, m, mline42)) //.A(md[0]) //for output
					{
						temp.output = m.str(2);
						cout << "Output: " << temp.output << endl << "----------------------\n";
						o = true;
					}
					else if (regex_match(line, m, mline442))//.A(md) //for output 
					{
						temp.output = m.str(2);
						cout << "Output: " << temp.output << endl << "----------------------\n";
						o = true;
					}
				}
				nodes.push_back(temp);
			}

			else if (regex_match(line, m, input))               //if line is netlist input, ex. ".A(md[0]),"
			{
				module temp;
				for (int c = atoi(m.str(2).c_str()); c <= atoi(m.str(1).c_str()); c++)
				{
					temp.name = m.str(3) + '[' + convert_to_string(c) + ']';
					temp.type = "input";
					nodes.push_back(temp);
				}
			}
			else if ((regex_match(line, m, input2)))            //if line is netlist input, ex. ".A(_0_),"
			{
				module temp;
				temp.name = m.str(1);
				temp.type = "input";
				nodes.push_back(temp);
			}
			else if (regex_match(line, m, output))              //if line is netlist output, ex. ".Y(md[0]),"
			{
				module temp;
				for (int c = atoi(m.str(2).c_str()); c <= atoi(m.str(1).c_str()); c++)
				{
					temp.name = m.str(3) + '[' + convert_to_string(c) + ']';
					temp.type = "output";
					nodes.push_back(temp);
				}
			}
			else if (regex_match(line, m, output2))             //if line is netlist output, ex. ".Y(_0_),"
			{
				module temp;
				temp.type = "output";
				temp.name = m.str(1);
				nodes.push_back(temp);
			}
			else if (regex_match(line, m, wire))                 //if line is wire declaration, ex. "f[0]"
			{
				module temp;
				cout << "Type: wires(BUS)" << endl;
				for (int c = atoi(m.str(2).c_str()); c <= atoi(m.str(1).c_str()); c++)
				{
					temp.name = m.str(3) + '[' + convert_to_string(c) + ']';
					temp.type = "wire";
					nodes.push_back(temp);
					cout << "Name: " << temp.name << endl;
					cout << "----------------------\n";
				}
			}
			else if (regex_match(line, m, wire2))                 //if line is netlist input, ex. "_0_"
			{
				module temp;
				temp.type = "wire";
				temp.name = m.str(1);
				nodes.push_back(temp);
				cout << "Name: " << temp.name << endl;
				cout << "Type: " << temp.type << endl;
				cout << "----------------------\n";
			}
			else if (regex_match(line, m, assign)) //assign x[2]=b[1]
			{
				module temp;
				temp.name = m.str(1) + '[' + m.str(2) + ']';

				temp.type = "assignment";
				temp.output = m.str(3) + '[' + m.str(4) + ']';
				nodes.push_back(temp);
				cout << "Name: " << temp.name << endl;
				cout << "Type: " << temp.type << endl;
				cout << "Input: " << temp.output << endl << "----------------------\n";
			}
			else if (regex_match(line, m, assign2)) //assign x[d:d] = y[d:d]
			{
				int def = atoi(m.str(3).c_str()) - atoi(m.str(6).c_str());

				module temp;
				temp.type = "assigment";
				for (int c = atoi(m.str(3).c_str()); c <= atoi(m.str(2).c_str()); c++)
				{
					cout << "Name: " << temp.name << endl;
					cout << "Type: " << temp.type << endl;
					temp.name = m.str(1) + '[' + convert_to_string(c) + ']';
					temp.output = m.str(4) + '[' + convert_to_string(c + def) + ']';
					cout << "Input: " << temp.output;
					nodes.push_back(temp);
				}
			}
			else if (regex_match(line, m, assign3)) //assign x=y
			{
				module temp;
				temp.name = m.str(1);
				temp.type = "assigment";
				temp.output = m.str(2);
				nodes.push_back(temp);
				cout << "Name: " << temp.name << endl;
				cout << "Input: " << m.str(2);
				cout << "Type: " << temp.type << endl;
			}

		}
	}
	myfile.close();

}
void StaticTiAnalysis::ReadLib(string loc)
{
	extract.ReadLib(loc);
}
void StaticTiAnalysis::CreateGraphy()
{
	for (int y = 0; y < nodes.size(); y++)
	{
		if (nodes[y].type != "wire") // cause we need to insert the wire at the end , to correctly create the best level accuracy
		{
			addEdge(nodes[y].name, nodes[y].type, nodes[y].output,"!");
			int f = get_index(nodes[y].name, adj);
			if (nodes[y].inputs.size() != 0)
			{
				for (int c = 0; c < nodes[y].inputs.size(); c++)
					addEdge(nodes[y].inputs[c].second, "!", nodes[y].name, nodes[y].inputs[c].first); // this for compeleting the missed part in the nodes with type !
			}
			else
			{
				if (nodes[y].type != "output" && nodes[y].type != "wire")
					adj[f].level = 1;
			}
		}
		else
		{
			wires.push_back(y);
		}
	}
	for (int y = 0; y < wires.size(); y++) // add the wires now
	{
		addEdge(nodes[wires[y]].name, nodes[wires[y]].type, nodes[wires[y]].output,"@");
		if (nodes[wires[y]].inputs.size() != 0)
		{
			for (int c = 0; c < nodes[wires[y]].inputs.size(); c++)
				addEdge(nodes[wires[y]].inputs[c].second, "!", nodes[y].name, nodes[wires[y]].inputs[c].first);
		}

	}
	for (int j = 0; j < adj.size(); j++)
	{
		cout << j << " ~ " << adj[j].node << " is a(n) " << adj[j].type;
		cout << " its output is connected to : ";
		for (int g = 0; g < adj[j].edges.size(); g++)
			cout <<" , " << adj[j].edges[g].second << " through " << adj[j].edges[g].first;
		cout << endl;
	}
	topologicalSort();                //topological sorting and JSON file generation
}
									//name,type,outputname,pin 
void StaticTiAnalysis::addEdge(string v, string t, string w, string p)
{

	bool s = 0; // this for w , if found
	bool s2 = 0; // this for v, if found

	if (adj.size() != 0) // if it is the first 2/1 nodes
	{
		for (int j = 0; j < adj.size(); j++)
		{
			if (adj[j].node == v)
			{
				s2 = 1;
				if (adj[j].type == "!" || adj[j].type == "") //since sometimes inside the main function we pass portions of the nodes via another one. so we solve that by implementing this
					adj[j].type = t;
				if (w != "") // if it is a wire, input  or output
				{
					pair<string, string>x;
					x.first =p ;
					x.second = w;
					adj[j].edges.push_back(x);

					for (int x = 0; x < adj.size(); x++)
					{
						if (adj[x].node == w)
						{
							s = 1;
							break;
						}
					}
				}
				else
					s = 1;
				if (s == 0)
				{
					cell temp; temp.node = w; temp.type = t; adj.push_back(temp);
				}

			}
			else if (adj[j].node == w) s = 1;

			if (s2 == 1) break;


		}
		if (!s2)
		{
			cell temp;
			temp.node = v;
			if (w != "")
			{
				pair<string, string>x;
				x.first = p;
				x.second = w;
				temp.edges.push_back(x);
			}
			temp.type = t;
			adj.push_back(temp);
			if (!s && t != "wire" && t != "input" && t != "output") { cell temp2; temp2.node = w; adj.push_back(temp2); }
		}
	}
	else // if it the first two/one input
	{
		cell temp;
		temp.node = v;
		temp.type = t;
		adj.push_back(temp);
		if (t != "wire" &&  t != "input")
		{
			cell temp2;
			pair<string, string>x;
			x.first = p;
			x.second = w;
			temp2.node = w;
			temp.edges.push_back(x);
			adj.push_back(temp2);
		}
	}
}

int  StaticTiAnalysis::get_index(string v, vector<cell> adj)
{
	//to get the v index in the vecotr adj
	for (int u = 0; u < adj.size(); u++)
	if (v == adj[u].node)
		return u;
}

void StaticTiAnalysis::topologicalSortUtil(int s, int v, vector<cell> &adj, stack<string> &Stack, bool tillReg, bool& WE, bool&  reg_flag, double input_net_trans)
{
	double outputcap = 0, arrival = 0, required = 0;
	// Mark the current node as visited.
	//adj[v].visted = true;
	// i removed the visted bool, cause we need a better calulation for the levels. we need to access the nodes with different parents and choose the highest level value
	 
	if (adj[v].type == "wire")
		adj[v].level = adj[s].level; // same level value if it was a wire
	
	else if (adj[v].type != "input")
		{
			int u = adj[s].level + 1; // if it isn't a wire nor a  input, increase it level value

			if (u > adj[v].level) // if the old value is less than that of calulated 
				adj[v].level = u; //replace it
			if (adj[v].level > highest)//getting the last(highest) level to assign it to all outputs
				highest = adj[v].level;

			//getting the capacitence on the output

			for (int j = 0; j < adj[v].edges.size(); j++)
			{
				//since all cells is only conected to wires, I mean all cells' either inputs are either wires/inputs and all outpus are either wires/output
				int x = get_index(adj[v].edges.at(j).second, adj);
				if (adj[x].type == "wire")
				{
					outputcap += GetWireCap(adj[x].node);
					for (int r = 0; r < adj[x].edges.size(); r++)
					{
						int xx = get_index(adj[x].edges.at(r).second, adj);
						outputcap += extract.input_capac(adj[xx].type, "A");
					}
				}
				/*if (adj[x].type == "wire")	outputcap += GetWireCap(adj[x].node);
				else
					outputcap += extract.input_capac(adj[x].type, "A");*/
			}
			//getting the delay of the gate 
			arrival += extract.getCellDelay(adj[v].type, "CLKh", outputcap, input_net_trans);
			//getting the out net_Trans
			input_net_trans = extract.getCellTransition(adj[v].type, "CLKh", outputcap, input_net_trans);
			
		}

	// Recurse for all the vertices adjacent to this vertex
	for (int j = 0; j < adj[v].edges.size(); j++)
	{
		tillReg == false;

		//getting the PATHS
		//putting this if condition here eleminates the Reg-out and in-out
		if (/*Paths.size() != 0 &&*/ tillReg == false && WE == true && p.size() != 0)
		{
			string h;
			//we found a path to a reg
			if (adj[v].type.find("DFF") != string::npos)
			{
				h = adj[v].node + "*R*" + ',';
				if (reg_flag == false)
					tillReg = true;
				else
					reg_flag = false;
				//Paths.append(h);
				p.back().append(h);
			}
			else if (p.back().find(adj[v].node) != string::npos)// if it is an alternative root
			{
				//----
						int v1 = p.back().find(adj[v].node);//the begnning of the node name to be compied 
						h = p.back().substr(0, v1 );//copy the orginal path till the twin's location
						h += adj[v].node + ',';
						p.push_back(h);
			}
			else
			{
				h = adj[v].node + ',';
				//Paths.append(h);
				p.back().append(h);
			}
			
		}
		int x = get_index(adj[v].edges.at(j).second, adj);
		topologicalSortUtil(v, x, adj, Stack, tillReg, WE, reg_flag, input_net_trans);
	}

	// Push current vertex to stack which stores result
	Stack.push(adj[v].node);


}


void StaticTiAnalysis::topologicalSort()
{
	stack<string> Stack;
	int intialdelay;
	bool tillReg;
	bool WE;
	bool reg_flag;
	for (int i = 0; i < adj.size(); i++)
	{
		double input_net_trans = 1.5;
		tillReg = false; // to be changed dynamically inside the recursive loop
		WE = true; // to not have path from each wire/cominational logic. just from input and regs
		reg_flag = false; //if the first node to traverser from is already a Reg. cause we aggreed that all paths are : input -  REG  & Reg-Reg
		if (adj[i].type == "input" && adj[i].node.find("clk") == string::npos)
		{
			//Paths.append("\n~i*"); for testing
			p.push_back("~i*");
			input_net_trans = GetInputDelay(adj[i].node); //setting the input delay
		}

		else if (adj[i].type.find("DFF") != string::npos)
		{
			//Paths.append("\n~R*"); for testing
			p.push_back("~R*");
			reg_flag = true;
		}
		else
			WE = false;
		
		topologicalSortUtil(0, i, adj, Stack, tillReg, WE, reg_flag, input_net_trans);
	}

	for (int i = 0; i < p.size(); i++)
	{
		p[i].erase(p[i].size()-1, 1);//for the comma
	}
}

void StaticTiAnalysis::printPaths()
{
	cout << "ALL PATHS " << endl;
		//cout << Paths<< endl; for testing
		cout << "------------------------" << endl;

		for (int g = 0; g < p.size(); g++)
		{
			cout << p[g] << endl;
		}
		cout << "*************************" << endl;
		
}

void StaticTiAnalysis::writeJsonfile(string filelocation)
{
	cout << endl;

	//JSON file generation (manual, without using libraries)
	//___________PLZ TAKE CARE OF THE LOCATION_____________//
	//ofstream jsonout("D:\\Auc Courses\\Spring 2015\\Digital Design II\\Projects\\Static Timing Analysis\\Static Tiiming\\Beautifier\\myout.txt");                   //destination file to export JSON data to
	ofstream jsonout(filelocation);
	vector<string> temp;
	jsonout << "{" << endl;

	//////////////////////////////////////////////////////////////////////Inputs
	jsonout << " \"inputs\": {" << endl;

	for (int u = 0; u < adj.size(); u++)
	{
		if (adj[u].type == "input")
		{
			temp.push_back(adj[u].node);
		}
	}
	for (int u = 0; u < temp.size() - 1; u++)
	{
		jsonout << "  \"" << temp[u] << "\"," << endl;
	}
	jsonout << "  \"" << temp[temp.size() - 1] << "\"" << endl;
	jsonout << " }," << endl;

	//////////////////////////////////////////////////////////////////////Outputs
	vector<string>temp2;
	vector<int> templevel;
	jsonout << " \"outputs\": [" << endl;
	for (int u = 0; u < adj.size(); u++)
	{
		if (adj[u].type == "output")
		{
			adj[u].level = highest;
			temp2.push_back(adj[u].node);
			templevel.push_back(adj[u].level);
		}
	}
	for (int u = 0; u < temp2.size() - 1; u++)
	{
		jsonout << "  \{" << endl;
		jsonout << "   \"name\":   \"" << temp2[u] << "\"," << endl;
		jsonout << "   \"level\":   " << templevel[u] << endl;
		jsonout << "  \}," << endl;
	}
	jsonout << "  \{" << endl;
	jsonout << "   \"name\":   \"" << temp2[temp2.size() - 1] << "\"," << endl;
	jsonout << "   \"level\":   " << templevel[temp2.size() - 1] << endl;
	jsonout << "  \}," << endl;

	jsonout << " ]" << endl;

	//////////////////////////////////////////////////////////////////////Modules
	vector<string>temp3;
	vector<string>temptype;
	vector<int>templevel2;
	jsonout << " \"modules\": [" << endl;
	for (int u = 0; u < adj.size(); u++)
	{
		if ((adj[u].type != "output") && (adj[u].type != "input") && (adj[u].type != "wire"))
		{
			temp3.push_back(adj[u].node);
			templevel2.push_back(adj[u].level);
			temptype.push_back(adj[u].type);
		}
	}
	for (int u = 0; u < temp3.size() - 1; u++)
	{
		jsonout << "  \{" << endl;
		jsonout << "   \"type\":   \"" << temptype[u] << "\"," << endl;
		jsonout << "   \"name\":   \"" << temp3[u] << "\"," << endl;
		jsonout << "   \"level\":   " << templevel2[u] << "," << endl;
		jsonout << "  \}," << endl;
	}
	jsonout << "  \{" << endl;
	jsonout << "   \"type\":   \"" << temptype[temp3.size() - 1] << "\"," << endl;
	jsonout << "   \"name\":   \"" << temp3[temp3.size() - 1] << "\"," << endl;
	jsonout << "   \"level\":   " << templevel2[temp3.size() - 1] << endl;
	jsonout << "  \}," << endl;
	jsonout << " ]" << endl;

	jsonout << "}" << endl;

}

void StaticTiAnalysis::ReadSkewFile(string filelocation)
{
	regex input("([[:w:]]+)\\s([[:w:]]+)");

	ifstream myfile;

	smatch m;

	string line;

	int n = 0;
	myfile.open(filelocation);
	if (!myfile.fail())
	{
		FFSkew temp;
		while (!myfile.eof())
		{
			getline(myfile, line);
			if (regex_match(line, m, input))
			{
				temp.FFName = m.str(1).c_str();
				temp.skew = stof(m.str(2));
				SkewsArray.push_back(temp);
			}
			else{ cout << "Syntax Error at line" << n << endl; }
			n++;
		}
		for (int i = 0; i < SkewsArray.size(); i++)
			cout << SkewsArray[i].FFName << " " << SkewsArray[i].skew << endl;
	}
	else
	{
		throw exception("Unable to open skew file\n");
	}
}

void StaticTiAnalysis::ReadWireCapFile(string filelocation)
{
	regex input("([[:w:]]+)\\s([0-9]*\\.[0-9]+)");

	ifstream myfile;

	smatch m;

	string line;

	int n = 0;
	myfile.open(filelocation);
	if (!myfile.fail())
	{
		cout << "Net Capacitance File opened successfully\n";
		wire temp;
		while (!myfile.eof())
		{
			getline(myfile, line);
			if (regex_match(line, m, input))
			{
				temp.name = m.str(1).c_str();
				temp.cap = stof(m.str(2));
				WireCap.push_back(temp);
			}
			else{ cout << "Syntax Error at line" << n << endl; }
			n++;
		}
		for (int i = 0; i < WireCap.size(); i++)
			cout << WireCap[i].name << " " << WireCap[i].cap << endl;
	}
	else
	{
		throw exception("Unable to open net capacitance file\n");
	}
}

float StaticTiAnalysis::GetSkew(string ffname)
{
	bool found = false;
	for (int i = 0; i<SkewsArray.size(); i++)
	{
		if (SkewsArray[i].FFName == ffname)
		{
			found = true;
			return SkewsArray[i].skew;
		}
	}
	if (!found)
		throw exception("Flip Flop not found.\n");

}

float StaticTiAnalysis::GetWireCap(string wirename)
{
	bool found = false;
	for (int i = 0; i<SkewsArray.size(); i++)
	{
		if (WireCap[i].name == wirename)
		{
			found = true;
			return WireCap[i].cap;
		}
	}
	if (!found)
		throw exception("Wire not found.\n");

}

void StaticTiAnalysis::ReadConstraints(string filelocation)
{
	regex input("([[:w:]]+)\\s([[:w:]]+)");

	ifstream myfile;

	smatch m;

	string line;

	int n = 0;
	myfile.open(filelocation);
	if (!myfile.fail())
	{
		getline(myfile, line);
		clockperiod = atoi(line.c_str());
		cout << "Clock Period=" << clockperiod << endl;

		InDelay temp;
		while (!myfile.eof())
		{
			getline(myfile, line);
			if (regex_match(line, m, input))
			{
				temp.input = m.str(1).c_str();
				temp.delay = stof(m.str(2));
				inputdelay.push_back(temp);
			}
			else{ cout << "Syntax Error at line" << n << endl; }
			n++;
		}
		for (int i = 0; i < inputdelay.size(); i++)
			cout << inputdelay[i].input << " " << inputdelay[i].delay << endl;
	}
	else
	{
		throw exception("Unable to open time constraints file.\n");
	}
}

int StaticTiAnalysis::GetInputDelay(string input)
{
	bool found = false;
	for (int i = 0; i<inputdelay.size(); i++)
	{
		if (WireCap[i].name == input)
		{
			found = true;
			return inputdelay[i].delay;
		}
	}
	if (!found)
		throw exception("Input not found.\n");
}
