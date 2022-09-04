#pragma once
//Дерево
typedef struct tree
{
	int token_id;
	struct tree* left;
	struct tree* right;
} node;

struct tree* root = NULL;
struct tree* current = NULL;
struct tree* prev = NULL;
struct tree* logicprev = NULL;
struct tree* funcprev = NULL;
struct tree* cicleprev = NULL;
struct tree* lastParse = NULL;
struct tree* tmpt = NULL;
int lastEnd = 0;
int lastEndFunc = 0;

node* create(node* root)
{
	node* tmp = (struct tree*)malloc(sizeof(node));
	tmp->token_id = 0;
	tmp->left = tmp->right = NULL;
	root = tmp;
	return root;
}

node* add(node* root, int key, int list)
{
	node* root2 = root, * root3 = NULL;
	root3 = root2;
	node* tmp = (struct tree*)malloc(sizeof(node));
	tmp->token_id = key;
	tmp->left = NULL;
	tmp->right = NULL;
	if (list == 0) root3->left = tmp;
	else root3->right = tmp;
	return root;
}

void print_tree(struct tree* r, std::string indent)
{
	if (!r) return;
	if (r->right != NULL)
	{
		indent = indent + "   ";
		std::cout << indent << token_vector[r->right->token_id].token << std::endl;
		print_tree(r->right, indent);
		indent.resize(indent.size() - 3);
	}
	if (r->left != NULL)
	{
		if (r->left->token_id == -1) std::cout << "\n" << std::endl;
		else std::cout << indent << token_vector[r->left->token_id].token << std::endl;
		print_tree(r->left, indent);
	}
}
int branch_tree = 0;
//Конец дерева

bool IF(int& token_id, int last);
bool Parser(int& first, int last);
bool Cicle(int& token_id, int last);
bool Cicle(int& token_id, int last);
bool CicleWhile(int& token_id, int last);
bool Assign(int& skip, int Const, int branch_const);
bool VarDec(int& first, int last, int branch);
bool Logic(int token_id);

bool FindEndModule(int& last_pos)
{
	int count_token = token_vector.size();
	while (last_pos < count_token - 1)
	{
		if (token_vector[last_pos].token == "End" && token_vector[last_pos + 1].token == "Module") return true;
		last_pos++;
	}
	return false;
}

bool FindEndWhile(int& last_pos, int& last)
{
	int count_token = last;
	while (last_pos < count_token - 1)
	{
		if (token_vector[last_pos].token == "End" && token_vector[last_pos + 1].token == "While") {
			last = last_pos;
			return true;
		}
		last_pos++;
	}
	return false;
}

bool FindEndStructure(int& last_pos, int& last)
{
	int count_token = last;
	while (last_pos < count_token - 1)
	{
		if (token_vector[last_pos].token == "End" && token_vector[last_pos + 1].token == "Structure") {
			last = last_pos;
			return true;
		}
		last_pos++;
	}
	return false;
}



bool FindEndSub(int& last_pos, int& last)
{
	int count_token = last;
	while (last_pos < count_token - 1)
	{
		if (token_vector[last_pos].token == "End" && token_vector[last_pos + 1].token == "Sub") {
			last = last_pos;
			return true;
		}
		last_pos++;
	}
	return false;
}

bool FindNext(int& last_pos, int& last)
{
	int count_token = last;
	while (last_pos < count_token)
	{
		if (token_vector[last_pos].token == "Next") {
			last = last_pos;
			return true;
		}
		last_pos++;
	}
	return false;
}

bool FindEndFunction(int& last_pos, int& last)
{
	int count_token = last;
	while (last_pos < count_token - 1)
	{
		if (token_vector[last_pos].token == "End" && token_vector[last_pos + 1].token == "Function") {
			last = last_pos;
			return true;
		}
		last_pos++;
	}
	return false;
}

bool FindEndIf(int& last_pos, int& last)
{
	int count_token = last;
	while (last_pos < count_token - 1)
	{
		if (token_vector[last_pos].token == "End" && token_vector[last_pos + 1].token == "If") {
			last = last_pos;
			return true;
		}
		last_pos++;
	}
	return false;
}

bool CicleFor(int& token_id, int last)
{
	int num = token_id;
	if (token_vector[token_id].token == "For" && token_vector[token_id + 1].type == ID && token_vector[token_id + 2].token == "As" && token_vector[token_id + 3].token == "Integer" && token_vector[token_id + 4].token == "=" && (token_vector[token_id + 5].type == ID || token_vector[token_id + 5].type == NUMBER) && token_vector[token_id + 6].token == "To" && (token_vector[token_id + 7].type == ID || token_vector[token_id + 7].type == NUMBER))
	{
		current = add(current, num, 0);
		current = current->left;
		cicleprev = current;
		current = add(current, num + 1, 1);
		current = current->right;
		current = add(current, num + 2, 1);
		current = current->right;
		current = add(current, num + 3, 1);
		current = current->right;
		current = add(current, num + 4, 1);
		current = current->right;
		current = add(current, num + 5, 1);
		current = current->right;
		current = add(current, num + 6, 1);
		current = current->right;
		current = add(current, num + 7, 1);
		current = current->right;
		branch_tree = 0;
		int Token_tmp = token_id;
		lastEndFunc = last;
		int LastBracket = last;
		if (!FindNext(Token_tmp, LastBracket)) return false;
		token_id += 8;
		lastEnd = LastBracket;
		if (Parser(token_id, LastBracket))
		{
			current = cicleprev;
			current = add(current, -1, 0);
			current = current->left;
			last = lastEndFunc;
			token_id = token_id + 1;
			return true;
		}
	}
	else {
		return false;
	}
}
//Цикл while
bool CicleWhile(int& token_id, int last)
{
	int num = token_id;
	if (token_vector[token_id].token == "While" && (token_vector[token_id + 1].type == ID || token_vector[token_id + 1].type == NUMBER) && (token_vector[token_id + 2].type == OPERATOR || token_vector[token_id + 2].type == LOGIC) && (token_vector[token_id + 3].type == ID || token_vector[token_id + 3].type == NUMBER))
	{
		current = add(current, num, 0);
		current = current->left;
		cicleprev = current;
		current = add(current, num + 1, 1);
		current = current->right;
		current = add(current, num + 2, 1);
		current = current->right;
		current = add(current, num + 3, 1);
		current = current->right;
		branch_tree = 0;
		int Token_tmp = token_id;
		lastEndFunc = last;
		int LastBracket = last;
		if (!FindEndWhile(Token_tmp, LastBracket)) return false;
		token_id += 4;
		lastEnd = LastBracket;
		if (Parser(token_id, LastBracket))
		{
			current = cicleprev;
			current = add(current, -1, 0);
			current = current->left;
			last = lastEndFunc;
			token_id = token_id + 2;
			return true;
		}
	}
	else {
		return false;
	}
}

//Цикл
bool Cicle(int& token_id, int last)
{
	if (CicleFor(token_id, last) || CicleWhile(token_id, last)) return true;
	return false;
}
//Цикл for

//Команда вывода
bool Read(int& token_id)
{
	int num = token_id;
	if (token_vector[token_id].token == "ReadLine" && token_vector[token_id + 1].token == "(" && (token_vector[token_id + 2].type == STR || token_vector[token_id + 2].type == NUMBER || token_vector[token_id + 2].type == ID) && token_vector[token_id + 3].token == ")")
	{
		current = add(current, num, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
			prev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			prev = current;
			current = add(current, -1, 0);
		}
		current = add(current, num + 2, 1);
		current = prev;
		branch_tree = 0;
		token_id += 4;
		return true;
	}
	return false;
}

//Команда ввода
bool Write(int& token_id)
{
	int num = token_id;
	if (token_vector[token_id].token == "WriteLine" && token_vector[token_id + 1].token == "(" && (token_vector[token_id + 2].type == STR || token_vector[token_id + 2].type == NUMBER || token_vector[token_id + 2].type == ID) && token_vector[token_id + 3].token == ")")
	{
		current = add(current, num, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
			prev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			prev = current;
			current = add(current, -1, 0);
		}
		current = add(current, num + 2, 1);
		current = prev;
		branch_tree = 0;
		token_id += 4;
		return true;
	}
	return false;
}
//Вызов процедуры
bool CallProc(int& token_id)
{
	int num = token_id;
	if (token_vector[token_id].type == ID && token_vector[token_id + 1].token == "(" &&  token_vector[token_id+2].token == ")")
	{
		current = add(current, num, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
			prev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			prev = current;
			current = add(current, -1, 0);
		}
		current = prev;
		branch_tree = 0;
		token_id += 3;
		return true;
	}
	return false;
}

//Логическое выражение
bool Logic(int token_id)
{
	int tmp_id = token_id;
	if ((token_vector[tmp_id].type == ID || NUMBER) && token_vector[tmp_id + 1].type == OPERATOR && (token_vector[tmp_id + 2].type == ID || NUMBER))
	{
		current = add(current, tmp_id + 1, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
			logicprev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			logicprev = current;
			current = add(current, -1, 0);
		}
		current = add(current, tmp_id, 1);
		current = current->right;
		current = add(current, tmp_id + 2, 1);
		current = current->right;
		return true;
	}
	return false;
}

bool Else(int& token_id, int last) {
	if (token_vector[token_id].token == "Else") {
		current = logicprev;
		current = current->left;
		current = add(current, token_id, branch_tree);
		if(branch_tree != 0) {
			current = current->right;
			prev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			prev = current;
			current = add(current, -1, 0);
		}
		token_id = token_id + 1;
		int LastBracket = last;
		if (Parser(token_id, LastBracket))
		{
			current = prev;
			current = add(current, -1, 0);
			current = current->left;
			return true;
		}
		return true;
	}
	else {
		return false;
	}
}

//Оператор сравнения
bool IF(int& token_id, int last)
{
	int num = token_id;
	if (token_vector[token_id].token == "if" && token_vector[token_id + 1].token == "(" && Logic(token_id + 2) && token_vector[token_id+5].token == ")" && token_vector[token_id + 6].token == "Then")
	{
		current = add(current, num, branch_tree);
		if (branch_tree != 0)current = current->right;
		else current = current->left;
		struct tree* tmpt = current;
		current = add(current, num + 6, 1);
		current = current->right;
		token_id += 7;
		branch_tree = 0;
		int Token_tmp = token_id;
		int LastBracket = last;
		if (!FindEndIf(Token_tmp, LastBracket)) return false;
		if (Parser(token_id, LastBracket))
		{
			last = lastEnd;
			current = prev;
			current = add(current, -1, 0);
			current = current->left;
			token_id = token_id + 2;
			return true;
		}
	}
	else {
		return false;
	}
}

//Арифметические операции
bool Ariphmetics(int token_id)
{
	int tmp_id = token_id;
	if ((token_vector[tmp_id].type == ID || NUMBER)  && token_vector[tmp_id + 1].type == ARIPHMETIC && (token_vector[tmp_id+2].type == ID || NUMBER))
	{
		current = add(current, tmp_id+1, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
			prev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			prev = current;
			current = add(current, -1, 0);
		}
		current = add(current, tmp_id, 1);
		current = current->right;
		current = add(current, tmp_id + 2, 1);
		current = current->right;
		return true;
	}
	return false;
}


bool isInitialize(int& token_id)
{
	int tmp_id = token_id;

	if (token_vector[tmp_id].token == "Dim" && token_vector[tmp_id + 1].type == ID && token_vector[tmp_id + 2].token == "As" && token_vector[tmp_id + 3].type == TYPE && token_vector[tmp_id + 4].token == "=")
	{
		return true;
	}
	return false;
}

bool isredefinition(int& token_id)
{
	int tmp_id = token_id;

	if (token_vector[tmp_id].type == ID && token_vector[tmp_id + 1].token == "=")
	{
		return true;
	}
	return false;
}

//Присвоение
bool Assign(int& skip, int Const, int branch_const)
{
	int num = skip - 1;
	if (isInitialize(num) && Ariphmetics(num+5)) {
		skip += 7;
		current = add(current, num + 4, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
		}
		else {
			current = current->left;
		}
		current = add(current, num, 1);
		current = current->right;
		current = add(current, num + 1, 1);
		current = current->right;
		current = add(current, num + 2, 1);
		current = current->right;
		current = add(current, num + 3, 1);
		current = prev;
		branch_tree = 0;
		return true;
	}
	else if (isredefinition(num) && Ariphmetics(num + 2)) {
		skip += 4;
		current = add(current, num + 1, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
		}
		else {
			current = current->left;
		}
		current = add(current, num, 1);
		current = prev;
		branch_tree = 0;
		return true;
	}
	else if ((isInitialize(num) && token_vector[num + 4].token == "=" && (token_vector[num + 5].type == NUMBER || token_vector[num + 5].type == ID || token_vector[num + 5].type == STR || token_vector[num + 5].token == "True" || token_vector[num + 5].token == "False"))){
		skip += 5;
		current = add(current, num, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
			prev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			prev = current;
			current = add(current, -1, 0);
		}
		current = add(current, num + 1, 1);
		current = current->right;
		current = add(current, num + 2, 1);
		current = current->right;
		current = add(current, num + 3, 1);
		current = current->right;
		current = add(current, num + 4, 1);
		current = current->right;
		current = add(current, num + 5, 1);
		current = prev;
		branch_tree = 0;
		return true;
	}
	else if (isredefinition(num) && token_vector[num + 2].type == ID && token_vector[num + 3].token == "." && token_vector[num + 4].type == ID) {
		skip += 4;
		current = add(current, num+2, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
			prev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			prev = current;
			current = add(current, -1, 0);
		}
		current = add(current, num + 3, 1);
		current = current->right;
		current = add(current, num + 4, 1);
		current = current->right;
		current = add(current, num + 1, 1);
		current = current->right;
		current = add(current, num, 1);
		current = prev;
		branch_tree = 0;
		return true;
	}
	else if (isredefinition(num) && (token_vector[num + 2].type == NUMBER || token_vector[num + 2].type == ID || token_vector[num + 2].type == STR || token_vector[num + 2].token == "True" || token_vector[num + 2].token == "False")) {
		skip += 2;
		current = add(current, num + 1, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
			prev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			prev = current;
			current = add(current, -1, 0);
		}
		current = add(current, num, 1);
		current = current->right;
		current = add(current, num + 2, 1);
		current = prev;
		branch_tree = 0;
		return true;
	}
	else if (token_vector[num].type == ID && token_vector[num + 1].token == "." && token_vector[num+2].type == ID & token_vector[num + 3].token == "=" && (token_vector[num + 4].type == NUMBER || token_vector[num + 4].type == ID || token_vector[num + 4].type == STR || token_vector[num + 4].token == "True" || token_vector[num + 4].token == "False")) {
		skip += 4;
		current = add(current, num, branch_tree);
		if (branch_tree != 0) {
			current = current->right;
			prev = current;
			current = add(current, -1, 0);
		}
		else {
			current = current->left;
			prev = current;
			current = add(current, -1, 0);
		}
		current = add(current, num + 1, 1);
		current = current->right;
		current = add(current, num + 2, 1);
		current = current->right;
		current = add(current, num + 3, 1);
		current = current->right;
		current = add(current, num + 4, 1);
		current = prev;
		branch_tree = 0;
		return true;
	}
	return false;
} 
//Объявление переменных
bool VarDec(int& first, int last, int branch = 0)
{
	int tmp = first + 1;
	int res = tmp;
	if (first == last)
	{
		return false;
	}
	if ((token_vector[first].token == "Dim" && token_vector[first + 1].type == ID && token_vector[first + 2].token == "As" && (token_vector[first + 3].type == TYPE || token_vector[first + 3].type == ID) && token_vector[first + 4].token != "=") )
	{
		if (tmp - res == 0)
		{
			first += 4;
			current = add(current, tmp - 1, branch_tree);
			if (branch_tree != 0) {
				current = current->right;
				prev = current;
				current = add(current, -1, 0);
			}
			else {
				current = current->left;
				prev = current;
				current = add(current, -1, 0);
			}
			current = add(current, tmp, 1);
			current = current->right;
			current = add(current, tmp+1, 1);
			current = current->right;
			current = add(current, tmp + 2, 1);
			current = prev;
			branch_tree = 0;
		}
	}
	else if (Assign(tmp, 0, 1))
	{
		first = tmp;
	}
	else {
		return false;
	}

	if (first != last)
	{
		return VarDec(first, last);
	}
	else {
		return true;
	}
}




//Функция и процедура
bool Func(int& token_id, int last)
{
	int num = token_id;
	if ((token_vector[token_id].token == "Sub" || token_vector[token_id].token == "Function") && token_vector[token_id + 1].type == ID && token_vector[token_id + 2].token == "("  && token_vector[token_id + 3].token == ")")
	{
		current = add(current, num, 0);
		current = current->left;
		funcprev = current;
		current = add(current, num + 1, 1);
		current = current->right;
		prev = current;
		branch_tree = 0;
		int Token_tmp = token_id;
		lastEndFunc = last;
		int LastBracket = last;
		if (token_vector[token_id].token == "Function") {
			if (!FindEndFunction(Token_tmp, LastBracket)) return false;
		}
		else if (token_vector[token_id].token == "Sub") {
			if (!FindEndSub(Token_tmp, LastBracket)) return false;
		}
		token_id += 4;
		lastEnd = LastBracket;
		if (Parser(token_id, LastBracket))
		{
			current = funcprev;
			current = add(current, -1, 0);
			current = current->left;
			last = lastEndFunc;
			token_id = token_id + 2;
			return true;
		}
	}
	else if ((token_vector[token_id].token == "Sub" || token_vector[token_id].token == "Function") && token_vector[token_id + 1].token == "Main" && token_vector[token_id + 2].token == "(" && token_vector[token_id + 3].token == ")") {
		current = add(current, num, 0);
		current = current->left;
		funcprev = current;
		current = add(current, num + 1, 1);
		current = current->right;
		prev = current;
		branch_tree = 0;
		int Token_tmp = token_id;
		lastEndFunc = last;
		int LastBracket = last;
		if (token_vector[token_id].token == "Function") {
			if (!FindEndFunction(Token_tmp, LastBracket)) return false;
		}
		else if (token_vector[token_id].token == "Sub") {
			if (!FindEndSub(Token_tmp, LastBracket)) return false;
		}
		token_id += 4;
		lastEnd = LastBracket;
		if (Parser(token_id, LastBracket))
		{
			current = funcprev;
			current = add(current, -1, 0);
			current = current->left;
			last = lastEndFunc;
			token_id = token_id + 2;
			return true;
		}
	}
	else {
		return false;
	}
}

//Структура
bool Struct(int& token_id, int last)
{
	if (token_vector[token_id].token == "Structure" && token_vector[token_id + 1].type == ID)
	{
		current = add(current, token_id, 0);
		current = current->left;
		lastParse = current;
		current = add(current, token_id + 1, 1);
		current = current->right;
		prev = current;
		branch_tree = 1;
		token_id += 2;
		int Token_tmp = token_id;
		int LastBracket = last;
		if (!FindEndStructure(Token_tmp, LastBracket)) return false;
		if (Parser(token_id, LastBracket))
		{
 			current = lastParse;
			current = add(current, -1, 0);
			current = current->left;
			last = lastEndFunc;
			token_id = token_id + 2;
			return true;
		}
	}
	return false;
}


//Последовательность
bool Parser(int& first, int last)
{
	if (first == last)
	{
		return true;
	}
	if (Struct(first, last) || Func(first, last))
	{
		return Parser(first, last);
	}
	if (IF(first, last) || VarDec(first, last, branch_tree) || Func(first, last) || Else(first, last) || Write(first) || Read(first) || CallProc(first) || Cicle(first, last))
	{
		if (first != last)
		{
			return Parser(first, last);
		}
		else
		{
			return true;
		}
	}
	return false;
}


//Программа
bool Program(int& token_id)
{
	if (token_vector[token_id].token == "Module" && token_vector[token_id + 1].type == ID)
	{
		current = add(current, token_id, 0);
		current = current->left;
		current = add(current, token_id + 1, 1);
		current = add(current, -1, 0);
		current = current->right;
		token_id += 2;
		int LastBracket = token_id;
		if (!FindEndModule(LastBracket)) return false;
		//branch_tree = 1;
		if (Parser(token_id, LastBracket))
		{
			token_id = LastBracket + 1;
			return true;
		}
	}
	return false;
}

