/*
To do:
1. Сдлеать парсер который будет разделять строку на слова и записывать в контейнер +
2. Сделать функцию Command validCmd() которая определяет тип команды
3. Для каждой команды должена быть своя функция нахождения ошибок 
4. Сделать возможность сохранять и загружать информацию с файлов

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


enum COMMANDS {PUSH = 0, GET, LIST, DELETE, QUIT, CLEAR};
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
	Command(0, "list",	 LIST),
	Command(1, "delete", DELETE),
	Command(0, "quit",	 QUIT),
	Command(0, "cls",	 CLEAR)
};

Data ArgsToMainData(string name, string body);
void push(Data d, vector<Data>* md);

//функии для работы с файлом
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
			//заменить конструкцию вывода ошибки на что-то компактное и разумное

			
			if (curCMD.argsNum < curCMD.args.size())			//	проверка количества аргументов
			{
				error_indicator = MANY_ARGS;	// много аргументов
				errorMessage(error_indicator);
				continue;
			}
			else if (curCMD.argsNum > curCMD.args.size())
			{
				error_indicator = FEW_ARGS;	// мало аргументов
				errorMessage(error_indicator);
				continue;
			}
			push(ArgsToMainData(curCMD.args[0], curCMD.args[1]), &mainData);
			system("cls");
			
			break;
		case GET:
			//если аргумент не число, в i записываеться 0, wtf ??
			cout << "get\n";
			if (curCMD.argsNum < curCMD.args.size())			//	проверка количества аргументов
			{
				error_indicator = MANY_ARGS;	// много аргументов
				errorMessage(error_indicator);
				continue;
			}
			else if (curCMD.argsNum > curCMD.args.size())
			{
				error_indicator = FEW_ARGS;	// мало аргументов
				errorMessage(error_indicator);
				continue;
			}

			system("cls");
			unsigned int i;
			std::istringstream (curCMD.args[0]) >> i;
			if (i <= 0)
			{
				cerr << "ERROR: Argument have to be > 0\n";
				continue;
			}

			if (i <= mainData.size())
			{
				cout << "/////////////////////////\n";
				cout << mainData[i - 1].getName() << endl << endl;
				cout << mainData[i - 1].getDefinition() << endl;
			}
			else
				cout << "There is no element under this number\n";
			
			break;
		case LIST:
			//добавить вывод ошибок
			//добавить обработку ошибок
			if (curCMD.argsNum != curCMD.args.size())			//	проверка количества аргументов
			{
				error_indicator = MANY_ARGS;
				errorMessage(error_indicator);
				continue;
			}
			for (int i = 0; i < mainData.size(); ++i)
			{
				cout << '[' << i << ']' << mainData[i].getName() << endl;
			}
			break;
		case DELETE:
			cout << "delete\n";
			break;
		case QUIT:
			return 0;
			break;
		case CLEAR:
			system("cls");
			break;
		default:
			cerr << "ERROR: UNKNOWN_COMMAND\n";
		}


	}


	return 0;
}

void errorMessage(int e)
{
	//временная конструкция для вывода ошибок
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
void pushInFile(string f_name, string name, string body)	// 2-й аргумент - название, 3-й аргумент - определение
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

string getStr(string f_name)	// для работы с файловым потоком
{
	string str = "";
	ifstream fin;
	fin.open(f_name);
	while (getline(fin, str)) { cout << str << endl; }
	fin.close();
	return str;	
}
string getStr()		// для работы с консольным потоком
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
	// при нахождении совпадения в списке доступных команд - возвращает объект команды
	//если совпадение не найдено - возвращаеться пустой объект команды
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
	//возвращает индикатор ошибки объекта команды 
	if (cmd.name == "")
		return UNKNOWN_CMD;		// неизвестная команда
	for (Command c : cmds)
	{
		if (c.argsNum < cmd.args.size())
			return MANY_ARGS;	// много аргументов
		else if (cmd.args.size() != 0 && c.argsNum > cmd.args.size())
			return FEW_ARGS;	// мало аргументов
	}
	return 0;
}
//-----------------------------------------

