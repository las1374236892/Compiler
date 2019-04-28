#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "Compiler.h"

char copy[500];
int count = 0;
int fileline = 0;
int lengthOfLine = 0;
int indexInLine = 0;
int indexInStr = 0;
int stringCount = 0;
char ch = ' ';
char stayWord[13][15] = { "const","int","char","void","main","while","if","switch","case","default","scanf","printf","return" };

int isLetter(char ch){
    if(isupper(ch) || islower(ch) || ch == '_'){
        return 1;
    }
    else{
        return 0;
    }
}

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
		return 9;
	else if (ch == '{')
		return 8;
    else if(ch == '}')
        return 7;
	else if (ch == '[')
        return 6;
    else if(ch == ']')
        return 5;
    else if(ch == '(')
		return 4;
	else if (ch == ')')
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
		indexInStr = 0;
		symbolType = -1;
		memset(symbolName,0,500);
		memset(copy,0,500);
		while (ch == ' ' || ch == '\t') {
			getCh();
		}
		if (isLetter(ch)) {
			while (isLetter(ch)|| isdigit(ch)) {
				symbolName[indexInStr++] = ch;
				getCh();
			}//????
			count++;
			if (isStayWord(symbolName) >= 13){
                symbolType = IDENT;
				//printf("%d IDENT %s Line:%d\n", count, symbolName, fileline);
			}
			else{
                symbolType = STAYWORD;
                switch(isStayWord(symbolName)){
                case 0:
                    symbolType = CONSTSYM;
                    break;
                case 1:
                    symbolType = INTSYM;
                    break;
                case 2:
                    symbolType = CHARSYM;
                    break;
                case 3:
                    symbolType = VOIDSYM;
                    break;
                case 4:
                    symbolType = MAINSYM;
                    break;
                case 5:
                    symbolType = WHILESYM;
                    break;
                case 6:
                    symbolType = IFSYM;
                    break;
                case 7:
                    symbolType = SWITCHSYM;
                    break;
                case 8:
                    symbolType = CASESYM;
                    break;
                case 9:
                    symbolType = DEFUALTSYM;
                    break;
                case 10:
                    symbolType = SCANFSYM;
                    break;
                case 11:
                    symbolType = PRINTFSYM;
                    break;
                case 12:
                    symbolType = RETURNSYM;
                    break;
                default:
                    break;
                }
				//printf("%d STAYWORD %s Line:%d\n", count, symbolName, fileline);
			}
		}
		else if (isdigit(ch)) {
			while (isdigit(ch)) {
				symbolName[indexInStr++] = ch;
				getCh();
			}
			count++;
			int i = 0;
			int num = 0;
			for (i = 0; i < indexInStr; i++) {
				num = num*10 + (int)(symbolName[i] - '0');
			}
			symbolType = UNSIGNEDINTGER;
			//printf("%d UNSIGNEDINTGER %d Line:%d\n", count, num, fileline);
		}
		else if (isCalculate(ch)) {
			while (isCalculate(ch)) {
				symbolName[indexInStr++] = ch;
				getCh();
				if(isCalculate(ch) != 3 && isCalculate(ch) != 4){
                    break;
				}
			}
			if (indexInStr > 1) {
				count++;
				if (symbolName[0] == '!' && symbolName[1] == '='){
                    symbolType = UNEQUAL;
					//printf("%d UNEQUAL %s Line:%d\n", count, symbolName, fileline);
				}
				else if (symbolName[0] == '=' && symbolName[1] == '='){
                    symbolType = EQUAL;
					//printf("%d EQUAL %s Line:%d\n", count, symbolName, fileline);
				}
				else if (symbolName[0] == '>' && symbolName[1] == '='){
                    symbolType = GEQUAL;
					//printf("%d GEQUAL %s Line:%d\n", count, symbolName, fileline);
				}
				else if (symbolName[0] == '<' && symbolName[1] == '='){
                    symbolType = SEQUAL;
					//printf("%d SEQUAL %s Line:%d\n", count, symbolName, fileline);
				}
				else{
					//printf("%d ERROR %s Line:%d\n", count, symbolName, fileline);
				}
			}
			else {
				count++;
				if (isCalculate(symbolName[0]) == 1){
                    switch (symbolName[0])
					{
					case '+':
					    symbolType = ADD;
						//printf("%d ADD %s Line:%d\n", count, symbolName, fileline);
						break;
					case '-':
					    symbolType = MINUS;
						//printf("%d MINUS %s Line:%d\n", count, symbolName, fileline);
						break;
					default:
						break;
					}
				}
				else if (isCalculate(symbolName[0]) == 2){
                    switch (symbolName[0])
					{
					case '*':
					    symbolType = MULT;
						//printf("%d MULT %s Line:%d\n", count, symbolName, fileline);
						break;
					case '/':
					    symbolType = DIV;
						//printf("%d DIV %s Line:%d\n", count, symbolName, fileline);
						break;
					default:
						break;
					}
				}
				else if (isCalculate(symbolName[0]) == 3)
					switch (symbolName[0])
					{
					case '>':
					    symbolType = GREATER;
						//printf("%d GREATER %s Line:%d\n", count, symbolName, fileline);
						break;
					case '<':
					    symbolType = SMALLER;
						//printf("%d SMALLER %s Line:%d\n", count, symbolName, fileline);
						break;
					default:
					    symbolType = ASSIGN;
						//printf("%d ASSIGN %s Line:%d\n", count, symbolName, fileline);
						break;
					}
			}
		}
		else if (isSymbol(ch)) {
			count++;
			symbolName[indexInStr++] = ch;
			switch (isSymbol(ch))
			{
            case 9:
                symbolType = COLON;
                break;
            case 8:
                symbolType = LBRACE;
				break;
            case 7:
                symbolType = RBRACE;
				break;
			case 6:
			    symbolType = LPAREN;
				break;
			case 5:
			    symbolType = RPAREN;
				break;
			case 4:
			    symbolType = Lparen;
				break;
			case 3:
			    symbolType = Rparen;
				break;
			case 2:
			    symbolType = SEMI;
				break;
			case 1:
			    symbolType = COMMA;
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
					copy[indexInStr++] = ch;
					if(ch == '\\'){
                        copy[indexInStr++] = ch;
					}
					getCh();
				}
				for (t = 0; t < indexInStr; t++) {
					if (copy[t] > 126) {
						flag = 1;
						break;
					}
					else if (copy[t] < 35 && copy[t] != 32 && copy[t] != 33) {
						flag = 1;
						break;
					}
					else {
						flag = 0;
					}
				}
				if (!flag){
                    symbolType = STRING;
                    sprintf(String[stringCount],"\"%s\"",copy);
                    stringCount++;
					strcpy(symbolName,"\"");
					strcat(symbolName,copy);
					strcat(symbolName,"\"");
				}
			}
			else {
				getCh();
				while (isQuotation(ch) != 1) {
					copy[indexInStr++] = ch;
					getCh();
				}
				if (indexInStr != 1) {
				}
				else if (isCalculate(copy[0]) != 1 && isCalculate(copy[0]) != 2
					&& !isupper(copy[0]) && !islower(copy[0]) && !isdigit(copy[0]) && copy[0] != '_') {
					flag = 1;
				}
				else{
                    symbolType = CHAR;
					strcpy(symbolName,"\'");
					strcat(symbolName,copy);
					strcat(symbolName,"\'");
				}
			}
			getCh();
		}
		else if (ch == '\n') {
			getCh();
		}
		else {
			while (ch != ' ') {
				getCh();
				symbolName[indexInStr++];
			}
			count++;
            error(unKnownSym_ERROR,fileline);
		}
}
