/*
To do:
1. ������� ������ ������� ����� ��������� ������ �� ����� � ���������� � ��������� +
2. ������� ������� Command validCmd() ������� ���������� ��� �������
3. ��� ������ ������� ������� ���� ���� ������� ���������� ������

*/


#define UNKNOWN_CMD -1
#define FEW_ARGS -2
#define MANY_ARGS -3

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "Data.h"
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

Data ArgsToMainData(string name, string body);
void push(Data d, vector<Data>* md);

//������ ��� ������ � ������
string fileName = "Data.txt";
string getStr(string f_name);

vector<string> parse(string str);
string getStr();
Command initCmd(vector<string> _words, vector<Command> _cmds);
int validateCmd(Command _cmd, vector<Command> _cmds);
void errorMessage(int e);
int main()
{
	vector<Data> mainData;		
	Command curCMD;
	bool isClose = false;
	string str = "";
	while (!isClose)
	{
		//getline(cin, str);
		str = getStr();
		vector<string> words = parse(str);
		if (words.empty())	
			continue;
		curCMD = initCmd(words, cmds);
		int error_indicator = validateCmd(curCMD, cmds);

		
		/*if (error_indicator)
		{
			errorMessage(error_indicator);
			continue;
		}*/

		switch (curCMD.e_cmd)
		{
		case PUSH:
			//�������� ����������� ������ ������ �� ���-�� ���������� � ��������
			if (curCMD.argsNum < curCMD.args.size())
			{
				error_indicator = MANY_ARGS;	// ����� ����������
				errorMessage(error_indicator);
				continue;
			}
			else if (curCMD.argsNum > curCMD.args.size())
			{
				error_indicator = FEW_ARGS;	// ���� ����������
				errorMessage(error_indicator);
				continue;
			}
			push(ArgsToMainData(curCMD.args[0], curCMD.args[1]), &mainData);
			system("cls");
			for (int i = 0; i < mainData.size(); ++i)
			{
				cout  << '[' << i << ']' << mainData[i].getName() << endl;
			}
			break;
		case GET:
			cout << "get\n";
			break;
		case LIST:
			cout << "list\n";
			break;
		case DELETE:
			cout << "delete\n";
			break;
		case QUIT:
			return 0;
			break;
		default:
			cerr << "ERROR: UNKNOWN_COMMAND\n";
		}


	}


	return 0;
}

void errorMessage(int e)
{
	//��������� ����������� ��� ������ ������
	switch (e)
	{
	case UNKNOWN_CMD:
		cerr << "ERROR: UNKNOWN_COMMAND\n";
		return;
	case MANY_ARGS:
		cerr << "ERROR: TO MUCH ARGUMENTS\n";
		return;
	case FEW_ARGS:
		cerr << "ERROR: FEW ARGUMENTS\n";
		return;
	default:
		return;
	}
}

Data ArgsToMainData(string name, string body)
{
	return Data(name, body);
}

void push(Data d, vector<Data>* md)
{
	md->push_back(d);
}
void pushInFile(string f_name, string name, string body)	// 2-� �������� - ��������, 3-� �������� - �����������
{
	ofstream fout;
	fout.open(f_name, ios_base::app);
	
}


//---------------Function for pasrse commands and arguments

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
					
		if (str[i] == '"' && !inBrkts)		//----- ����� ����������
		{										
			inBrkts = true;
			continue;
		}
		if (str[i] == '"' && inBrkts)		//----- ����� ����������
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

string getStr(string f_name)	// ��� ������ � �������� �������
{
	string str = "";
	ifstream fin;
	fin.open(f_name);
	while (getline(fin, str)) { cout << str << endl; }
	fin.close();
	return str;	
}
string getStr()		// ��� ������ � ���������� �������
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
	// ��� ���������� ���������� � ������ ��������� ������ - ���������� ������ �������
	//���� ���������� �� ������� - ������������� ������ ������ �������
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
	int n_cmd = cmd.argsNum;
	//���������� ��������� ������ ������� ������� 
	if (cmd.name == "")
		return UNKNOWN_CMD;		// ����������� �������
	for (Command c : cmds)
	{
		if (c.argsNum < cmd.args.size())
			return MANY_ARGS;	// ����� ����������
		else if (cmd.args.size() != 0 && c.argsNum > cmd.args.size())
			return FEW_ARGS;	// ���� ����������
	}
	return 0;
}
//-----------------------------------------

