#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

#ifndef _TABLEMAKER_H_
#define _TABLEMAKER_H_ 1

int count_width(wstring x) //统计宽度
{
	int length = 0;
	for(int i=0;i<x.size();i++) //统计长度
	{
		if(x[i] < 256) //英文占一个位置
			length++;
		else
			length+=2; //中文占两个位置
	}
	return length;
}

wstring filling(wstring x, int width, wchar_t space=L' ') //填充
{
	wstring n(x);
	int space_count = width - count_width(x);
	for(int i=0; i<space_count; i++)
		n += space;
	return n;
}

class tablemaker: public ostream
{
	public:
		tablemaker(int c):col(c),count(0) {}
		template <class T>
		tablemaker& operator<<(T a);
		void put(bool lineno=true);
	private:
		wstringstream buffer;
		int col;
		int count;
};

template <class T>
tablemaker& tablemaker::operator<<(T a)
{
	buffer << a << endl;
	count++;
	return *this;
}

void tablemaker::put(bool lineno)
{
	vector<wstring> items; //统计项目

	for(int j=0; j<count; j++)
	{
		wstring item;
		getline(buffer, item);
		items.push_back(item);
	}

	vector<int> widths(col+1,0);

	for(int i=0; i<count; i++) //统计每列宽度（行、列从1开始计数）
	{
		int width = count_width(items[i]);
		if(width > widths[i % col + 1])
			widths[i % col + 1] = width;
	}
	
	int total_length = 1; //行总长度
	for(int i=1; i<=col; i++)
		total_length += widths[i] + 1;

	if(lineno) //"序 号|"共6格
		total_length += 5;

	for(int i=0; i<total_length;i++) //输出标题线
		wcout << L"-";
	wcout << endl;

	vector<wstring>::iterator it=items.begin();
	for(int i=1; i<=ceil((float)count/col); i++) //处理每行
	{
		wcout << L"|";
		if(lineno) //带序号的输出
		{
			if(i == 1)
				wcout << L"序 号" << L"|";
			else
				wcout << "  " << i - 1 << L"  |";
		}

		for(int j=1; j<=col; j++) //列数
		{
			wstring item;
			if((i - 1) * col + j <= count)
				item = *(it++);
			else
				item = L"";
			wcout << filling(item, widths[j]) << L"|";
		}
		wcout << endl;
	}

	for(int i=0; i<total_length;i++)
		wcout << L"-";
	wcout << endl;
}

#endif
