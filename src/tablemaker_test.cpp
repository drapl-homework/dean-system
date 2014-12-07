#include "tablemaker.h"

int main()
{
	tablemaker a(3);
	a << L"a";
	a << L"b";
	a << L"c";
	a << L"d";
	a << L"e";
	a << L"f";
	a << L"g";
	a << L"h";
	a << L"i";
	a.put(true);
}
