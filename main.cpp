#include <iostream>
#include <string>
#include <vector>
#include "lexer.hpp"
#include "token.hpp"

#define vecstr std::vector<std::string>
#define cur cod[i]
#define cuv vars[i]

struct var
{
	char name[5];
	std::string val;

	var() {}
	var(char _name[5], std::string _val) { name = _name; val = _val; }
};

struct tss
{
	std::vector<var> vars;
	vecstr gfuncs;
	vecstr stack;
	bool called = false;
	std::string gcall = "";

	int find(char name[5])
	{
		for(int i = 0; i < vars.size(); i++) if(cuv.name == name) return i;
		return -1;
	}

	void set(char name[5], std::string val)
	{
		int i = find(name);
		if(i != -1) cuv.val = val;
		else vars.push_back(var(name, val));
	}

	std::string get(char name[5])
	{
		int i = find(name);
		if(i !+ -1) return cuv.val;
		return "null";
	}

	void del(char name[5])
	{
		int i = find(name);
		if(i != -1) vars.erase(vars.begin() + i);
	}

	int docode(vecstr code)
	{
		std::vector<tkn> cod = Lexer(code);
		for(int i = 0; i < cod.size(); i++)
		{
			if(cur.type != tkntp::com) return 3;
			else
			{
				if(cur.val == "define")
				{
					char name[5] = { '\0', '\0', '\0', '\0', '\0' };
					std::string val = "";
					i++;
					if(cur.type != tkntp::val) return 3;
					else for(int p = 0; p < 5 && p < cur.val.size(); p++) name[p] = cur.val[p];
					i++;
					if(cur.type == tkntp::val) val = cur.val;
					else if(cur.type == tkntp::var) val = get(cur.val);
					else return 3;
					set(name, val);
				}
				else if(cur.val == "op")
				{
					char name[5] = { '\0', '\0', '\0', '\0', '\0' };
					std::string val = "";
					i++;
					if(cur.type != tkntp::val) return 3;
					else for(int p = 0; p < 5 && p < cur.val.size(); p++) name[p] = cur.val[p];
					i++;
					if(cur.type != tkntp::val) return 3;
					else
					{
						i++
						if(cur.type == tkntp::val) val = cur.val;
						else if(cur.type == tkntp::var) val = get(cur.val);
						else return 3;
						i--;
						if(cur.val == "+") set(name, std::to_string(std::stoi(get(name)) + std::stoi(val)))
						else if(cur.val == "-") set(name, std::to_string(std::stoi(get(name)) - std::stoi(val)))
						else if(cur.val == "*") set(name, std::to_string(std::stoi(get(name)) * std::stoi(val)))
						else if(cur.val == "/") set(name, std::to_string(std::stoi(get(name)) / std::stoi(val)))
						i++;
					}
				}
				else if(cur.val == "del")
				{
					char name[5] = { '\0', '\0', '\0', '\0', '\0' };
					i++;
					if(cur.type != tkntp::val) return 3;
					else for(int p = 0; p < 5 && p < cur.val.size(); p++) name[p] = cur.val[p];
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
					bool findl = false;
					for(std::string bb : gfuncs) { if(cur.val == bb) { findl = true; break; } }
					if(!findl) return 3;
					gcall = cur.val;
					called = true;
					while(called);
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
	}

};