#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "token.hpp"

#define vecstr std::vector<std::string>

vecstr split(std::string str, char delim)
{
	vecstr result;
	std::string temp;
	for(char s : str)
	{
		if(s != delim) temp += s;
		else { result.push_back(temp); temp = ""; } 
	}
	result.push_back(temp); temp = "";
	return result;
}

std::string trip(std::string str)
{

	int i = 0;
	while(str[i] == ' ' || str[i] == '\t') i++;
	return str.substr(i);
}

std::vector<tkn> Lexer(vecstr code)
{
	std::vector<tkn> result;
	vecstr res;
	bool com = true;
	for(std::string s : code)
	{
		std::string st = trip(s);
		if(st != "")
		{
			res = split(trip(st), ' ');
			for(int i = 0; i < res.size(); i++)
			{
				if(res[i][0] == ';') break;
				else if(res[i][0] == '$') result.push_back(tkn(tkntp::var, res[i].substr(1)));
				else if(res[i][0] == ':') { result.push_back(tkn(tkntp::lab, res[i].substr(1))); }
				else if(com) { result.push_back(tkn(tkntp::com, res[i])); com = false; }
				else result.push_back(tkn(tkntp::val, res[i]));
			}
			com = true;
		}
	}
	return result;
}