#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <iostream>
#include "stringutils.h"

using namespace std;

struct MenuItem;

class menu{ //菜单类
	public:
		menu();
		void addItem(string label, string text, MenuItem item);
		int bind(string label, void(*function)());
		int retext(string label, string text);
		void(*header_function)();
		int print_and_choose();
		void execute();
	private:
		void print_menu();
		void number();
		map<string,pair<string,MenuItem> > items;
		/*给菜单项目编号*/
		vector<map<string,pair<string,MenuItem> >::iterator > idmap;
};

enum ItemType {SUBMENU, FUNCTION};

struct MenuItem{ //菜单表项
	ItemType type; //可以是普通项目或者子菜单
	menu* submenu; //子菜单指针
	void(*function)(); //普通项目执行的函数
};

menu::menu()
{
	header_function = NULL;
}

void menu::addItem(string label, string text, MenuItem item)
{
	items.insert(pair<string, pair<string, MenuItem> >(label, pair<string, MenuItem>(text, item)));
	number();
}

void menu::number()
{
	idmap.clear();
	/*给菜单项目编号*/
	for(map<string,pair<string,MenuItem> >::iterator it = items.begin();
			it !=items.end(); it++)
		idmap.push_back(it);
}

class BindError{};
int menu::bind(string label, void(*function)())
{
	int result = 0;
	for(map<string,pair<string,MenuItem> >::iterator it = items.begin();
			it !=items.end(); it++)
	{
		if(it->first == label and it->second.second.type == FUNCTION)
		// 查找本菜单
		{
			it->second.second.function = function;
			return true;
		}
		if(it->first == label and it->second.second.type == SUBMENU)
		{
			it->second.second.submenu->header_function = function;
			return true;
		}
		if(it->second.second.type == SUBMENU) //查找子菜单
			if(it->second.second.submenu->bind(label,function))
				return true;
	}
	return false;
}

int menu::retext(string label, string text)
{
	int result = 0;
	for(map<string,pair<string,MenuItem> >::iterator it = items.begin();
			it !=items.end(); it++)
	{
		if(it->first == label)
		// 查找本菜单
		{
			it->second.first = text;
			return true;
		}
		if(it->second.second.type == SUBMENU) //查找子菜单
			if(it->second.second.submenu->retext(label,text))
				return true;
	}
	return false;
}

void menu::print_menu()
{
	//打印标题栏
	printf("\n\n\n====================\n");
	if(header_function)
		header_function(); //执行标题函数

	/*打印菜单*/
	for(vector<map<string,pair<string,MenuItem> >::iterator >::iterator it = idmap.begin(); it != idmap.end(); it++)
		printf("[%d]%s\n", int(it-idmap.begin()+1), (*it)->second.first.c_str());
}

int menu::print_and_choose() //打印并等待用户选择
{
	print_menu();
	printf("请选择（输入0返回）：\n");
	int selected;
	cin >> selected;
	return selected;
}

void menu::execute() //显示菜单
{
	while(true)
	{
		int selected = print_and_choose();

		if(!(selected > 0 and selected <= idmap.size()))
			break;

		//进入子菜单
		if(idmap[selected - 1]->second.second.type == SUBMENU)
			idmap[selected - 1]->second.second.submenu->execute();

		//执行函数
		if(idmap[selected - 1]->second.second.type == FUNCTION)
		{
			if(!idmap[selected - 1]->second.second.function)
			{
				printf("功能未实现！\n");
				continue;
			}
			idmap[selected - 1]->second.second.function();
		}
	}
}


class SyntaxError{};

class MenuCreator{
	public:
		MenuCreator(ifstream& infile);
		void execute();
		int bind(string label, void(*function)());
		int retext(string label, string text);
		int print_and_choose();
		menu* parse(vector<string>::iterator begin, vector<string>::iterator end, vector<string>::iterator& it);
	private:
		menu* topMenu;
};

MenuCreator::MenuCreator(ifstream& infile)
{
	string buffer;
	vector<string> menustring;
	getline(infile, buffer);
	if(buffer != "{")
		throw SyntaxError();
	while(not infile.eof())
	{
		getline(infile, buffer);
		menustring.push_back(trim(buffer));
	}
	vector<string>::iterator it = menustring.begin();
	topMenu = parse(menustring.begin(), menustring.end(), it);
}

/*MenuCreator::~MenuCreator() //TODO
{
}*/

void MenuCreator::execute()
{
	topMenu->execute();
}

int MenuCreator::bind(string label, void(*function)())
{
	return topMenu->bind(label, function);
}

int MenuCreator::print_and_choose() //打印并等待用户选择
{
	return topMenu->print_and_choose();
}


int MenuCreator::retext(string label, string text)
{
	return topMenu->retext(label, text);
}

menu* MenuCreator::parse(vector<string>::iterator begin, vector<string>::iterator end, vector<string>::iterator& it)
{
	menu* m = new menu;
	string label, text;
	while(true)
	{
		if(*it == "}") //结束
		{
			it++;
			break;
		}
		MenuItem item;
		item.function = NULL;

		/* 解析标记和文字 */
		string label, text;
		vector<string> tmp = split(*it, ':');
		label = tmp[0];
		text = tmp[1];
		it++;

		if(*it == "{") //下一行判断出是子菜单
		{
			it++;
			vector<string>::iterator nbegin = it + 1;
			vector<string>::iterator nend = end;
			menu* subm = parse(nbegin, nend, it);
			item.type = SUBMENU;
			item.submenu = subm;
		}
		else //没有子菜单
			item.type = FUNCTION;
		m->addItem(label, text, item);
	}
	return m;
}

