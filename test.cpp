#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "tinyss.hpp"

tss a;

void run()
{
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
}

int main()
{
	std::thread ads(run);
	ads.detach(); // Отсоединяем TSS от текущего потока, чтобы проверять функции
	a.work = true; // ставим, что программа работает
	// Пока программа работает, проверяем вызвана ли функция, если да, то проверяем имя и выполняем нужный код
	while(a.work) { if(a.called) { if (a.gcall == "testcout") { std::cout << "etar" << a.stack[0] << std::endl; a.called = false; } } }
	return 0;
}