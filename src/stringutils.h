#include <vector>
#include <string>

using namespace std;

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

