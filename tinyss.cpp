#include <iostream>
#include <string>
#include <vector>
#include "lexer.hpp"
#include "tinyss.hpp"

#define cur cod[i]
#define cuv vars[i]

std::string tss::gcall = "";
bool tss::called = false;
bool tss::work = false;

varb::varb() {}
varb::varb(std::string _name, std::string _val) { name = _name; val = _val; }

int tss::find(std::string name)
{
	for(int i = 0; i < vars.size(); i++) if(cuv.name == name) return i;
	return -1;
}

void tss::set(std::string name, std::string val)
{
	int i = find(name);
	if(i != -1) cuv.val = val;
	else vars.push_back(varb(name, val));
}

std::string tss::get(std::string name)
{
	int i = find(name);
	if(i != -1) return cuv.val;
	return "null";
}

void tss::del(std::string name)
{
	int i = find(name);
	if(i != -1) vars.erase(vars.begin() + i);
}

/*
void tss::gfunc(std::string name)
{
	if(name == "testcout")
	{
		std::cout << "etar" << stack[0] << std::endl;
	}
}
*/

int tss::docode(vecstr code)
{
	std::vector<tkn> cod = Lexer(code);
	for(int i = 0; i < cod.size(); i++)
	{
		if(cur.type != tkntp::com) return 3;
		else
		{
			if(cur.val == "define")
			{

				std::string name = "";
				std::string val = "";
				i++;
				if(cur.type != tkntp::val) return 3;
				else name = cur.val;
				i++;
				if(cur.type == tkntp::val) val = cur.val;
				else if(cur.type == tkntp::var) val = get(cur.val);
				else return 3;
				set(name, val);
			}
			else if(cur.val == "op")
			{
				std::string name = "";
				std::string val = "";
				i++;
				if(cur.type != tkntp::val) return 3;
				else name = cur.val;
				i++;
				if(cur.type != tkntp::val) return 3;
				i++;
				if(cur.type == tkntp::val) val = cur.val;
				else if(cur.type == tkntp::var) val = get(cur.val);
				else return 3;
				i--;
				if(cur.val == "+") set(name, std::to_string(std::stoi(get(name)) + std::stoi(val)));
				else if(cur.val == "-") set(name, std::to_string(std::stoi(get(name)) - std::stoi(val)));
				else if(cur.val == "*") set(name, std::to_string(std::stoi(get(name)) * std::stoi(val)));
				else if(cur.val == "/") set(name, std::to_string(std::stoi(get(name)) / std::stoi(val)));
				i++;
			}
			else if(cur.val == "del")
			{
				std::string name = "";
				i++;
				if(cur.type != tkntp::val) return 3;
				else name = cur.val;
				del(name);
			}
			else if(cur.val == "goto")
			{
				i++;
				bool findl = false;
				for(int a = 0; a < cod.size(); a++)
				{
					if(cod[a].type == tkntp::lab)
					{
						a++;
						if(cod[a].type != tkntp::val) return 3;
						if(cod[a].val == cur.val) { i = a; findl = true; break; }
					}
				}
				if(!findl) return 3;
			}
			else if(cur.val == "gcall")
			{
				i++;
				//gfunc(cur.val);
				gcall = cur.val;
				called = true;
				stack.clear();
			}
			else if(cur.val == "gpushb")
			{
				std::string val = "";
				i++;
				if(cur.type == tkntp::val) val = cur.val;
				else if(cur.type == tkntp::var) val = get(cur.val);
				else return 3;
				stack.push_back(val);
			}
		}
	}
	if(work) work = false;
	return 0;
}

tss::tss() { };