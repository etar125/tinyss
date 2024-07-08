#pragma once

#include <string>
#include <vector>

#include "token.hpp"

struct TSSException
{
	int index = -1;
	tkn token;
	std::string message;

	TSSException();
	TSSException(int _index, tkn _token, std::string _message);
};

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
	// static std::string gcall;
	// static bool called;
	// static bool work;

	int find(std::string name);
	void set(std::string name, std::string val);
	std::string get(std::string name);
	void del(std::string name);
	void gfunc(std::string name);
	TSSException docode(std::vector<std::string> code, bool debug = false);

	tss();
};
