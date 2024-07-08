#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "tinyss.hpp"

// Определяем метод с функциями, даже если их нет
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
			"call cout",
		"elif test e 5",
			"op test * 25",
			"call cout",
		"else",
			"gpushb $not",
			"gcall testcout",
		"end",
		"exit",
		":cout",
			"gpushb $test",
			"gcall testcout",
		"ret"
	};
	TSSException te = a.docode(code);
	if(te.index != -1)
	{
		std::cout << "TSSException: index " << std::to_string(te.index) << " token:[ ";
		if(te.token.type == tkntp::com) std::cout << "command, ";
		else if(te.token.type == tkntp::var) std::cout << "variable, ";
		else if(te.token.type == tkntp::lab) std::cout << "label, ";
		else if(te.token.type == tkntp::val) std::cout << "value, ";
		std::cout << "\"" << te.token.val << "\" ]\n" << te.message << std::endl;
	}
	code[0] = "define test 5";
	te = a.docode(code);
	if(te.index != -1)
	{
		std::cout << "TSSException: index " << std::to_string(te.index) << " token:[ ";
		if(te.token.type == tkntp::com) std::cout << "command, ";
		else if(te.token.type == tkntp::var) std::cout << "variable, ";
		else if(te.token.type == tkntp::lab) std::cout << "label, ";
		else if(te.token.type == tkntp::val) std::cout << "value, ";
		std::cout << "\"" << te.token.val << "\" ]\n" << te.message << std::endl;
	}
	code[0] = "define test 26";
	te = a.docode(code);
	if(te.index != -1)
	{
		std::cout << "TSSException: index " << std::to_string(te.index) << " token:[ ";
		if(te.token.type == tkntp::com) std::cout << "command, ";
		else if(te.token.type == tkntp::var) std::cout << "variable, ";
		else if(te.token.type == tkntp::lab) std::cout << "label, ";
		else if(te.token.type == tkntp::val) std::cout << "value, ";
		std::cout << "\"" << te.token.val << "\" ]\n" << te.message << std::endl;
	}

	// Вывод:
	// etar125
	// etar125
	// etar126
	// Код работает правильно

	 // выполняем код
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
