#include <fstream>
#include <string>

using namespace std;

template < class T >
class db
{
	public:
		db(string filename);
		vector<T>& getData();
		void putData();
		vector<T> buffer;
	private:
		string fname;
};

template < class T >
db<T>::db(string filename)
{
	fname = filename;
	ifstream infile(fname.c_str(), ios::in | ios::binary);
	if(infile.fail()) //文件不存在
	{
		infile.close();
		ofstream outfile(fname.c_str(), ios::out | ios::trunc | ios::binary); //创建文件
		outfile.close();
		return;
	}

	/*获取数据个数*/
	infile.seekg (0, infile.end);
	int length = infile.tellg() / sizeof(T);
	infile.seekg (0, infile.beg);

	for(int i=0;i<length;i++)
	{
		T tmp;
		char* cptr = reinterpret_cast<char *>(&tmp);
		infile.read(cptr, sizeof(T));
		buffer.push_back(tmp);
	}
}

template < class T >
vector<T>& db<T>::getData()
{
	return buffer;
}

template < class T >
void db<T>::putData()
{
	ofstream outfile(fname.c_str(), ios::out | ios::trunc | ios::binary); //打开文件
	for(typename vector<T>::iterator i = buffer.begin(); i!=buffer.end(); i++)
	{
		T tmp = *i;
		char* cptr = reinterpret_cast<char *>(&tmp);
		outfile.write(cptr, sizeof(T));
	}
}
