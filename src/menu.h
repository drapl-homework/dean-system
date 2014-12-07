#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <iostream>
#include "stringutils.h"

using namespace std;

#ifndef _MENU_H_
#define _MENU_H_ 1

struct MenuItem;

class menu{ //菜单类
	public:
		menu();
		void addItem(wstring label, wstring text, MenuItem item); //增加菜单项目
		int bind(wstring label, void(*function)()); //绑定函数到某个菜单项
		int retext(wstring label, wstring text); //修改某菜单项名字
		void(*header_function)(); //显示菜单之前打印标题的函数
		int print_and_choose(); //弹出选择对话框并返回选择值
		void execute(); //执行菜单逻辑
	private:
		void print_menu(); //打印菜单文字
		void number(); //给项目编号
		map<wstring,pair<wstring,MenuItem> > items; //菜单项
		/*给菜单项目编号到idmap*/
		vector<map<wstring,pair<wstring,MenuItem> >::iterator > idmap;
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

void menu::addItem(wstring label, wstring text, MenuItem item)
{
	items.insert(pair<wstring, pair<wstring, MenuItem> >(label, pair<wstring, MenuItem>(text, item)));
	number();
}

void menu::number()
{
	idmap.clear();
	/*给菜单项目编号*/
	for(map<wstring,pair<wstring,MenuItem> >::iterator it = items.begin();
			it !=items.end(); it++)
		idmap.push_back(it);
}

class BindError{};
int menu::bind(wstring label, void(*function)())
{
	int result = 0;
	if(label == L"__MAIN__") //绑定主菜单
	{
		header_function = function;
		return 1;
	}

	for(map<wstring,pair<wstring,MenuItem> >::iterator it = items.begin();
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

int menu::retext(wstring label, wstring text)
{
	int result = 0;
	for(map<wstring,pair<wstring,MenuItem> >::iterator it = items.begin();
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
	wcout << endl << endl << endl << L"====================" << endl;
	if(header_function)
		header_function(); //执行标题函数

	/*打印菜单*/
	for(vector<map<wstring,pair<wstring,MenuItem> >::iterator >::iterator it = idmap.begin(); it != idmap.end(); it++)
	{
		wcout << L"[" << int(it-idmap.begin()+1) << L"]";
		wcout << (*it)->second.first << endl;
	}
}

int menu::print_and_choose() //打印并等待用户选择
{
	print_menu();
	wcout << L"请选择（输入0返回）：\n" << endl;
	int selected;
	wcin >> selected;
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
				cout << L"功能未实现！" << endl;
				continue;
			}
			idmap[selected - 1]->second.second.function();
		}
	}
}


class SyntaxError{};

class MenuCreator{ //从文件生成菜单
	public:
		MenuCreator(wifstream& infile);
		~MenuCreator();
		void execute(); //执行主菜单
		int bind(wstring label, void(*function)()); //绑定函数到某项
		int retext(wstring label, wstring text); //修改某项文字
		int print_and_choose(); //作为选择器使用
		menu* parse(vector<wstring>::iterator begin, vector<wstring>::iterator end, vector<wstring>::iterator& it);
	private:
		menu* topMenu;
		vector<menu*> pool; //记录申请的内存，便于回收
	
};

MenuCreator::MenuCreator(wifstream& infile)
{
	vector<wstring> menuwstring;
	wstring buffer;
	getline(infile, buffer);
	if(buffer != L"{")
		throw SyntaxError();
	while(not infile.eof())
	{
		getline(infile, buffer);
		menuwstring.push_back(trim(buffer));
	}
	vector<wstring>::iterator it = menuwstring.begin();
	topMenu = parse(menuwstring.begin(), menuwstring.end(), it);
}

MenuCreator::~MenuCreator()
{
	for(vector<menu*>::iterator it=pool.begin(); it!=pool.end(); it++)
		delete *it;
}

void MenuCreator::execute()
{
	topMenu->execute();
}

int MenuCreator::bind(wstring label, void(*function)())
{
	return topMenu->bind(label, function);
}

int MenuCreator::print_and_choose() //打印并等待用户选择
{
	return topMenu->print_and_choose();
}


int MenuCreator::retext(wstring label, wstring text)
{
	return topMenu->retext(label, text);
}

menu* MenuCreator::parse(vector<wstring>::iterator begin, vector<wstring>::iterator end, vector<wstring>::iterator& it)
{
	/* 菜单文件格式：
	 * {
	 * 	标记:项目文字（普通项目）
	 * 	标记:项目文字（子菜单）
	 * 	{
	 * 	}
	 * }
	 */
	menu* m = new menu;
	pool.push_back(m);
	wstring label, text;
	while(true)
	{
		if(*it == L"}") //结束
		{
			it++;
			break;
		}
		MenuItem item;
		item.function = NULL;

		/* 解析标记和文字 */
		wstring label, text;
		vector<wstring> tmp = split(*it, L':');
		label = tmp[0];
		text = tmp[1];
		it++;

		if(*it == L"{") //下一行判断出是子菜单
		{
			it++;
			vector<wstring>::iterator nbegin = it + 1;
			vector<wstring>::iterator nend = end;
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

#endif
