#include <vector>
#include <string>
#include <sstream>
#include <cwchar>

using namespace std;

vector<wstring> split(const wstring &s, wchar_t delim) {
	vector<wstring> elems;
	wstringstream ss(s);
	wstring item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

wstring trim(const wstring &s)
{
	wstring s2 = s;
	s2.erase(remove(s2.begin(), s2.end(), L'\t'), s2.end());
	return s2;
}

