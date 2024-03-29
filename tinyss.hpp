#include <string>
#include <vector>

struct var
{
	char name[5];
	std::string val;

	var();
	var(char _name[5], std::string _val);
};

struct tss
{
	std::vector<var> vars;
	std::vector<std::string> gfuncs;
	std::vector<std::string> stack;
	bool called;
	std::string gcall;

	int find(char name[5]);
	void set(char name[5], std::string val);
	std::string get(char name[5]);
	void del(char name[5]);
	int docode(std::vector<std::string> code);
};