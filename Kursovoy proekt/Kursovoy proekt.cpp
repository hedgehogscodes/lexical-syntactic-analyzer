#include<iostream>
#include <setjmp.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "LEX.h"
#include "SYNTAX.h"
#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

int main()
{
	setlocale(LC_ALL, "RUS");
	FILE* fp;
	fopen_s(&fp, "./kurs1.txt", "r");
	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	rewind(fp);
	char* buffer = (char*)malloc(sizeof(char) * lSize);
	fread(buffer, 1, lSize, fp);
	fclose(fp);
	GetToken(buffer, sizeof(char) * lSize);
	int token_id = 0;
	root = create(root);
	current = root;
	if (Program(token_id))
	{
		std::cout << "Синтаксический анализатор: Успех" << std::endl;
		std::cout << "\nДерево\n" << std::endl;
		print_tree(root, "");
	}
	else std::cout << "Синтаксический анализатор: Неудача" << std::endl;
}