#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Lex.h"

char fileName[100];//文件路径
char symbolArray[100000][500];//单词数组
int reIndex = 0;//有返回值函数下标
int unreIndex = 0;//无返回值函数下标
int reLength = 0;//有返回值函数长度
int unreLength = 0;//无返回值函数长度
char returnFuncList[10000][500];//返回值函数表
char unreturnFuncList[10000][500];//无返回值函数表
int isInt = 0;//是否为整数

typedef struct{
    char name[500] ; //name of identifier
    int type ;//0-const_int 1-const_char 2-var_int 3-var_char 4-function_int 5-func_char 5-para_int 6-para_char
    int value ;//常量的值,特别的，如果标识符是一个函数名，则用2表示函数的类型为char 1表示函数类型为int，0为void
    int address ;//标识符存储地址或者地址位移
    int para ;//表示函数参数个数或者数组大小
    int level;//表示其所在层次
}symbol ;
symbol element[10000];

int symtype[100000];
int symbolIndex = 0;//单词数组下标
int symbolArrayLength = 0;
int symbolTabAdress = 0;

void getSymbol();
void flashBack(int times);
void program();
void constDesc(int level);
void varDesc(int level);
void Func(int level);
void returnFuncDef(int level);
void unreturnFuncDef(int level);
void mainFunc(int level);
void constDef(int level);
void varDef(int level);
void paraTab(int level);
void comStatement(int level);
void stateColumn();
void Statement();
void conditionState();
void loopState();
void reFuncCall();
void unreFuncCall();
void assignState();
void scanfState();
void printfState();
void switchState();
void returnState();
void condition();
void valueParaTab();
void expression();
void caseTab();
void caseChildState();
void defaultFunc();
void term();
void factor();

int insertSymbolTab(char *name,int type,int value,int para,int level){
    strcpy(element[symbolTabAdress].name,name);
    element[symbolTabAdress].type = type;
    element[symbolTabAdress].value = value;
    element[symbolTabAdress].address = symbolTabAdress;
    element[symbolTabAdress].para = para;
    element[symbolTabAdress].level = level;
    symbolTabAdress++;
}

int isInSymbolTab(char *name,int type,int level){
    int i=0;
    for(i=0;i<symbolTabAdress;i++){
        if(element[i].type == type && element[i].level == level){
            if(!strcmp(element[i].name,name)){
                return element[i].address;
            }
            else{
                return 0;
            }
        }
    }
    return 0;
}

void getSymbol(){
    strcpy(symbolName,symbolArray[symbolIndex]);
    symbolType = symtype[symbolIndex];
    printf("%s ",symbolName);
    symbolIndex++;
}

void flashBack(int times){
    symbolIndex -= times;
    symbolIndex--;
    getSymbol();
}

void program(){
    constDesc(0);
    varDesc(0);
    Func(1);
    mainFunc(2);
}

void constDesc(int level){
    getSymbol();
    if(symbolType == CONSTSYM){
        constDef(level);
        if(symbolType == SEMI){
            printf("This is constDesc\n");
            constDesc(level);
        }
    }
}

void varDesc(int level){
    varDef(level);
    if(symbolType == SEMI){
        printf("This is varDesc\n");
        getSymbol();
        varDesc(level);
    }
}

void Func(int level){
    flashBack(2);
    while(symbolType == INTSYM || symbolType == CHARSYM ||symbolType == VOIDSYM){
        if(symbolType == INTSYM || symbolType == CHARSYM){
            returnFuncDef(level);
            getSymbol();
        }
        else if(symbolType == VOIDSYM){
            getSymbol();
            if(symbolType == MAINSYM){
                flashBack(2);
                break;
            }
            else{
                flashBack(1);
                unreturnFuncDef(level);
                getSymbol();
            }
        }
        else{
            printf("This is ERROR,no int or char or void\n");
            break;
        }
    }
}

void returnFuncDef(int level){
    if(symbolType == INTSYM || symbolType == CHARSYM){
        getSymbol();
        if(symbolType == IDENT){
            strcpy(returnFuncList[reLength++],symbolName);
            getSymbol();
            if(symbolType == Lparen){
                paraTab();
                if(symbolType == Rparen){
                    getSymbol();
                    if(symbolType == LBRACE){
                        comStatement(level);
                        if(symbolType == RBRACE){
                            printf("This is returnFuncDef\n");
                        }
                        else{
                            printf("This is ERROR,no RBRACE\n");
                        }
                    }
                    else{
                        printf("This is ERROR,no LBRACE\n");
                    }
                }
                else{
                    printf("This is ERROR,no Rparen\n");
                }
            }
            else{
                printf("This is ERROR,no Lparen\n");
            }
        }
        else{
            printf("This is ERROR,no IDENT\n");
        }
    }
    else{
        printf("This is ERROR,no int or char\n");
    }
}

void unreturnFuncDef(int level){
    if(symbolType == VOIDSYM){
        getSymbol();
        if(symbolType == IDENT){
            strcpy(unreturnFuncList[unreLength++],symbolName);
            getSymbol();
            if(symbolType == Lparen){
                paraTab();
                if(symbolType == Rparen){
                    getSymbol();
                    if(symbolType == LBRACE){
                        comStatement(level);
                        if(symbolType == RBRACE){
                            printf("This is unreturnFuncDef\n");
                        }
                        else{
                            printf("This is ERROR,no RBRACE\n");
                        }
                    }
                    else{
                        printf("This is ERROR,no LBRACE\n");
                    }
                }
                else{
                    printf("This is ERROR,no Rparen\n");
                }
            }
            else{
                printf("This is ERROR,no Lparen\n");
            }
        }
        else{
           printf("This is ERROR,no IDENT\n");
        }
    }
    else{
        printf("This is ERROR,no void\n");
    }
}

void mainFunc(int level){
    getSymbol();
    if(symbolType == VOIDSYM){
        getSymbol();
        if(symbolType == MAINSYM){
            getSymbol();
            if(symbolType == Lparen){
                getSymbol();
                if(symbolType == Rparen){
                    getSymbol();
                    if(symbolType == LBRACE){
                        comStatement(level);
                        if(symbolType == RBRACE){
                            printf("This is mainFunc\n");
                        }
                        else{
                            printf("This is ERROR,no LBRACE\n");
                        }
                    }
                    else{
                        printf("This is ERROR,no LBRACE\n");
                    }
                }
                else{
                    printf("This is ERROR,no Rparen\n");
                }
            }
            else{
                printf("This is ERROR,no Lparen\n");
            }
        }
        else{
            printf("This is ERROR,no main\n");
        }
    }
    else{
        printf("This is ERROR,no void\n");
    }
}

void constDef(int level){
    char identName[500];
    char charactor;
    getSymbol();
    if(symbolType == INTSYM){
        do{
            getSymbol();
            if(symbolType == IDENT){
                strcpy(identName,symbolName);
                getSymbol();
                if(symbolType == ASSIGN){
                    getSymbol();
                    if(symbolType == ADD){
                        getSymbol();
                        if(symbolType == UNSIGNEDINTGER){
                            insertSymbolTab(identName,0,atoi(symbolName),0,level);
                            printf("This is constDef\n");
                            getSymbol();
                        }
                        else{
                            printf("This is ERROR,no UNSIGNEDINTGER\n");
                            break;
                        }
                    }
                    else if(symbolType == MINUS){
                        getSymbol();
                        if(symbolType == UNSIGNEDINTGER){
                            insertSymbolTab(identName,0,(0-atoi(symbolName)),0,level);
                            printf("This is constDef\n");
                            getSymbol();
                        }
                        else{
                            printf("This is ERROR,no UNSIGNEDINTGER\n");
                            break;
                        }
                    }
                    else if(symbolType == UNSIGNEDINTGER){
                        insertSymbolTab(identName,0,atoi(symbolName),0,level);
                        printf("This is constDef\n");
                        getSymbol();
                    }
                    else{
                        printf("This is ERROR,no UNSIGNEDINTGER\n");
                        break;
                    }
                }
                else{
                    printf("This is ERROR,no ASSIGN\n");
                    break;
                }
            }
            else{
                printf("This is ERROR,no IDENT\n");
                break;
            }
        }
        while(symbolType == COMMA);
    }
    else if(symbolType == CHARSYM){
        do{
            getSymbol();
            if(symbolType == IDENT){
                strcpy(identName,symbolName);
                getSymbol();
                if(symbolType == ASSIGN){
                    getSymbol();
                    if(symbolType == CHAR){
                        charactor = symbolName[1];
                        insertSymbolTab(identName,1,(int)charactor,0,level);
                        printf("This is constDef\n");
                        getSymbol();
                    }
                    else{
                        printf("This is ERROR,no CHAR\n");
                        break;
                    }
                }
                else{
                    printf("This is ERROR,no ASSIGN\n");
                    break;
                }
            }
            else{
                printf("This is ERROR,no IDENT\n");
                break;
            }
        }while(symbolType == COMMA);

    }
    else{
    }
}

void varDef(int level){
    int arrayLength;
    char identName[500];
    int type;
    if(symbolType == INTSYM || symbolType == CHARSYM){
        type = (symbolType == INTSYM) ? 2 : 3;
        do{
            getSymbol();
            if(symbolType == IDENT){
                strcpy(identName,symbolName);
                getSymbol();
                if(symbolType == LPAREN){
                    getSymbol();
                    if(symbolType == UNSIGNEDINTGER){
                        arrayLength = atoi(symbolName);
                        getSymbol();
                        if(symbolType == RPAREN){
                            insertSymbolTab(identName,type,0,arrayLength,level);
                            printf("This is varDef\n");
                            getSymbol();
                        }
                        else{
                            printf("This is ERROR,no RPAREN\n");
                            break;
                        }
                    }
                    else {
                        printf("This is ERROR,no UNSIGNEDINTGER\n");
                        break;
                    }
                }
                else if(symbolType == COMMA){
                    insertSymbolTab(identName,type,0,0,level);
                    printf("This is varDef\n");
                }
                else{
                    break;
                }
            }
            else{
                printf("This is ERROR,no IDENT\n");
                break;
            }
        }
        while(symbolType == COMMA);
   }
}

void paraTab(int level){
    int type;
    int paraNum = 0;
    getSymbol();
    if(symbolType == Rparen){
        printf("This is paraTab\n");
        return;
    }
    if(symbolType == INTSYM ||symbolType == CHARSYM){
        do{
            if(symbolType == COMMA){
                getSymbol();
            }
            if(symbolType == INTSYM ||symbolType == CHARSYM){
                type = (symbolType == INTSYM) ? 5 : 6;
                getSymbol();
                if(symbolType == IDENT){
                    paraNum++;
                    insertSymbolTab(symbolName,type,0,paraNum,level);
                    getSymbol();
                }
                else{
                    printf("This is ERROR,no IDENT\n");
                    break;
                }
            }
            else{
                printf("This is ERROR,no int or char\n");
                break;
            }
        }
        while(symbolType == COMMA);
        printf("This is paraTab\n");
    }
}

void comStatement(int level){
    getSymbol();
    if(symbolType == CONSTSYM){
        flashBack(1);
        constDesc(level);
    }
    if(symbolType == INTSYM || symbolType == CHARSYM){
        varDesc(level);
    }
    if(symbolType == IFSYM || symbolType == WHILESYM || symbolType == LBRACE || symbolType == IDENT || symbolType == IFSYM ||symbolType == SCANFSYM || symbolType == PRINTFSYM || symbolType == SEMI || symbolType == SWITCHSYM ||symbolType == RETURNSYM){
        stateColumn();
        printf("This is Compound statement\n");
    }
    else{
        printf("This is ERROR,no many language head\n");
    }
}

void stateColumn(){
    while(symbolType == IFSYM || symbolType == WHILESYM || symbolType == LBRACE || symbolType == IDENT || symbolType == IFSYM ||
    symbolType == SCANFSYM || symbolType == PRINTFSYM || symbolType == SEMI || symbolType == SWITCHSYM ||symbolType == RETURNSYM){
        Statement();
        getSymbol();
    }
    printf("This is Statement Column\n");
}

void Statement(){
    if(symbolType == IFSYM){
        conditionState();
    }
    else if(symbolType == WHILESYM){
        loopState();
    }
    else if(symbolType == LBRACE){
        getSymbol();
        stateColumn();
        if(symbolType == RBRACE){
            printf("This is stateColumn\n");
        }
        else{
            printf("This is ERROR,no RBRACE\n");
        }
    }
    else if(symbolType == IDENT){
        reIndex = 0;
        unreIndex = 0;
        int flag = 0;
        for(reIndex=0;reIndex<reLength;reIndex++){
            if(!strcmp(returnFuncList[reIndex],symbolName)){
                flag = 1;
                break;
            }
        }
        for(unreIndex=0;unreIndex<unreLength;unreIndex++){
            if(!strcmp(unreturnFuncList[unreIndex],symbolName)){
                flag = 2;
                break;
            }
        }
        if(flag == 1){
            reFuncCall();
            getSymbol();
            if(symbolType == SEMI){
                printf("This is Call with return value function\n");
            }
            else{
                printf("This is ERROR,no SEMI\n");
            }
        }
        else if(flag == 2){
            unreFuncCall();
            getSymbol();
            if(symbolType == SEMI){
                printf("This is Call with no return value function\n");
            }
            else{
                printf("This is ERROR,no SEMI\n");
            }
        }
        else{
            assignState();
            if(symbolType == SEMI){
            }
            else{
                printf("This is ERROR,no SEMI\n");
            }
        }
    }
    else if(symbolType == SCANFSYM){
        scanfState();
        getSymbol();
        if(symbolType == SEMI){
            printf("This is scanf statement\n");
        }
        else{
            printf("This is ERROR,no SEMI\n");
        }
    }
    else if(symbolType == PRINTFSYM){
        printfState();
        getSymbol();
        if(symbolType == SEMI){
            printf("This is printf statement\n");
        }
        else{
            printf("This is ERROR,no SEMI\n");
        }
    }
    else if(symbolType == SEMI){
        printf("This is Empty statement\n");
    }
    else if(symbolType == SWITCHSYM){
        switchState();
    }
    else if(symbolType == RETURNSYM){
        returnState();
        if(symbolType == SEMI){
            printf("This is Return statement\n");
        }
        else{
            printf("This is ERROR,no SEMI\n");
        }
    }
    else{
        printf("This is ERROR,no many language head\n");
    }
}

void conditionState(){
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        condition();
        if(symbolType == Rparen){
            getSymbol();
            Statement();
            printf("This is conditionState\n");
        }
        else{
            printf("This is ERROR,no Lparen\n");
        }
    }
    else{
        printf("This is ERROR,no Lparen\n");
    }

}

void loopState(){
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        condition();
        if(symbolType == Rparen){
            getSymbol();
            Statement();
            printf("This is loopState\n");
        }
        else{
            printf("This is ERROR,no Lparen\n");
        }
    }
    else{
        printf("This is ERROR,no Lparen\n");
    }
}

void reFuncCall(){
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        valueParaTab();
        if(symbolType == Rparen){
            printf("This is reFuncCall\n");
        }
    }
    else{
        printf("This is ERROR,no Lparen\n");
    }
}

void unreFuncCall(){
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        valueParaTab();
        if(symbolType == Rparen){
            printf("This is unreFuncCall\n");
        }
    }
    else{
        printf("This is ERROR,no Lparen\n");
    }
}

void assignState(){
    getSymbol();
    if(symbolType == ASSIGN){
        getSymbol();
        expression();
        printf("This is assignState\n");
    }
    else if(symbolType == LPAREN){
        getSymbol();
        expression();
        if(symbolType == RPAREN){
            getSymbol();
            if(symbolType == ASSIGN){
                getSymbol();
                expression();
                printf("This is assignState\n");
            }
            else{
                printf("This is ERROR,no ASSIGN\n");
            }
        }
        else{
            printf("This is ERROR,no RPAREN\n");
        }
    }
    else{
        printf("This is ERROR,no ASSIGN or LPAREN\n");
    }

}

void scanfState(){
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        do{
            if(symbolType == COMMA){
                getSymbol();
            }
            if(symbolType == IDENT){
                getSymbol();
            }
            else{
                printf("This is ERROR,no IDENT\n");
            }
        }while(symbolType == COMMA);
    }
    else{
        printf("This is ERROR,no Lparen\n");
    }
}

void printfState(){
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        if(symbolType == STRING){
            getSymbol();
            if(symbolType == COMMA){
                getSymbol();
                expression();
                if(symbolType == Rparen){
                }
                else{
                    printf("This is ERROR,no Rparen\n");
                }
            }
            else if(symbolType == Rparen){
            }
            else{
                printf("This is ERROR,no COMMA\n");
            }
        }
        else{
            expression();
            if(symbolType == Rparen){
            }
            else{
                printf("This is ERROR,no Rparen\n");
            }
        }
    }
    else{
        printf("This is ERROR,no Lparen\n");
    }

}

void switchState(){
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        expression();
        if(symbolType == Rparen){
            getSymbol();
            if(symbolType == LBRACE){
                getSymbol();
                caseTab();
                if(symbolType == DEFUALTSYM){
                    defaultFunc();
                    printf("This is switchState\n");
                }
                else{
                    printf("This is ERROR,no DEFUALTSYM\n");
                }
            }
            else{
                printf("This is ERROR,no LBRACE\n");
            }
        }
        else{
            printf("This is ERROR,no Rparen\n");
        }
    }
    else{
        printf("This is ERROR,no Lparen\n");
    }
}

void returnState(){
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        expression();
        if(symbolType == Rparen){
            printf("This is returnState\n");
        }
        else{
            printf("This is ERROR,no Rparen\n");
        }
    }
}

void condition(){
    expression();
    if(symbolType == UNEQUAL || symbolType == EQUAL || symbolType == GEQUAL ||
       symbolType == SEQUAL || symbolType == GREATER || symbolType == SMALLER){
        getSymbol();
        expression();
    }
}

void valueParaTab(){
    if(symbolType == Rparen){
    }
    else{
        expression();
        while(symbolType == COMMA){
            getSymbol();
            expression();
        }
    }
}

void expression(){
    if(symbolType == ADD || symbolType == MINUS){
        getSymbol();
    }
    term();
    while(symbolType == ADD || symbolType == MINUS){
        getSymbol();
        term();
    }
}

void caseTab(){
    caseChildState();
    getSymbol();
    while(symbolType == CASESYM){
        caseChildState();
        getSymbol();
    }
}

void caseChildState(){
    getSymbol();
    if(symbolType == CHAR){
        getSymbol();
        if(symbolType == COLON){
            getSymbol();
            Statement();
            printf("This is caseChildState\n");
        }
        else{
            printf("This is ERROR,no COLON\n");
        }
    }
    else if(symbolType == ADD || symbolType == MINUS){
        getSymbol();
        if(symbolType == UNSIGNEDINTGER){
            getSymbol();
            if(symbolType == COLON){
                getSymbol();
                Statement();
                printf("This is caseChildState\n");
            }
            else{
                printf("This is ERROR,no COLON\n");
            }
        }
        else{
            printf("This is ERROR,no UNSIGNEDINTGER\n");
        }
    }
    else if(symbolType == UNSIGNEDINTGER){
        getSymbol();
        if(symbolType == COLON){
            getSymbol();
            Statement();
            printf("This is caseChildState\n");
        }
        else{
            printf("This is ERROR,no COLON\n");
        }
    }
    else{
        printf("This is ERROR,no CONST\n");
    }
}

void defaultFunc(){
    getSymbol();
    if(symbolType == COLON){
        getSymbol();
        if(symbolType == RBRACE){
            printf("This is defaultState\n");
        }
        else{
            Statement();
            getSymbol();
            printf("This is defaultState\n");
        }
    }
    else{
        printf("This is ERROR,no COLON\n");
    }
}

void term(){
    factor();
    while(symbolType == MULT || symbolType == DIV){
        getSymbol();
        factor();
    }
}

void factor(){
    if(symbolType == IDENT){
        getSymbol();
        if(symbolType == LPAREN){
            getSymbol();
            expression();
            if(symbolType == RPAREN){
                getSymbol();
            }
            else{
                printf("This is ERROR,no RPAREN\n");
            }
        }
        else if(symbolType == Lparen){
            flashBack(1);
            for(reIndex=0;reIndex<reLength;reIndex++){
                if(!strcmp(symbolName,returnFuncList[reIndex])){
                    reFuncCall();
                    getSymbol();
                    break;
                }
                else{
                    printf("This is ERROR,no find function IDENT\n");
                }
            }
        }
        else{
        }
    }
    else if(symbolType == Lparen){
        getSymbol();
        expression();
    }
    else if(symbolType == CHAR){
        getSymbol();
    }
    else if(symbolType == UNSIGNEDINTGER){
        getSymbol();
    }
    else{
        printf("ERROR\n");
    }
}

int main(){
    FILE *fp;
	memset(line, 0, 100000);
	scanf("%s",fileName);
	fp = fopen(fileName, "r");
	while (fgets(line, 100000, fp) != NULL) {
		fileline++;
		lengthOfLine = strlen(line);
		ch = ' ';
        indexInLine = 0;
        while(indexInLine < lengthOfLine){
            getSym();
            if(strlen(symbolName)!=0){
                strcpy(symbolArray[symbolArrayLength],symbolName);
                symtype[symbolArrayLength] = symbolType;
                symbolArrayLength++;
            }
        }//将所有的标识符存入数组中
		memset(line, 0, 100000);
	}//每次读入一行

    /*for(symbolIndex=0;symbolIndex<symbolArrayLength;symbolIndex++){
        printf("%s\n",symbolArray[symbolIndex]);
    }*/
    program();
    int i = 0;
    for(i=0;i<reLength;i++){
        printf("%s\n",returnFuncList[i]);
    }
    for(i=0;i<unreLength;i++){
        printf("%s\n",unreturnFuncList[i]);
    }
	//system("pause");
	return 0;
}
