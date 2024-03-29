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
}

std::string trip(std::string str)
{
	int i = 0;
	while(str[i] != ' ' || str[i] != '\t') i++;
	return str.substr(i);
}

std::vector<tkn> Lexer(vecstr code)
{
	std::vector<tkn> result;
	vecstr res;
	for(std::string s : code)
	{
		res = split(trip(s), ' ');
		bool com = true;
		for(int i = 0; i < res.size(); i++)
		{
			if(res[i][0] == ';') break;
			else if(res[i][0] == '$') result.push_back(tkn(tkntp::val, res[i].substr(1)));
			else if(res[i][0] == ':') result.push_back(tkn(tkntp::lab, res[i].substr(1)));
			else if(com) result.push_back(tkn(tkntp::com, res[i]));
			else result.push_back(tkn(tkntp::val, res[i]));
		}
	}
	return 0;
}