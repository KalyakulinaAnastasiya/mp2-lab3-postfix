#include "postfix.h"
#include "stack.h"


bool TPostfix::IsOperation(char p)
{
	if ((p == '+') || (p == '-') || (p == '*') || (p == '/'))
		return true;
	return false;
}
bool TPostfix::CheckInfix()
{
	int a = 0, ops = 0;
	for (int i = 0; i < infix.size(); i++)
	{
		if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/')
			ops++;
		else if (!IsOperation(infix[i]) && infix[i] != '(' && infix[i] != ')')
		{
			while (infix[i] != '+' && infix[i] != '-'  && infix[i] != '*'  && infix[i] != '/')
			{
				i++;
				if (i == infix.size())
					break;
			}
			i--;
			a++;
		}
	}
	if (a != ops + 1)
		return false;


	int left = 0, right = 0;
	for (int i = 0; i < infix.size(); i++)
		if (infix[i] == '(')
			left++;
		else if (infix[i] == ')')
			right++;
	if (left != right)
		return false;
	if (infix[0] == ')' || infix[0] == '*' || infix[0] == '/' || infix[0] == '-' || infix[0] == '+')
		return false;
	if (infix[infix.size() - 1] == '(' || infix[infix.size() - 1] == '*' || infix[infix.size() - 1] == '/' || infix[infix.size() - 1] == '-' || infix[infix.size() - 1] == '+')
		return false;
	return true;
}
int TPostfix::Priority(char a, char b)
{
	if ((a == '*' || a == '/') && (b == '*' || b == '/'))
		return 0;
	if ((a == '*' || a == '/') && (b == '+' || b == '-'))
		return 1;
	if ((a == '+' || a == '-') && (b == '*' || b == '/'))
		return -1;
	if ((a == '+' || a == '-') && (b == '+' || b == '-'))
		return 0;
	throw "not operation";
}
bool TPostfix::Operand(char op)
{
	for (char i = '0'; i <= '9'; i++)
		if (op == i)
			return true;
	return false;
}
string TPostfix::ToPostfix()
{
	//postfix = string("ab+");
	//return postfix;
	if (!CheckInfix())
		throw "Error";
	TStack<char> ops(infix.size());
	for (int i = 0; i < infix.size(); i++)
	{
		if (!IsOperation(infix[i]) && infix[i] != '(' && infix[i] != ')')
		{
			int j = i;
			while (!IsOperation(infix[j]) && j != infix.size() && infix[j] != ')')
			{
				postfix += infix[j];
				j++;
			}
			i = j - 1;
			postfix += ',';
		}
		else if (ops.IsEmpty())
			ops.Push(infix[i]);
		else if (ops.Back() == '(')
			ops.Push(infix[i]);
		else if (infix[i] == '(')
			ops.Push(infix[i]);
		else if (infix[i] == ')')
		{
			while (ops.Back() != '(')
			{
				postfix += ops.Pop();
				postfix += ',';
			}
			ops.Pop();
		}
		else if (Priority(infix[i], ops.Back()) == 1)
			ops.Push(infix[i]);
		else
		{
			while ((ops.Back() != '('))
			{
				if (Priority(infix[i], ops.Back()) == 1)
					break;
				postfix += ops.Pop();
				postfix += ',';
				if (ops.IsEmpty())
					break;
			}
			ops.Push(infix[i]);
		}
	}
	while (!ops.IsEmpty())
	{
		postfix += ops.Pop();
		postfix += ',';
	}
	return postfix;
}
double TPostfix::Calculate()
{
	//return 0;
	TStack<double>stack(postfix.size());
	string str;
	double tmp1, tmp2;
	for (int i = 0; i < postfix.size(); i++)
	{
		if (postfix[i] == ',')
			continue;
		if (Operand(postfix[i]))
		{
			str = "";
			str += postfix[i];
			while (Operand(postfix[i + 1]) || (postfix[i + 1] == '.'))
			{
				str += postfix[i + 1];
				i++;
			}
			stack.Push(atof(str.c_str()));
		}
		else
		{
			tmp1 = stack.Pop();
			tmp2 = stack.Pop();
			
			switch (postfix[i]) {
			case'+':stack.Push(tmp1 + tmp2); break;
			case'-':stack.Push(tmp2 - tmp1); break;
			case'*':stack.Push(tmp1 * tmp2); break;
			case'/':stack.Push(tmp2 / tmp1); break;
			default: "Error";
			}
		}
	}
	return stack.Pop();
}


