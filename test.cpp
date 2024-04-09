#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "tinyss.hpp"

// Определяем функции, даже если их нет
void tss::gfunc(std::string name)
{
	if(name == "testcout") std::cout << "etar" << tss::stack[0] << std::endl;
}

int main()
{
	tss a;
	std::vector<std::string> code = // наш код
	{
		"define test 25",
		"define not 126",
		"if test e 25",
		"op test * 5",
		"gpushb $test",
		"gcall testcout",
		"else",
		"gpushb $not",
		"gcall testcout",
		"end"
	};
	TSSException b = a.docode(code); // выполняем код
	/* Данный код нужен для отладки
	std::cout << "Index: " << std::to_string(b.index);
	switch(b.token.type)
	{
		case tkntp::com: std::cout << " Type: Command "; break;
		case tkntp::var: std::cout << " Type: Variable "; break;
		case tkntp::val: std::cout << " Type: Value "; break;
		case tkntp::lab: std::cout << " Type: Label "; break;
	}
	std::cout << "Value: " << b.token.val << std::endl;
	*/
	return 0;
}