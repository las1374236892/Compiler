#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define IDENT 0
#define STAYWORD 1
#define UNSIGNEDINTGER 2
#define UNEQUAL 3
#define EQUAL 4
#define GEQUAL 5
#define SEQUAL 6
#define ADD 7
#define MINUS 8
#define MULT 9
#define DIV 10
#define GREATER 11
#define SMALLER 12
#define ASSIGN 13
#define COLON 14
#define LBRACE 15
#define RBRACE 16
#define LPAREN 17
#define RPAREN 18
#define Lparen 19
#define Rparen 20
#define SEMI 21
#define COMMA 22
#define STRING 23
#define CHAR 24
#define CONSTSYM 25
#define INTSYM 26
#define CHARSYM 27
#define VOIDSYM 28
#define MAINSYM 29
#define WHILESYM 30
#define IFSYM 31
#define SWITCHSYM 32
#define CASESYM 33
#define DEFUALTSYM 34
#define SCANFSYM 35
#define PRINTFSYM 36
#define RETURNSYM 37

typedef struct{
    char name[500] ; //name of identifier
    int type ;//0-const_int 1-const_char 2-var_int 3-var_char 4-function_int 5-func_char 6-func_void 7-para_int 8-para_char
    int value ;//常量的值
    int address ;//标识符存储地址或者地址位移
    int para ;//表示函数参数个数或者数组大小
    int level;//表示其所在层次
}symbol ;
extern symbol element[10000];

typedef struct{
    char op[500];
    char left_Num[500];
    char right_Num[500];
    char result[500];
}midCode ;
extern midCode midElement[100000];

extern char symbolArray[100000][500];//单词数组
extern int symtype[100000];
extern int symbolIndex;//单词数组下标
extern int symbolArrayLength;
extern int symbolTabAdress;
extern int midCodeIndex;
extern char symbolName[500];//用于获取读取的单词
extern int symbolType;//读取的单词类型
extern char copy[500];
extern char line[100000];//文件缓冲区
extern int count;//单词计数器
extern int fileline;//当前行
extern int lengthOfLine;//当前行的长度
extern int indexInLine;//当前字符在当前行的位置
extern int indexInStr;//单词的下标
extern char ch;//当前读取的字符
extern char stayWord[13][15];
extern int stringCount;
extern char String[500][500];

//int isStayWord(char *str);
int isCalculate(char ch);
int isSymbol(char ch);
int isQuotation(char ch);
void getCh();
void getSym();

void program();
int isInSymbolTab(int level,char *symName);
#endif // LEX_H_INCLUDED
