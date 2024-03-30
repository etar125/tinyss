#include <string>

enum tkntp // token type
{
	com, var, lab, val
};

struct tkn // token
{
	tkntp type;
	std::string val;

	tkn() {}
	tkn(tkntp _type) { type = _type; }
	tkn(tkntp _type, std::string _val) { type = _type; val = _val; }

};