#include <fstream>
#include <iostream>
#include "menu.h"

using namespace std;
void print()
{
	wcout << L"Hello,World!" << endl;
}

void printsub1()
{
	wcout << L"Hello,sub1." << endl;
}

int main()
{
	wifstream infile("config/menu.config");
	MenuCreator a(infile);
	a.bind(L"main2", print);
	a.bind(L"sub1", printsub1);
	a.execute();
}
