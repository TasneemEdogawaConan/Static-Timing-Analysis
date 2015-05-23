#include <string>
#include"StatTiAn.h"
using namespace std;
void main()
{
	StaticTiAnalysis test;
	string f1 = "D:\\Auc Courses\\Spring 2015\\Digital Design II\\Projects\\Static Timing Analysis\\Static Tiiming\\Beautifier\\TestCase1_yehia\\netlistfile.txt";
	string f2 = "D:\\Auc Courses\\Spring 2015\\Digital Design II\\Projects\\Static Timing Analysis\\Static Tiiming\\Beautifier\\TestCase1_yehia\\jsonfile.txt";
	string f3 = "D:\\Auc Courses\\Spring 2015\\Digital Design II\\Projects\\Static Timing Analysis\\Static Tiiming\\Beautifier\\TestCase1_yehia\\my_skewfile.skw";
	string f4 = "D:\\Auc Courses\\Spring 2015\\Digital Design II\\Projects\\Static Timing Analysis\\Static Tiiming\\Beautifier\\TestCase1_yehia\\my_inputConstraints.txt";
	string f5 = "D:\\Auc Courses\\Spring 2015\\Digital Design II\\Projects\\Static Timing Analysis\\Static Tiiming\\Beautifier\\TestCase1_yehia\\my_netcap.ncp";
	string f6 = "D:\\Auc Courses\\Spring 2015\\Digital Design II\\Projects\\Static Timing Analysis\\Static Tiiming\\Beautifier\\TestCase1_yehia\\Liberity.lib";
	test.ReadLib(f6);
	test.ReadNetlist(f1);
	test.ReadSkewFile(f3);
	test.ReadConstraints(f4);
	test.ReadWireCapFile(f5);
	test.CreateGraphy();
	test.writeJsonfile(f2);
	test.printPaths();

	system("pause");
}