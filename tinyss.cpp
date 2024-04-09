#include <iostream>
#include <string>
#include <vector>
#include "lexer.hpp"
#include "tinyss.hpp"

#define cur cod[i]
#define cuv vars[i]

// std::string tss::gcall = "";
// bool tss::called = false;
// bool tss::work = false;

TSSException::TSSException() { TSSException::index = -1; }
TSSException::TSSException(int _index, tkn _token) { TSSException::index = _index; TSSException::token = _token; }

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

TSSException tss::docode(vecstr code)
{
	bool iff = false;
	std::vector<tkn> cod = Lexer(code);
	// for(tkn ch : cod)
	// {
	// 	switch(ch.type)
	// 	{
	// 		case tkntp::com: std::cout << "Type: Command "; break;
	// 		case tkntp::var: std::cout << "Type: Variable "; break;
	// 		case tkntp::val: std::cout << "Type: Value "; break;
	// 		case tkntp::lab: std::cout << "Type: Label "; break;
	// 	}
	// 	std::cout << "Value: " << ch.val << std::endl;
	// }
	for(int i = 0; i < cod.size(); i++)
	{
		if(cur.type != tkntp::com) return TSSException(i, cur);
		else
		{
			if(cur.val == "define")
			{

				std::string name = "";
				std::string val = "";
				i++;
				if(cur.type != tkntp::val) return TSSException(i, cur);
				else name = cur.val;
				i++;
				if(cur.type == tkntp::val) val = cur.val;
				else if(cur.type == tkntp::var) val = get(cur.val);
				else return TSSException(i, cur);
				set(name, val);
			}
			else if(cur.val == "op")
			{
				std::string name = "";
				std::string val = "";
				i++;
				if(cur.type != tkntp::val) return TSSException(i, cur);
				else name = cur.val;
				i++;
				if(cur.type != tkntp::val) return TSSException(i, cur);
				i++;
				if(cur.type == tkntp::val) val = cur.val;
				else if(cur.type == tkntp::var) val = get(cur.val);
				else return TSSException(i, cur);
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
				if(cur.type != tkntp::val) return TSSException(i, cur);
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
						if(cod[a].type != tkntp::val) return TSSException(i, cur);
						if(cod[a].val == cur.val) { i = a; findl = true; break; }
					}
				}
				if(!findl) return TSSException(i, cur);
			}
			else if(cur.val == "gcall")
			{
				i++;
				//gfunc(cur.val);
				gfunc(cur.val);
				stack.clear();
			}
			else if(cur.val == "gpushb")
			{
				std::string val = "";
				i++;
				if(cur.type == tkntp::val) val = cur.val;
				else if(cur.type == tkntp::var) val = get(cur.val);
				else return TSSException(i, cur);
				stack.push_back(val);
			}
			else if(cur.val == "else")
			{
				if(iff)
				{
					iff = false;
					for(int a = 0; a < cod.size(); a++)
					{
						if(cod[a].type == tkntp::com && cod[a].val == "end")
						{
							i = a;
							break;
						}
					}
				}
			}
			else if(cur.val == "if" || cur.val == "elif")
			{
				if(iff)
				{
					iff = false;
					for(int a = 0; a < cod.size(); a++)
					{
						if(cod[a].type == tkntp::com && cod[a].val == "end")
						{
							i = a;
							break;
						}
					}
				}
				else
				{
					std::string first = "";
					std::string val = "";
					bool is = false;
					i++;
					if(cur.type != tkntp::val) return TSSException(i, cur);
					first = get(cur.val);
					i++; i++;
					if(cur.type == tkntp::val) val = cur.val;
					else if(cur.type == tkntp::var) val = get(cur.val);
					else return TSSException(i, cur);
					i--;
					if(cur.type != tkntp::val) return TSSException(i, cur);
					if(cur.val == "e" && first == val) is = true;
					else if(cur.val == "ne" && first != val) is = true;
					else if(cur.val == "g" && std::stoi(first) > std::stoi(val)) is = true;
					else if(cur.val == "ge" && std::stoi(first) >= std::stoi(val)) is = true;
					else if(cur.val == "ng" && !(std::stoi(first) > std::stoi(val))) is = true;
					else if(cur.val == "nge" && !(std::stoi(first) >= std::stoi(val))) is = true;
					else if(cur.val == "l" && std::stoi(first) < std::stoi(val)) is = true;
					else if(cur.val == "le" && std::stoi(first) <= std::stoi(val)) is = true;
					else if(cur.val == "nl" && !(std::stoi(first) < std::stoi(val))) is = true;
					else if(cur.val == "nle" && !(std::stoi(first) <= std::stoi(val))) is = true;
					i++;
					if(!is)
					{
						for(int a = 0; a < cod.size(); a++)
						{
							if(cod[a].type == tkntp::com && (cod[a].val == "elif" || cod[a].val == "else" || cod[a].val == "end"))
							{
								i = a--;
								break;
							}
						}
					}
					else iff = true;
				}
			}
		}
	}
	return TSSException();
}

tss::tss() { };