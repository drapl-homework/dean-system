#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

class tablemaker: public ostream
{
	public:
		tablemaker(int c):col(c),count(0) {}
		template <class T>
		ostream& operator<<(T a);
		void put(bool lineno=true);
	private:
		const static int width = 15;
		stringstream buffer;
		int col;
		int count;
};

template <class T>
ostream& tablemaker::operator<<(T a)
{
	buffer << a << endl;
	count++;
}

void tablemaker::put(bool lineno)
{
	/*
	cout << "┌";
	for(int i=0; i<col; i++)
		for(int j=
		cout << "─"*/
	int total_length = (width + 1) * col + 1; //行总长度
	if(lineno)
		total_length += width + 1;

	for(int i=0; i<total_length;i++)
		cout << "-";
	cout << endl;
	for(int i=1; i<=ceil((float)count/col); i++) //行数
	{
		cout << "|";
		if(lineno) //带序号的输出
		{
			if(i == 1)
				cout << std::setw(width) << setfill(' ') << std::internal 
			<< "序号" << "|";
			else
				cout << std::setw(width) << setfill(' ') << std::internal 
			<< i - 1 << "|";
		}

		for(int j=1; j<=col; j++) //列数
		{
			string item;
			if((i - 1) * col + j <= count)
				getline(buffer, item);
			else
				item = "";
			cout << std::setw(width) << setfill(' ') << std::internal 
				<< item << "|";
		}
		cout << endl;
	}

	for(int i=0; i<total_length;i++)
		cout << "-";
	cout << endl;
}

