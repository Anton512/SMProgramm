/*
To do:
1. ������� ������ ������� ����� ��������� ������ �� ����� � ���������� � ��������� +
2. ������� ������� Command validCmd() ������� ���������� ��� �������
*/


#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


enum COMMANDS {PUSH = 0, GET, LIST, DELETE, QUIT, ERROR_CMD };

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
	
	unsigned int argsNum;
	string name;
	vector<string> args;
	enum COMMANDS e_cmd;
};

vector<Command> cmds =
{
	Command(2, "push", PUSH),
	Command(1, "get",  GET),
	Command(1, "list", LIST),
	Command(1, "delete", DELETE),
	Command(0, "quit", QUIT),
	Command(0, "error", ERROR_CMD),
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
		for (string str : words) // ��� �������
			cout << str << endl;

	cout  << "Error_indicator: "<< error_indicator << endl;
	cout  << "Command: "<< curCMD.name << endl;
	cout  << "Number of arguments: "<< curCMD.argsNum << endl;

	}


	return 0;
}


vector<string> parse(string str)
{
	bool inBrkts = false;
	bool inWord = false;
	vector<string> words;
	string tmpstr = "";
	char ch; // ��� �������� �������
	for (unsigned int i = 0; i < str.size(); i++)
	{
		ch = str[i];
					
		if (str[i] == '"' && !inBrkts)
		{
			inBrkts = true;
			continue;
		}
		if (str[i] == '"' && inBrkts)
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
//����������, ������ ������ � �������� �������� �� ����� ������ 
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
		else if (_cmds.size())
			cmd = _cmds[ERROR_CMD];
	}
	//add arguments 
	if (cmd.e_cmd != ERROR_CMD)
	{
		for (unsigned int i = 1; i < _words.size(); i++)
			cmd.args.push_back(_words[i]);
	}
	return cmd;
}

int validateCmd(Command _cmd, vector<Command> _cmds)
{
	for (Command c : _cmds)
	{
		if (c.argsNum == _cmd.args.size())
			return 1;
		else
			return 0;
	}
	return -1;
}