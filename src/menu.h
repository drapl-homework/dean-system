#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstdio>

using namespace std;

enum ItemType {SUBMENU, FUNCTION};

struct MenuItem;

class BindError{};
class menu{
	public:
		void addItem(string label, string text, MenuItem item);
		int bind(string label, void(*function)());
		int retext(string label, string text);
		void execute();
	private:
		map<string,pair<string,MenuItem> > items;
};

struct MenuItem{
	ItemType type;
	menu* submenu;
	void(*function)();
};

void menu::addItem(string label, string text, MenuItem item)
{
	items.insert(pair<string, pair<string, MenuItem> >(label, pair<string, MenuItem>(text, item)));
}

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

void menu::execute()
{
	while(true)
	{
		vector<map<string,pair<string,MenuItem> >::iterator > idmap;

		for(map<string,pair<string,MenuItem> >::iterator it = items.begin();
				it !=items.end(); it++)
			idmap.push_back(it);
		for(vector<map<string,pair<string,MenuItem> >::iterator >::iterator it = idmap.begin(); it != idmap.end(); it++)
			printf("[%d]%s\n", int(it-idmap.begin()+1), (*it)->second.first.c_str());

		printf("请选择（输入0返回）：\n");
		int selected;
		scanf("%d", &selected);


		if(selected <= 0 )
			break;

		if(idmap[selected - 1]->second.second.type == SUBMENU)
			idmap[selected - 1]->second.second.submenu->execute();

		if(idmap[selected - 1]->second.second.type == FUNCTION)
			idmap[selected - 1]->second.second.function();
	}
}

vector<string> split(const string &s, char delim) {
	vector<string> elems;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

string trim(const string &s)
{
	string s2 = s;
	s2.erase(remove(s2.begin(), s2.end(), '\t'), s2.end());
	return s2;
}

class SyntaxError{};

class MenuCreator{
	public:
		MenuCreator(ifstream& infile)
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

		void execute()
		{
			topMenu->execute();
		}
		
		int bind(string label, void(*function)())
		{
			return topMenu->bind(label, function);
		}

		int retext(string label, string text)
		{
			return topMenu->retext(label, text);
		}

		menu* parse(vector<string>::iterator begin, vector<string>::iterator end, vector<string>::iterator& it)
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
	private:
		menu* topMenu;
};
