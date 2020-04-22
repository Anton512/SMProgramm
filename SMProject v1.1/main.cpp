/*
To do:
1. Сдлеать парсер который будет разделять строку на слова и записывать в контейнер +
2. Сделать функцию Command validCmd() которая определяет тип команды

*/

#define UNKNOWN_CMD -1
#define FEW_ARGS -2
#define MANY_ARGS -3

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;


enum COMMANDS {PUSH = 0, GET, LIST, DELETE, QUIT};
enum ERRORS { ARGS_COUNT_ERROR = 0, UNCORRECT_ARGS_TYPE };

struct Command
{
public:
	Command() {};
	Command(unsigned int _argsNum, string _name, enum COMMANDS _e_cmd) 
	{
		argsNum = _argsNum;
		name = _name;
		e_cmd = _e_cmd;
	};
	
	unsigned int argsNum = 0;
	string name = "";
	vector<string> args;
	enum COMMANDS e_cmd;
};

vector<Command> cmds =  
{
	Command(2, "push",	 PUSH),
	Command(1, "get",	 GET),
	Command(1, "list",	 LIST),
	Command(1, "delete", DELETE),
	Command(0, "quit",	 QUIT),
};

vector<string> parse(string str);
string getStr();
Command initCmd(vector<string> _words, vector<Command> _cmds);
int validateCmd(Command _cmd, vector<Command> _cmds);
int main()
{
	
	Command curCMD;
	bool isClose = false;
	string str = "";
	while (!isClose)
	{
		//getline(cin, str);
		str = getStr();
		vector<string> words = parse(str);
		curCMD = initCmd(words, cmds);
		int error_indicator = validateCmd(curCMD, cmds);
		for (string str : words) // для отладки
			cout << str << endl;



		cout  << "Error_indicator: "<< error_indicator << endl;
		cout  << "Command: "<< curCMD.name << endl;
		cout  << "Number of arguments: "<< curCMD.argsNum << endl;
	


	}


	return 0;
}

//---------------Function for pasrse commands and arguments





vector<string> parse(string str)
{
	bool inBrkts = false;  
	bool inWord = false;
	vector<string> words;
	string tmpstr = "";
	char ch; // для простоты отладки
	for (unsigned int i = 0; i < str.size(); i++)
	{
		ch = str[i];
					
		if (str[i] == '"' && !inBrkts)		//----- можно объеденить
		{										
			inBrkts = true;
			continue;
		}
		if (str[i] == '"' && inBrkts)		//----- можно объеденить
		{
			inBrkts = false;
			words.push_back(tmpstr);
			tmpstr = "";
			continue;
		}

		if (!inBrkts)
		{
			if ((str[i] != ' ' && str[i] != '\n') && !inWord)
			{
				inWord = true;
			}
			if ((str[i] == ' ' || str[i] == '\n') && inWord)
			{
				inWord = false;
				words.push_back(tmpstr);
				tmpstr = "";
			}
			
		}
		if (inBrkts || inWord)
			tmpstr += str[i];
		
	}
	return words;
}

string getStr()
//возвращает, полную строку с символом перехода на новую строку 
{
	string str = "";
	char ch = ' ';
	do {
		ch = cin.get();
		str += ch;
	} while (ch != '\n');
	return str;
}

Command initCmd(vector<string> _words, vector<Command> _cmds)
{
	Command cmd;
	for (unsigned int i = 0; i < _cmds.size(); i++)
	{
		if (_words[0] == _cmds[i].name)
		{
			cmd = _cmds[i];
			break;
		}
		else if (i + 1 == _cmds.size())
			return cmd;
	}
	//add arguments 
		for (unsigned int i = 1; i < _words.size(); i++)
			cmd.args.push_back(_words[i]);
	return cmd;
}

int validateCmd(Command cmd, vector<Command> cmds)
{

	if (cmd.name == "")
		return UNKNOWN_CMD;
	for (Command c : cmds)
	{
		if (c.argsNum < cmd.args.size())
			return MANY_ARGS;
		else if (c.argsNum > cmd.args.size())
			return FEW_ARGS;
	}
}
//-----------------------------------------

