#include<stdio.h>
#define MAX_SIZE 101
char stack[MAX_SIZE];
int top = -1;
int istack[MAX_SIZE];
int itop = -1;
//integer stack operations
void ipush(int x)
{
	if (itop == MAX_SIZE - 1)
	{
		printf("Stack overflow\n");
		return;
	}
	istack[++itop] = x;
}
void ipop()
{
	if (itop == -1)
	{
		printf("Stack  underflow\n");
		return;
	}
	itop--;
}
int isiEmpty()
{
	if (itop == -1)
		return 1;
	else
		return 0;
}
int topiStackElement()
{
	if (itop == -1)
		return -1;
	return istack[itop];
}
//character stack operations
void push(char c)
{
	if (top == MAX_SIZE - 1)
	{
		printf("Stack overflow\n");
		return;
	}
	stack[++top] = c;
}
void pop()
{
	if (top == -1)
	{
		printf("Stack  underflow\n");
		return;
	}
	top--;
}
int isEmpty()
{
	if (top == -1)
		return 1;
	else
		return 0;
}
char topStackElement()
{
	if (top == -1)
		return -1;
	return stack[top];
}
//finding the length of the string
int strLength(char* str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}
int isNumericDigit(char C)
{
	if (C >= '0' && C <= '9') return 1;
	return 0;
}
int isOperator(char C)
{
	if (C == '+' || C == '-' || C == '*' || C == '/' || C == '^')
		return 1;

	return 0;
}
int getOperatorWeight(char op)
{
	int weight = -1;
	switch (op)
	{
	case '(':
		return 1;
	case '+':
	case '-':
		return 2;
	case '*':
	case '/':
		return 3;
	case '^':
		return 4;
	}
	return weight;
}
//*************
int higherPrecedence(char op1, char op2)
{
	int op1Weight = getOperatorWeight(op1);
	int op2Weight = getOperatorWeight(op2);

	if (op1Weight == op2Weight)
		return 1;

	return op1Weight > op2Weight ? 1 : 0;
}
int power(int x, int y)
{
	if (y == 0)
		return 1;
	else if (y % 2 == 0)
		return power(x, y / 2)*power(x, y / 2);
	else
		return x*power(x, y / 2)*power(x, y / 2);
}
int performOperation(char operation, int operand1, int operand2)
{
	if (operation == '+') return operand1 + operand2;
	else if (operation == '-') return operand1 - operand2;
	else if (operation == '*') return operand1 * operand2;
	else if (operation == '/') return operand1 / operand2;
	else if (operation == '^') return power(operand1, operand2);
	else printf("Error\n");
	return -1;
}
int evaluatePostfix(char *expr)
{
	int i;
	int len = strLength(expr);
	for (i = 0; i<len; i++)
	{
		if (expr[i] == ' ' || expr[i] == ',')
			continue;
		else if (isOperator(expr[i]) == 1)
		{
			// Pop two operands.
			int operand2 = topiStackElement();
			ipop();
			int operand1 = topiStackElement();
			ipop();
			// Perform operation
			int result = performOperation(expr[i], operand1, operand2);
			//Push back result of operation on stack.
			ipush(result);
		}
		else if (isNumericDigit(expr[i]))
		{
			int operand = 0;
			while (i<len && isNumericDigit(expr[i])) {
				operand = (operand * 10) + (expr[i] - '0');
				i++;
			}
			i--;
			// Push operand on stack.
			ipush(operand);
		}
	}//end of for loop
	return topiStackElement();

}
int InfixtoPostfix(char * expr)
{
	int len = strLength(expr);
	char postfix[len];
	int index = 0, i, result;
	char topele;

	for (i = 0; i<len; i++)
	{
		if (expr[i] == ' ' || expr[i] == ',')
			continue;
		else if (isNumericDigit(expr[i]) == 1)//checking if the literal is operator
		{
			while (i<len && isNumericDigit(expr[i]) == 1)
			{
				postfix[index++] = expr[i];
				i++;
			}
			postfix[index++] = ' ';
			i--;
		}
		else if (expr[i] == '(')
		{
			push(expr[i]);
		}
		else if (expr[i] == ')')
		{
			while (isEmpty() == 1 || topStackElement() != '(')
			{
				postfix[index++] = topStackElement();
				postfix[index++] = ' ';
				pop();
			}
			pop();//pops out the respective opening paranthesis
		}
		else
		{
			while (getOperatorWeight(topStackElement()) >= getOperatorWeight(expr[i]))
			{
				postfix[index++] = topStackElement();
				postfix[index++] = ' ';
				pop();
			}
			push(expr[i]);
		}
	}//end of for loop
	while (isEmpty() == 0)
	{
		postfix[index++] = topStackElement();
		postfix[index++] = ' ';
		pop();
	}
	postfix[index] = '\0';
	printf("the string after converting into postfix is:\n");
	puts(postfix);
	result = evaluatePostfix(postfix);
	return result;
}
int powerSymbol(char* str)
{
	int i, count = 0;
	for (i = 1; str[i] != '\0'; i++)
	{
		if (str[i] == '*'&&str[i + 1] == '*')
		{
			str[i] = ' ';
			str[i + 1] = '^';
		}
	}
	int result = InfixtoPostfix(str);
	return result;
}
int main()
{

	FILE *fp1 = fopen("SampleFile_ExpressionEvaluator.txt", "r");
	FILE *fp2 = fopen("output.txt", "w");
	char expr[50];
	while (fscanf(fp1, "%s", expr) != EOF)
	{
		int result = powerSymbol(expr);
		fprintf(fp2, "%s=%d\n", expr, result);
		printf("the answer for the given expression:%s %d\n", expr, result);
	}
	fclose(fp1);
	fclose(fp2);
	int r = remove("SampleFile_ExpressionEvaluator.txt");
	printf("%d\n", rename("output.txt", "SampleFile_ExpressionEvaluator.txt"));
	return 0;
}
