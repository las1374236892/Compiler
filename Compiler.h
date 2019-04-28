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

#define unKnownSym_ERROR 0//�ʷ������޷�ʶ��ķ���
#define unDefFunc_ERROR 1//δ���庯��
#define unDefIdent_ERROR 2 //δ����ı�ʶ��
#define unMatchSinQuoto_ERROR 3 //�����Ų�ƥ��
#define unMatchDouQuoto_ERROR 4 //˫���Ų�ƥ��
#define funcNameConplitc_ERROR 5 //��������ͻ
#define varNameConplict_ERROR 6 //��������ͻ
#define idMissHead_ERROR 7 // ����ͷ��ȱ�ٱ�ʶ��(ȱint��char)
#define symbolMissAfterAssign_ERROR 8 // ������ͷ����ȱ�ٱ�ʶ��
#define symbolMissAfterConst_ERROR 9 // ��������֮��ȱ�������ı�ʶ��
#define constType_ERROR 10 // �������ʹ���
#define semicSymMis_ERROR 11 //ȱʧ�ֺ�
#define varDefArrayIndex_ERROR 12 //������±겻Ϊ�Ѿ�����Ϊ���ֵı���
#define varDefIdent_ERROR 13 // ���������ʶ������
#define mainSymMis_ERROR 14 //������ȱʧmain
#define leftParSymMis_ERROR 15 // ȱʧ�����š�(��
#define rightParSymMis_ERROR 16 // ȱʧ�����š�)��
#define leftBparSymMis_ERROR 17 // ȱʧ�����š�{��
#define rightBparSymMis_ERROR 18 // ȱʧ�һ����š�}��
#define rightMparSymMis_ERROR 19 // ȱ���������š�]��
#define afterOPNumMis_ERROR 20 // �ڷ���֮������ֻ��߱�ʶ��ȱʧ
#define assignState_ERROR 21 // ��ֵ����
#define Statement_ERROR 22 // ������
#define conditionState_ERROR 23 // ��������
#define caseSymMis_ERROR 24 // ��������ȱ��case
#define colonSymMis_ERROR 25 // �����������ȱʡ���ȱ��ð�š�:��
#define voidMiss_ERROR 26//ȱ��void
#define assignMiss_ERROR 27//ȱ��=
#define commaMiss_ERROR 28//ȱ��,
#define constMis_ERROR 29//ȱ�����ֳ������ַ�����
#define paraNum_ERROR 30

typedef struct{
    char name[500] ; //name of identifier
    int type ;//0-const_int 1-const_char 2-var_int 3-var_char 4-function_int 5-func_char 6-func_void 7-para_int 8-para_char
    int value ;//������ֵ
    int address ;//��ʶ���洢��ַ���ߵ�ַλ��
    int para ;//��ʾ���������������������С
    int level;//��ʾ�����ڲ��
}symbol ;
extern symbol element[10000];

typedef struct{
    char op[500];
    char left_Num[500];
    char right_Num[500];
    char result[500];
}midCode ;
extern midCode midElement[100000];

extern char symbolArray[100000][500];//��������
extern int symbolLine[100000];//������������
extern int symtype[100000];
extern int symbolIndex;//���������±�
extern int symbolArrayLength;
extern int symbolTabAdress;
extern int midCodeIndex;
extern char symbolName[500];//���ڻ�ȡ��ȡ�ĵ���
extern int symbolType;//��ȡ�ĵ�������
extern char copy[500];
extern char line[100000];//�ļ�������
extern int count;//���ʼ�����
extern int fileline;//��ǰ��
extern int lengthOfLine;//��ǰ�еĳ���
extern int indexInLine;//��ǰ�ַ��ڵ�ǰ�е�λ��
extern int indexInStr;//���ʵ��±�
extern char ch;//��ǰ��ȡ���ַ�
extern char stayWord[13][15];
extern int stringCount;
extern char String[500][500];

//int isStayWord(char *str);
int isCalculate(char ch);
int isSymbol(char ch);
int isQuotation(char ch);
void error(int errorCode,int line);
void getCh();
void getSym();

void program();
int isInSymbolTab(int level,char *symName);
#endif // LEX_H_INCLUDED
