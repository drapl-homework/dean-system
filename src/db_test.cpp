#include <fstream>
#include <iostream>
#include "menu.h"
#include "db.h"

using namespace std;

db<int> db1(string("config/test.db"));
void writedb()
{
	int s;
	cout << "输入要写入的整数：" << endl;
	cin >> s;
	db1.getData().push_back(s);
	db1.putData();
}

void readdb()
{
	vector<int>& v = db1.buffer;
	for(vector<int>::iterator i = v.begin(); i!=v.end();i++)
		cout << *i << endl;
}

void printsub1()
{
	cout << "Hello,sub1." << endl;
}

int main()
{
	ifstream infile("config/menu.config");
	MenuCreator a(infile);
	a.bind("main2", writedb);
	a.retext("main2", "测试数据库写入");
	a.bind("main3", readdb);
	a.retext("main3", "测试数据库读取");
	a.bind("sub1", printsub1);
	a.execute();
}
