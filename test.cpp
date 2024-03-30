#include <iostream>
#include <string>
#include <vector>

#include "tinyss.hpp"

int main()
{
	std::vector<std::string> code =
	{
		"define test 125",
		"gpushb $test",
		"gcall testcout",
		
	};

	tss a;
	a.docode(code);
	return 0;
}