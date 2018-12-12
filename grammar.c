#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Compiler.h"

char symbolArray[100000][500];//单词数组
int symtype[100000];
int symbolIndex;//单词数组下标
int symbolArrayLength;
int midCodeIndex;
char symbolName[500];//用于获取读取的单词
int symbolType;//读取的单词类型
int level = 0;//层次
int symbolTabAdress = 0;
int varIndex = 0;
int labelIndex = 0;
int isArray = 0;
int paraNum = 0;
char String[500][500];
symbol element[10000];
midCode midElement[100000];

void insertSymbolTab(char *name,int type,int value,int para,int level);
void genNewLable(char *label);
void genMidCode(char *name,char *left_Num,char *right_Num,char *result);
void getSymbol();
void flashBack(int times);
void constDesc();
void varDesc();
void Func();
void returnFuncDef();
void unreturnFuncDef();
void mainFunc();
void constDef();
void varDef();
void paraTab();
void comStatement();
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
void condition(char *varTemp1,char *varTemp2,char *opType);
void valueParaTab();
void expression(char *tempVar);
void caseTab(char *varTemp);
void caseChildState(char *varTemp);
void defaultFunc();
void term(char *tempVar);
void factor(char *ident);

void insertSymbolTab(char *name,int type,int value,int para,int level){
    strcpy(element[symbolTabAdress].name,name);
    element[symbolTabAdress].type = type;
    element[symbolTabAdress].value = value;
    element[symbolTabAdress].address = symbolTabAdress;
    element[symbolTabAdress].para = para;
    element[symbolTabAdress].level = level;
    if(isArray)
        symbolTabAdress += para;
    else
        symbolTabAdress++;
}

void genMidCode(char *op,char *left_Num,char *right_Num,char *result){
    strcpy(midElement[midCodeIndex].op,op);
    strcpy(midElement[midCodeIndex].left_Num,left_Num);
    strcpy(midElement[midCodeIndex].right_Num,right_Num);
    strcpy(midElement[midCodeIndex].result,result);
    //printf("\nop:%s left:%s right:%s result:%s\n",midElement[midCodeIndex].op,midElement[midCodeIndex].left_Num,midElement[midCodeIndex].right_Num,midElement[midCodeIndex].result);
    midCodeIndex++;
}

int isInSymbolTab(int level,char *symName){
    int i=0;
    for(i=0;i<symbolTabAdress;i++){
        if(!strcmp(symName,element[i].name)){
            if(element[i].level == level || element[i].level == 0){
                return i;
            }
            else if(level == 0 && (element[i].type == 4 || element[i].type == 5 || element[i].type == 6)){
                return i;
            }
        }
    }
    return -1;
}

void genNewLable(char *label){
    sprintf(label,"label%d",labelIndex);
    labelIndex++;
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
    constDesc();
    varDesc();
    Func();
    mainFunc();
}

void constDesc(){
    getSymbol();
    if(symbolType == CONSTSYM){
        constDef();
        if(symbolType == SEMI){
            //printf("This is constDesc\n");
            constDesc();
        }
    }
}

void varDesc(){
    varDef();
    if(symbolType == SEMI){
        //printf("This is varDesc\n");
        getSymbol();
        varDesc();
    }
}

void Func(){
    if(symbolType == VOIDSYM)
        flashBack(1);
    else
        flashBack(2);
    while(symbolType == INTSYM || symbolType == CHARSYM ||symbolType == VOIDSYM){
        if(symbolType == INTSYM || symbolType == CHARSYM){
            returnFuncDef();
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
                unreturnFuncDef();
                getSymbol();
            }
        }
        else{
            printf("This is ERROR,no int or char or void\n");
            break;
        }
    }
}

void returnFuncDef(){
    level++;
    char identName[500];
    char typeString[500];
    int type;
    int address = 0;
    if(symbolType == INTSYM || symbolType == CHARSYM){
        type = (symbolType == INTSYM) ? 4 : 5;
        if(type == 4)
            strcpy(typeString,"int");
        else
            strcpy(typeString,"char");
        getSymbol();
        if(symbolType == IDENT){
            strcpy(identName,symbolName);
            address = symbolTabAdress;
            insertSymbolTab(identName,type,0,0,level);
            genMidCode("func","int",identName,"()");
            getSymbol();
            if(symbolType == Lparen){
                paraTab();
                element[address].para = paraNum;
                if(symbolType == Rparen){
                    getSymbol();
                    if(symbolType == LBRACE){
                        comStatement();
                        if(symbolType == RBRACE){
                            //printf("This is returnFuncDef\n");
                        }
                        else{
                            //printf("This is ERROR,no RBRACE\n");
                        }
                    }
                    else{
                        //printf("This is ERROR,no LBRACE\n");
                    }
                }
                else{
                    //printf("This is ERROR,no Rparen\n");
                }
            }
            else{
                //printf("This is ERROR,no Lparen\n");
            }
        }
        else{
            //printf("This is ERROR,no IDENT\n");
        }
    }
    else{
        //printf("This is ERROR,no int or char\n");
    }
}

void unreturnFuncDef(){
    level++;
    char identName[500];
    int type;
    int address = 0;
    if(symbolType == VOIDSYM){
        type = 6;
        getSymbol();
        if(symbolType == IDENT){
            strcpy(identName,symbolName);
            address = symbolTabAdress;
            insertSymbolTab(identName,type,0,0,level);
            genMidCode("func","void",identName,"()");
            getSymbol();
            if(symbolType == Lparen){
                paraTab();
                element[address].para = paraNum;
                if(symbolType == Rparen){
                    getSymbol();
                    if(symbolType == LBRACE){
                        comStatement();
                        if(symbolType == RBRACE){
                            //printf("This is unreturnFuncDef\n");
                        }
                        else{
                            //printf("This is ERROR,no RBRACE\n");
                        }
                    }
                    else{
                        //printf("This is ERROR,no LBRACE\n");
                    }
                }
                else{
                    //printf("This is ERROR,no Rparen\n");
                }
            }
            else{
                //printf("This is ERROR,no Lparen\n");
            }
        }
        else{
           //printf("This is ERROR,no IDENT\n");
        }
    }
    else{
        //printf("This is ERROR,no void\n");
    }
}

void mainFunc(){
    level++;
    getSymbol();
    if(symbolType == VOIDSYM){
        getSymbol();
        if(symbolType == MAINSYM){
            getSymbol();
            if(symbolType == Lparen){
                getSymbol();
                if(symbolType == Rparen){
                    getSymbol();
                    genMidCode("func","void","main","()");
                    if(symbolType == LBRACE){
                        comStatement();
                        if(symbolType == RBRACE){
                            //printf("This is mainFunc\n");
                        }
                        else{
                            //printf("This is ERROR,no LBRACE\n");
                        }
                    }
                    else{
                        //printf("This is ERROR,no LBRACE\n");
                    }
                }
                else{
                    //printf("This is ERROR,no Rparen\n");
                }
            }
            else{
                //printf("This is ERROR,no Lparen\n");
            }
        }
        else{
            //printf("This is ERROR,no main\n");
        }
    }
    else{
        //printf("This is ERROR,no void\n");
    }
}

void constDef(){
    char identName[500];
    char result[500];
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
                            genMidCode("const","int",identName,symbolName);
                            //printf("This is constDef\n");
                            getSymbol();
                        }
                        else{
                            //printf("This is ERROR,no UNSIGNEDINTGER\n");
                            break;
                        }
                    }
                    else if(symbolType == MINUS){
                        getSymbol();
                        if(symbolType == UNSIGNEDINTGER){
                            insertSymbolTab(identName,0,(0-atoi(symbolName)),0,level);
                            strcat(result,"-");
                            strcat(result,symbolName);
                            genMidCode("const","int",identName,result);
                            //printf("This is constDef\n");
                            getSymbol();
                        }
                        else{
                            printf("This is ERROR,no UNSIGNEDINTGER\n");
                            break;
                        }
                    }
                    else if(symbolType == UNSIGNEDINTGER){
                        insertSymbolTab(identName,0,atoi(symbolName),0,level);
                        genMidCode("const","int",identName,symbolName);
                        //printf("This is constDef\n");
                        getSymbol();
                    }
                    else{
                        //printf("This is ERROR,no UNSIGNEDINTGER\n");
                        break;
                    }
                }
                else{
                    //printf("This is ERROR,no ASSIGN\n");
                    break;
                }
            }
            else{
                //printf("This is ERROR,no IDENT\n");
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
                        genMidCode("const","char",identName,symbolName);
                        //printf("This is constDef\n");
                        getSymbol();
                    }
                    else{
                        //printf("This is ERROR,no CHAR\n");
                        break;
                    }
                }
                else{
                    //printf("This is ERROR,no ASSIGN\n");
                    break;
                }
            }
            else{
                //printf("This is ERROR,no IDENT\n");
                break;
            }
        }while(symbolType == COMMA);

    }
    else{
    }
}

void varDef(){
    int arrayLength;
    char array_Length[500];
    char identName[500];
    int type;
    char typeString[500];
    isArray = 0;
    if(symbolType == INTSYM || symbolType == CHARSYM){
        type = (symbolType == INTSYM) ? 2 : 3;
        if(type == 2)
            strcpy(typeString,"int");
        else
            strcpy(typeString,"char");
        do{
            getSymbol();
            if(symbolType == IDENT){
                strcpy(identName,symbolName);
                getSymbol();
                if(symbolType == LPAREN){
                    getSymbol();
                    if(symbolType == UNSIGNEDINTGER){
                        arrayLength = atoi(symbolName);
                        strcpy(array_Length,symbolName);
                        getSymbol();
                        if(symbolType == RPAREN){
                            isArray = 1;
                            insertSymbolTab(identName,type,0,arrayLength,level);
                            genMidCode("var",typeString,array_Length,identName);
                            isArray = 0;
                            //printf("This is varDef\n");
                            getSymbol();
                        }
                        else{
                            //printf("This is ERROR,no RPAREN\n");
                            break;
                        }
                    }
                    else {
                        //printf("This is ERROR,no UNSIGNEDINTGER\n");
                        break;
                    }
                }
                else if(symbolType == COMMA){
                    insertSymbolTab(identName,type,0,0,level);
                    genMidCode("var",typeString,"",identName);
                    //printf("This is varDef\n");
                }
                else if(symbolType == Lparen){
                    break;
                }
                else{
                    insertSymbolTab(identName,type,0,0,level);
                    genMidCode("var",typeString,"",identName);
                    break;
                }
            }
            else{
                //printf("This is ERROR,no IDENT\n");
                break;
            }
        }
        while(symbolType == COMMA);
   }
}

void paraTab(){
    paraNum = 0;
    int type;
    char typeString[500];
    getSymbol();
    if(symbolType == Rparen){
        //printf("This is paraTab\n");
        return;
    }
    if(symbolType == INTSYM ||symbolType == CHARSYM){
        do{
            if(symbolType == COMMA){
                getSymbol();
            }
            if(symbolType == INTSYM ||symbolType == CHARSYM){
                type = (symbolType == INTSYM) ? 7 : 8;
                if(type == 7)
                    strcpy(typeString,"int");
                else
                    strcpy(typeString,"char");
                getSymbol();
                if(symbolType == IDENT){
                    paraNum++;
                    insertSymbolTab(symbolName,type,0,0,level);
                    genMidCode("para",typeString,"",symbolName);
                    getSymbol();
                }
                else{
                    //printf("This is ERROR,no IDENT\n");
                    break;
                }
            }
            else{
                //printf("This is ERROR,no int or char\n");
                break;
            }
        }
        while(symbolType == COMMA);
        //printf("This is paraTab\n");
    }
}

void comStatement(){
    getSymbol();
    if(symbolType == CONSTSYM){
        flashBack(1);
        constDesc();
    }
    if(symbolType == INTSYM || symbolType == CHARSYM){
        varDesc();
    }
    if(symbolType == IFSYM || symbolType == WHILESYM || symbolType == LBRACE || symbolType == IDENT || symbolType == IFSYM ||symbolType == SCANFSYM || symbolType == PRINTFSYM || symbolType == SEMI || symbolType == SWITCHSYM ||symbolType == RETURNSYM){
        stateColumn();
        //printf("This is Compound statement\n");
    }
    else{
        //printf("This is ERROR,no many language head\n");
    }
}

void stateColumn(){
    while(symbolType == IFSYM || symbolType == WHILESYM || symbolType == LBRACE || symbolType == IDENT || symbolType == IFSYM ||
    symbolType == SCANFSYM || symbolType == PRINTFSYM || symbolType == SEMI || symbolType == SWITCHSYM ||symbolType == RETURNSYM){
        Statement();
        getSymbol();
    }
    //printf("This is Statement Column\n");
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
            //printf("This is stateColumn\n");
        }
        else{
            //printf("This is ERROR,no RBRACE\n");
        }
    }
    else if(symbolType == IDENT){
        int i=0;
        int flag = 0;
        int type;
        for(i=0;i<symbolTabAdress;i++){
            if(level == element[i].level || element[level].level == 0){
                if(!strcmp(symbolName,element[i].name)){
                    type = element[i].type;
                    break;
                }
            }
        }
        if(type == 4 || type == 5){
            flag = 1;
        }
        else if(type == 6){
            flag = 2;
        }
        else{
            flag = 0;
        }
        if(flag == 1){
            reFuncCall();
            getSymbol();
            if(symbolType == SEMI){
                //printf("This is Call with return value function\n");
            }
            else{
                //printf("This is ERROR,no SEMI\n");
            }
        }
        else if(flag == 2){
            unreFuncCall();
            getSymbol();
            if(symbolType == SEMI){
                //printf("This is Call with no return value function\n");
            }
            else{
                //printf("This is ERROR,no SEMI\n");
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
            //printf("This is scanf statement\n");
        }
        else{
            //printf("This is ERROR,no SEMI\n");
        }
    }
    else if(symbolType == PRINTFSYM){
        printfState();
        getSymbol();
        if(symbolType == SEMI){
            //printf("This is printf statement\n");
        }
        else{
            //printf("This is ERROR,no SEMI\n");
        }
    }
    else if(symbolType == SEMI){
        //printf("This is Empty statement\n");
    }
    else if(symbolType == SWITCHSYM){
        switchState();
    }
    else if(symbolType == RETURNSYM){
        returnState();
        getSymbol();
        if(symbolType == SEMI){
            //printf("This is Return statement\n");
        }
        else{
            //printf("This is ERROR,no SEMI\n");
        }
    }
    else{
        //printf("This is ERROR,no many language head\n");
    }
}

void conditionState(){
    char varTemp1[500];
    char varTemp2[500];
    char opType[5];
    char label1[500];
    char label2[500];
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        condition(varTemp1,varTemp2,opType);
        if(symbolType == Rparen){
            getSymbol();
            genNewLable(label1);
            genMidCode("BNZ","","",label1);
            genNewLable(label2);
            genMidCode("BZ","","",label2);
            genMidCode(label1,":","","");
            Statement();
            genMidCode(label2,":","","");
            //printf("This is conditionState\n");
        }
        else{
            //printf("This is ERROR,no Lparen\n");
        }
    }
    else{
        //printf("This is ERROR,no Lparen\n");
    }

}

void loopState(){
    char varTemp1[500];
    char varTemp2[500];
    char opType[5];
    char label1[500];
    char label2[500];
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        condition(varTemp1,varTemp2,opType);
        if(symbolType == Rparen){
            getSymbol();
            genNewLable(label1);
            genMidCode("BNZ","","",label1);
            genNewLable(label2);
            genMidCode("BZ","","",label2);
            genMidCode(label1,":","","");
            Statement();
            genMidCode(opType,varTemp1,varTemp2,"");
            genMidCode("BNZ","","",label1);
            genMidCode("BZ","","",label2);
            genMidCode(label2,":","","");
            //printf("This is loopState\n");
        }
        else{
            //printf("This is ERROR,no Lparen\n");
        }
    }
    else{
        //printf("This is ERROR,no Lparen\n");
    }
}

void reFuncCall(){
    char funcName[500];
    strcpy(funcName,symbolName);
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        valueParaTab();
        if(symbolType == Rparen){
            genMidCode("call","","",funcName);
            //printf("This is reFuncCall\n");
        }
    }
    else{
        //printf("This is ERROR,no Lparen\n");
    }
}

void unreFuncCall(){
    char funcName[500];
    strcpy(funcName,symbolName);
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        valueParaTab();
        if(symbolType == Rparen){
            genMidCode("call","","",funcName);
            //printf("This is unreFuncCall\n");
        }
    }
    else{
        //printf("This is ERROR,no Lparen\n");
    }
}

void assignState(){
    char identName[500];
    char varTemp[500];
    strcpy(identName,symbolName);
    getSymbol();
    if(symbolType == ASSIGN){
        getSymbol();
        expression(varTemp);
        //printf("This is assignState\n");
        genMidCode("=","",varTemp,identName);
    }
    else if(symbolType == LPAREN){
        getSymbol();
        expression(varTemp);
        if(symbolType == RPAREN){
            sprintf(identName,"%s[%s]",identName,varTemp);
            getSymbol();
            if(symbolType == ASSIGN){
                getSymbol();
                expression(varTemp);
                genMidCode("=","",varTemp,identName);
                //printf("This is assignState\n");
            }
            else{
               //printf("This is ERROR,no ASSIGN\n");
            }
        }
        else{
            //printf("This is ERROR,no RPAREN\n");
        }
    }
    else{
        //printf("This is ERROR,no ASSIGN or LPAREN\n");
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
                genMidCode("scanf","","",symbolName);
                getSymbol();
            }
            else{
                //printf("This is ERROR,no IDENT\n");
            }
        }while(symbolType == COMMA);
    }
    else{
        //printf("This is ERROR,no Lparen\n");
    }
}

void printfState(){
    char tempVar[500];
    char stringName[500];
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        if(symbolType == STRING){
            strcpy(stringName,symbolName);
            getSymbol();
            if(symbolType == COMMA){
                getSymbol();
                expression(tempVar);
                if(symbolType == Rparen){
                    genMidCode("printf",stringName,tempVar,"");
                }
                else{
                    //printf("This is ERROR,no Rparen\n");
                }
            }
            else if(symbolType == Rparen){
                genMidCode("printf",stringName,"","");
            }
            else{
                //printf("This is ERROR,no COMMA\n");
            }
        }
        else{
            expression(tempVar);
            if(symbolType == Rparen){
                genMidCode("printf","",tempVar,"");
            }
            else{
                //printf("This is ERROR,no Rparen\n");
            }
        }
    }
    else{
        //printf("This is ERROR,no Lparen\n");
    }

}

void switchState(){
    char varTemp[500];
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        expression(varTemp);
        if(symbolType == Rparen){
            getSymbol();
            if(symbolType == LBRACE){
                getSymbol();
                caseTab(varTemp);
                if(symbolType == DEFUALTSYM){
                    defaultFunc();
                    //printf("This is switchState\n");
                }
                else{
                    //printf("This is ERROR,no DEFUALTSYM\n");
                }
            }
            else{
                //printf("This is ERROR,no LBRACE\n");
            }
        }
        else{
            //printf("This is ERROR,no Rparen\n");
        }
    }
    else{
        //printf("This is ERROR,no Lparen\n");
    }
}

void returnState(){
    char varTemp[500];
    getSymbol();
    if(symbolType == Lparen){
        getSymbol();
        expression(varTemp);
        if(symbolType == Rparen){
            genMidCode("ret","","",varTemp);
            //printf("This is returnState\n");
        }
        else{
            //printf("This is ERROR,no Rparen\n");
        }
    }
}

void condition(char *varTemp1,char *varTemp2,char *opType){
    expression(varTemp1);
    strcpy(opType,"!=");
    if(symbolType == UNEQUAL || symbolType == EQUAL || symbolType == GEQUAL ||
       symbolType == SEQUAL || symbolType == GREATER || symbolType == SMALLER){
        switch(symbolType){
        case UNEQUAL:
            strcpy(opType,"!=");
            break;
        case EQUAL:
            strcpy(opType,"==");
            break;
        case GEQUAL:
            strcpy(opType,">=");
            break;
        case SEQUAL:
            strcpy(opType,"<=");
            break;
        case GREATER:
            strcpy(opType,">");
            break;
        case SMALLER:
            strcpy(opType,"<");
            break;
        default:
            break;
        }
        getSymbol();
        expression(varTemp2);
    }
    genMidCode(opType,varTemp1,varTemp2,"");
}

void valueParaTab(){
    char varTemp[500];
    if(symbolType == Rparen){
    }
    else{
        expression(varTemp);
        genMidCode("push","","",varTemp);
        while(symbolType == COMMA){
            getSymbol();
            expression(varTemp);
            genMidCode("push","","",varTemp);
        }
    }
}

void caseTab(char *varTemp){
    caseChildState(varTemp);
    getSymbol();
    while(symbolType == CASESYM){
        caseChildState(varTemp);
        getSymbol();
    }
}

void caseChildState(char *varTemp){
    char label1[500];
    char label2[500];
    char opType = ' ';
    getSymbol();
    if(symbolType == CHAR){
        genMidCode("==",varTemp,symbolName,"");
        getSymbol();
        if(symbolType == COLON){
            getSymbol();
            genNewLable(label1);
            genMidCode("BNZ","","",label1);
            genNewLable(label2);
            genMidCode("BZ","","",label2);
            genMidCode(label1,":","","");
            Statement();
            genMidCode(label2,":","","");
            //printf("This is caseChildState\n");
        }
        else{
            //printf("This is ERROR,no COLON\n");
        }
    }
    else if(symbolType == ADD || symbolType == MINUS){
        opType = (symbolType == MINUS)?'-':'+';
        getSymbol();
        if(symbolType == UNSIGNEDINTGER){
            char name[500];
            if(opType != ' ')
                sprintf(name,"%c%s",opType,symbolName);
            else
                strcpy(name,symbolName);
            genMidCode("==",varTemp,name,"");
            getSymbol();
            if(symbolType == COLON){
                getSymbol();
                genNewLable(label1);
                genMidCode("BNZ","","",label1);
                genNewLable(label2);
                genMidCode("BZ","","",label2);
                genMidCode(label1,":","","");
                Statement();
                genMidCode(label2,":","","");
                //printf("This is caseChildState\n");
            }
            else{
                //printf("This is ERROR,no COLON\n");
            }
        }
        else{
            //printf("This is ERROR,no UNSIGNEDINTGER\n");
        }
    }
    else if(symbolType == UNSIGNEDINTGER){
        genMidCode("==",varTemp,symbolName,"");
        getSymbol();
        if(symbolType == COLON){
            getSymbol();
            genNewLable(label1);
            genMidCode("BNZ","","",label1);
            genNewLable(label2);
            genMidCode("BZ","","",label2);
            genMidCode(label1,":","","");
            Statement();
            genMidCode(label2,":","","");
            //printf("This is caseChildState\n");
        }
        else{
            //printf("This is ERROR,no COLON\n");
        }
    }
    else{
        //printf("This is ERROR,no CONST\n");
    }
}

void defaultFunc(){
    getSymbol();
    if(symbolType == COLON){
        getSymbol();
        if(symbolType == RBRACE){
            //printf("This is defaultState\n");
        }
        else{
            Statement();
            getSymbol();
            //printf("This is defaultState\n");
        }
    }
    else{
        //printf("This is ERROR,no COLON\n");
    }
}

void expression(char *tempVar){
    char opType[5];
    char tempVarTerm[500];
    char temp[500];
    char op = ' ';
    if(symbolType == ADD || symbolType == MINUS){
        op = (symbolType == ADD)?'+':'-';
        getSymbol();
    }
    term(tempVarTerm);
    if(op!=' ')
        sprintf(tempVar,"%c%s",op,tempVarTerm);
    else
        sprintf(tempVar,"%s",tempVarTerm);
    while(symbolType == ADD || symbolType == MINUS){
        if(symbolType == ADD)
            strcpy(opType,"+");
        else
            strcpy(opType,"-");
        getSymbol();
        term(tempVarTerm);
        strcpy(temp,tempVar);
        sprintf(tempVar,"$t%d",varIndex);
        varIndex++;
        genMidCode(opType,temp,tempVarTerm,tempVar);
    }
}

void term(char *tempVar){
    char ident[500];
    char temp[500];
    char opType[5];
    factor(ident);
    strcpy(tempVar,ident);
    while(symbolType == MULT || symbolType == DIV){
        if(symbolType == MULT)
            strcpy(opType,"*");
        else
            strcpy(opType,"/");
        getSymbol();
        factor(ident);
        strcpy(temp,tempVar);
        sprintf(tempVar,"$t%d",varIndex);
        varIndex++;
        genMidCode(opType,temp,ident,tempVar);
    }
}

void factor(char *ident){
    char varTemp[500];
    if(symbolType == IDENT){
        strcpy(ident,symbolName);
        getSymbol();
        if(symbolType == LPAREN){
            getSymbol();
            expression(varTemp);
            if(symbolType == RPAREN){
                sprintf(ident,"%s[%s]",ident,varTemp);
                getSymbol();
            }
            else{
                //printf("This is ERROR,no RPAREN\n");
            }
        }
        else if(symbolType == Lparen){
            flashBack(1);
            int i=0;
            for(i=0;i<symbolTabAdress;i++){
                if(level == element[i].level || element[level].level == 0){
                    if(!strcmp(symbolName,element[i].name)){
                        reFuncCall();
                        sprintf(varTemp,"$t%d",varIndex);
                        varIndex++;
                        genMidCode("funcRet","","",varTemp);
                        strcpy(ident,varTemp);
                        getSymbol();
                        break;
                    }
                    if(i == symbolTabAdress){
                        //printf("This is ERROR,no find function IDENT\n");
                    }
                }
            }
        }
        else{
        }
    }
    else if(symbolType == Lparen){
        getSymbol();
        expression(varTemp);
        if(symbolType == Rparen){
            strcpy(ident,varTemp);
            getSymbol();
        }
        else{
            //printf("This is ERROR,no Rparen\n");
        }
    }
    else if(symbolType == CHAR){
        strcpy(ident,symbolName);
        getSymbol();
    }
    else if(symbolType == UNSIGNEDINTGER){
        strcpy(ident,symbolName);
        getSymbol();
    }
    else{
        //printf("ERROR\n");
    }
}


