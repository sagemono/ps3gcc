// { dg-do run }
// { dg-xfail-if "256K size limit" { "spu-*-*" } "*" "" }
// { dg-options -fomit-frame-pointer }

#include <iostream>

class Bug
{
};

int throw_bug()
{
	throw Bug();

	return 0;
}

int main()
{
	try {
		std::cout << throw_bug();
	} catch (Bug bug) {
	};
	
	return 0;
}
