#include "tablemaker.h"

int main()
{
	tablemaker a(3);
	a << "a";
	a << "b";
	a << "c";
	a << "d";
	a << "e";
	a << "f";
	a << "g";
	a << "h";
	a << "i";
	a.put(true);
}
