#include <fstream>
#include <iostream>
#include "menu.h"

using namespace std;
void print()
{
	cout << "Hello,World!" << endl;
}

void printsub1()
{
	cout << "Hello,sub1." << endl;
}

int main()
{
	ifstream infile("config/menu.config");
	MenuCreator a(infile);
	a.bind("main2", print);
	a.bind("sub1", printsub1);
	a.execute();
}
