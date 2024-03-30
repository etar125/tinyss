#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "tinyss.hpp"

tss a;

void run()
{
	std::vector<std::string> code =
	{
		"define test 125",
		"gpushb $test",
		"gcall testcout",
		
	};
	a.docode(code);
}

int main()
{
	std::thread ads(run);
	ads.detach();
	a.work = true;
	while(a.work) { if(a.called) { if (a.gcall == "testcout") { std::cout << "etar" << a.stack[0] << std::endl; a.called = false; } } }
	return 0;
}