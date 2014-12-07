#include <fstream>
#include <iostream>
#include "menu.h"
#include "db.h"

using namespace std;

db<int> db1(string("config/test.db"));
void writedb()
{
	int s;
	wcout << L"输入要写入的整数：" << endl;
	wcin >> s;
	db1.getData().push_back(s);
	db1.putData();
}

void readdb()
{
	vector<int>& v = db1.getData();
	for(vector<int>::iterator i = v.begin(); i!=v.end();i++)
		wcout << *i << endl;
}

void printsub1()
{
	wcout << L"Hello,sub1." << endl;
}

int main()
{
	wifstream infile("config/menu.config");
	MenuCreator a(infile);
	a.bind(L"main2", writedb);
	a.retext(L"main2", L"测试数据库写入");
	a.bind(L"main3", readdb);
	a.retext(L"main3", L"测试数据库读取");
	a.bind(L"sub1", printsub1);
	a.execute();
}
