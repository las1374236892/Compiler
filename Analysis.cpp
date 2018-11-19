#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char line[100000];//文件缓冲区
int count = 0;//单词计数器
int fileline = 0;//当前行
int lengthOfLine = 0;//当前行的长度
int indexInLine = 0;//当前字符在当前行的位置
int indexInStr = 0;//单词的下标
char ch = ' ';//当前读取的字符
char Symbol[500];//用于获取读取的单词

char stayWord[13][15] = { "const","int","char","void","main","while","if","switch","case","default","scanf","printf","return" };

int isStayWord(char *str) {
	int i = 0;
	for (i = 0; i < 13; i++) {
		if (!strcmp(str, stayWord[i])) {
			break;
		}
	}
	return i;
}

int isCalculate(char ch) {
	if (ch == '!')
		return 4;
	else if (ch == '>' || ch == '<' || ch == '=')
		return 3;
	else if (ch == '*' || ch == '/')
		return 2;
	else if (ch == '+' || ch == '-')
		return 1;
	else
		return 0;
}

int isSymbol(char ch) {
	if (ch == ':')
		return 6;
	else if (ch == '{' || ch == '}')
		return 5;
	else if (ch == '[' || ch == ']')
		return 4;
	else if (ch == '(' || ch == ')')
		return 3;
	else if (ch == ';')
		return 2;
	else if (ch == ',')
		return 1;
	else
		return 0;
}

int isQuotation(char ch) {
	if (ch == '\"')
		return 2;
	else if (ch == '\'')
		return 1;
	else
		return 0;
}

void getCh() {
	ch = line[indexInLine++];
}

void getSym() {
	ch = ' ';
	indexInLine = 0;
	memset(Symbol, 0, 500);
	while (indexInLine < lengthOfLine) {
		indexInStr = 0;
		memset(Symbol, 0, 500);
		/*if(ch == '\n')
			break;*/
		while (ch == ' ' || ch == '\t') {
			getCh();
		}
		if (isupper(ch) || islower(ch)) {
			while (islower(ch) || isupper(ch) || isdigit(ch)) {
				Symbol[indexInStr++] = ch;
				getCh();
			}//????
			count++;
			if (isStayWord(Symbol) >= 13)
				printf("%d IDENT %s Line:%d\n", count, Symbol, fileline);
			else
				printf("%d STAYWORD %s Line:%d\n", count, Symbol, fileline);
		}
		else if (isdigit(ch)) {
			while (isdigit(ch)) {
				Symbol[indexInStr++] = ch;
				getCh();
			}
			count++;
			printf("%d UNSIGNEDINTGER %s Line:%d\n", count, Symbol, fileline);
		}
		else if (isCalculate(ch)) {
			while (isCalculate(ch)) {
				Symbol[indexInStr++] = ch;
				getCh();
			}
			if (indexInStr > 1) {
				count++;
				if (Symbol[0] == '!' && Symbol[1] == '=')
					printf("%d UNEQUAL %s Line:%d\n", count, Symbol, fileline);
				else if (Symbol[0] == '=' && Symbol[1] == '=')
					printf("%d EQUAL %s Line:%d\n", count, Symbol, fileline);
				else if (Symbol[0] == '>' && Symbol[1] == '=')
					printf("%d GEQUAL %s Line:%d\n", count, Symbol, fileline);
				else if (Symbol[0] == '<' && Symbol[1] == '=')
					printf("%d SEQUAL %s Line:%d\n", count, Symbol, fileline);
				else
					printf("%d ERROR %s Line:%d\n", count, Symbol, fileline);
			}
			else {
				count++;
				if (isCalculate(Symbol[0]) == 1)
					printf("%d ADDOP %s Line:%d\n", count, Symbol, fileline);
				else if (isCalculate(Symbol[0]) == 2)
					printf("%d MULTOP %s Line:%d\n", count, Symbol, fileline);
				else if (isCalculate(Symbol[0]) == 3)
					switch (Symbol[0])
					{
					case '>':
						printf("%d GREATER %s Line:%d\n", count, Symbol, fileline);
						break;
					case '<':
						printf("%d SMALLER %s Line:%d\n", count, Symbol, fileline);
						break;
					default:
						printf("%d EQUAL %s Line:%d\n", count, Symbol, fileline);
						break;
					}
			}
		}
		else if (isSymbol(ch)) {
			count++;
			switch (isSymbol(ch))
			{
			case 6:
				printf("%d COLON %c Line:%d\n", count, ch, fileline);
				break;
			case 5:
				printf("%d BRACE %c Line:%d\n", count, ch, fileline);
				break;
			case 4:
				printf("%d PAREN %c Line:%d\n", count, ch, fileline);
				break;
			case 3:
				printf("%d paren %c Line:%d\n", count, ch, fileline);
				break;
			case 2:
				printf("%d SEMI %c Line:%d\n", count, ch, fileline);
				break;
			case 1:
				printf("%d COMMA %c Line:%d\n", count, ch, fileline);
				break;
			default:
				break;
			}
			getCh();
		}
		else if (isQuotation(ch)) {
			count++;
			int t;
			int flag;
			if (isQuotation(ch) == 2) {
				getCh();
				while (isQuotation(ch) != 2) {
					Symbol[indexInStr++] = ch;
					getCh();
				}
				for (t = 0; t < indexInStr; t++) {
					if (Symbol[t] > 126) {
						printf("%d ERROR \"%s\" Line:%d\n", count, Symbol, fileline);
						flag = 1;
						break;
					}
					else if (Symbol[t] < 35 && Symbol[t] != 32 && Symbol[t] != 33) {
						printf("%d ERROR \"%s\" Line:%d\n", count, Symbol, fileline);
						flag = 1;
						break;
					}
					else {
						flag = 0;
					}
				}
				if (!flag)
					printf("%d STRING \"%s\" Line:%d\n", count, Symbol, fileline);
			}
			else {
				getCh();
				while (isQuotation(ch) != 1) {
					Symbol[indexInStr++] = ch;
					getCh();
				}
				if (indexInStr != 1) {
					printf("%d ERROR \'%s\' Line:%d\n", count, Symbol, fileline);
				}
				else if (isCalculate(Symbol[0]) != 1 && isCalculate(Symbol[0]) != 2
					&& !isupper(Symbol[0]) && !islower(Symbol[0]) && !isdigit(Symbol[0])) {
					printf("%d ERROR \'%s\' Line:%d\n", count, Symbol, fileline);
					flag = 1;
				}
				else
					printf("%d CHAR \'%s\' Line:%d\n", count, Symbol, fileline);
			}
			getCh();
		}
		else if (ch == '\n') {
			getCh();
			continue;
		}
		else {
			while (ch != ' ') {
				getCh();
				Symbol[indexInStr++];
			}
			count++;
			printf("%d ERROR %s Line:%d\n", count, Symbol, fileline);
		}
	}
}

int main() {
	FILE *fp;
	memset(line, 0, 100000);
	fp = fopen("D:\\compiler\\input.txt", "r");
	while (fgets(line, 100000, fp) != NULL) {
		fileline++;
		lengthOfLine = strlen(line);
		getSym();
		memset(line, 0, 100000);
	}//每次读入一行
	system("pause");
	return 0;
}
