#pragma once
#include <locale.h>
#include <iostream>
#include <fstream>
#include <setjmp.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
/*Структура хранящая всю информацию о токене*/
struct Tokens
{
	std::string token;
	int type;
};
Tokens token_name;
std::vector <Tokens> token_vector;
int count_token;
/*Идентификатор, Число, Клюевое слово, Разделитель, Фигурные скобки, Тип переменной,Ошибка*/
const char* types[]{ "Идентификатор", "Число", "Ключевое слово", "Разделитель", "Тип переменной","Арифметическая операция","Оператор сравнения","Логический оператор","Специальный символ","Строка","Ошибка" };
enum Type_Token { ID, NUMBER, KEYWORD, DELIMITER, TYPE, ARIPHMETIC, OPERATOR, LOGIC, SPEC, STR, ERROR };

const char* out_file_path = "./tokens.txt";
FILE* out_file;
std::ofstream out;

void WriteCurrTknToFile(int count_token) {
	out.open("./tokens.txt");
	for (int i = 0; i <= count_token; i++) {

		out << "ID:"<< i << "  VALUE:" << token_vector[i].token << "            TYPE:" << types[token_vector[i].type] << std::endl;
	}
	out.close();
}


void Skip_Space(char* buffer, int& i)
{
	while (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n' || buffer[i] == '\r' || buffer[i] == '\0')
	{
		i++;
	}
}

bool Check_Str(std::string str, const char* alf[], int len)
{
	for (int i = 0; i < len; i++)
	{
		if (!str.compare(alf[i])) return true;
	}
	return false;
}

bool IsSpecials(char* buffer, int& i)
{
	const char* alf_special[]{ ".",",","(",")","{","}",";","[","]","+","-","*","/","=","&&","||","!","!=","==",">","<","<=",">=","#" };
	const char* alf_ariphmetic_operation[]{ "+","-","*","/" };
	const char* alf_logic_operation[]{ "&&","||","!" };
	const char* alf_operator[]{ "!=","==",">","<","<=",">=" };
	const char* alf_delimiter[]{ ";" };
	std::string token = "";
	token = token + buffer[i];
	std::string tmp = "";
	tmp = tmp + buffer[i + 1];
	if (!token.compare(tmp))
	{
		token = token + buffer[i + 1];
		i++;
	}
	if (Check_Str(token, alf_special, sizeof(alf_special) / sizeof(alf_special[0])))
	{
		if (Check_Str(token, alf_delimiter, sizeof(alf_delimiter) / sizeof(alf_delimiter[0]))) token_name.type = DELIMITER;
		else if (Check_Str(token, alf_ariphmetic_operation, sizeof(alf_ariphmetic_operation) / sizeof(alf_ariphmetic_operation[0]))) token_name.type = ARIPHMETIC;
		else if (Check_Str(token, alf_operator, sizeof(alf_operator) / sizeof(alf_operator[0]))) token_name.type = OPERATOR;
		else if (Check_Str(token, alf_logic_operation, sizeof(alf_logic_operation) / sizeof(alf_logic_operation[0]))) token_name.type = LOGIC;
		else token_name.type = SPEC;
		token_name.token = token;
		token_vector.push_back(token_name);
		return true;
	}
	return false;
}
bool IsWord(char* buffer, int& i)
{
	const char* alf_key[]{ "Module", "Main", "End", "Dim", "As", "Integer", "String","Boolean", "Double","True", "False",
							"Structure","Sub", "Function",
							"If", "Then", "Else", "Elseif", "Select", "Case", 
							"For", "Next", "To", "While", "Do", "Loop",
							"WriteLine", "ReadLine" };
	const char* alf_type[]{ "Boolean","Integer","Double", "String" };
	std::string token = "";
	token = token + buffer[i];
	i++;
	while ((isalpha(buffer[i]) || isdigit(buffer[i])))
	{	
		token = token + buffer[i];
		i++;
		if ((buffer[i] == ' ') || (buffer[i] == '(') || (buffer[i] == ')') || (buffer[i] == '\0') || (buffer[i] == '\n') || (buffer[i] == -51)) {
			if (Check_Str(token, alf_key, sizeof(alf_key) / sizeof(alf_key[0])))
			{
				if (Check_Str(token, alf_type, sizeof(alf_type) / sizeof(alf_type[0]))) token_name.type = TYPE;
				else token_name.type = KEYWORD;
				token_name.token = token;
				token_vector.push_back(token_name);
				return true;
			}
		}
	}
	token_name.token = token;
	token_name.type = ID;
	token_vector.push_back(token_name);
	return true;
}

bool IsString(char* buffer, int& i, int len)
{
	std::string token = "";
	token = token + buffer[i];
	std::string tmp = "";
	i++;
	tmp = tmp + buffer[i];
	while (tmp.compare("\""))
	{
		if (i == len - 1)
		{
			token_name.token = token;
			return false;
		}
		token = token + buffer[i];
		i++;
		tmp = "";
		tmp = tmp + buffer[i];
	}
	token = token + buffer[i];
	token_name.token = token;
	token_name.type = STR;
	token_vector.push_back(token_name);
	return true;
}

bool IsNumber(char* buffer, int& i, int len)
{
	std::string token = "";
	int count = 0;
	while (isdigit(buffer[i]) || buffer[i] == '.')
	{
		if (buffer[i] == '.' && count == 0) count++;
		else if (buffer[i] == '.' && count != 0)
		{
			token_name.token = token;
			return false;
		}
		token = token + buffer[i];
		i++;
	}
	token_name.token = token;
	token_name.type = NUMBER;
	token_vector.push_back(token_name);

	return true;
}

void GetToken(char* buffer, int len)
{
	for (int i = 0; i < len; i++)
	{
		Skip_Space(buffer, i);
		if (isalpha(buffer[i]))
		{
			IsWord(buffer, i);
		}
		if (isdigit(buffer[i]))
		{
			if (!IsNumber(buffer, i, len))
			{
				token_name.type = ERROR;
				token_vector.push_back(token_name);
				break;
			}
			i--;
		}
		std::string tmp = "";
		tmp = tmp + buffer[i];
		if (!tmp.compare("\""))
		{
			if (!IsString(buffer, i, len))
			{
				token_name.type = ERROR;
				token_vector.push_back(token_name);
				break;
			}
		}
		IsSpecials(buffer, i);
	}

	WriteCurrTknToFile(token_vector.size() - 1);
}
