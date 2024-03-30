#pragma once

#include <string>
#include <vector>

struct varb
{
	std::string name;
	std::string val;

	varb();
	varb(std::string _name, std::string _val);
};

struct tss
{
	std::vector<varb> vars;
	std::vector<std::string> stack;

	int find(std::string name);
	void set(std::string name, std::string val);
	std::string get(std::string name);
	void del(std::string name);
	void gfunc(std::string name);
	int docode(std::vector<std::string> code);

	tss();
};