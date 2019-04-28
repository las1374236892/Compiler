#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Compiler.h"

char symbolArray[100000][500];//单词数组
int symbolLine[100000];
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
int switchIndex = 0;
int isArray = 0;
int paraNum = 0;
int caselevel = 0;//case的层数
int expressionType = -1;
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
void assignState(int type);
void scanfState();
void printfState();
void switchState(char *switchLabel);
void returnState();
void condition(char *varTemp1,char *varTemp2,char *opType,char *conditionLabel);
void valueParaTab(char *name);
void expression(char *tempVar);
void caseTab(char *varTemp,char *switchLabel);
void caseChildState(char *varTemp,char *switchLabel);
void defaultFunc(char *switchLabel);
void term(char *tempVar);
void factor(char *ident);

void insertSymbolTab(char *name,int type,int value,int para,int level){
    int i;
    strcpy(element[symbolTabAdress].name,name);
    element[symbolTabAdress].type = type;
    element[symbolTabAdress].value = value;
    element[symbolTabAdress].address = symbolTabAdress;
    element[symbolTabAdress].para = para;
    element[symbolTabAdress].level = level;
    if(isArray){
        i=0;
        while(i<para){
            element[i + symbolTabAdress].address = i + symbolTabAdress;
            i++;
        }
        symbolTabAdress += para;
    }
    else
        symbolTabAdress++;
}

void genMidCode(char *op,char *left_Num,char *right_Num,char *result){
    strcpy(midElement[midCodeIndex].op,op);
    strcpy(midElement[midCodeIndex].left_Num,left_Num);
    strcpy(midElement[midCodeIndex].right_Num,right_Num);
    strcpy(midElement[midCodeIndex].result,result);
    ////printf("\nop:%s left:%s right:%s result:%s\n",midElement[midCodeIndex].op,midElement[midCodeIndex].left_Num,midElement[midCodeIndex].right_Num,midElement[midCodeIndex].result);
    midCodeIndex++;
}

int isInSymbolTab(int level,char *symName){
    int i=0;
    for(i=symbolTabAdress-1;i>=0;i--){
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

void genNewSwitch(char *label){
    sprintf(label,"switch%d",switchIndex);
    switchIndex++;
}

void getSymbol(){
    strcpy(symbolName,symbolArray[symbolIndex]);
    symbolType = symtype[symbolIndex];
    ////printf("%s ",symbolName);
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
    if(symbolType != VOIDSYM)
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
            error(idMissHead_ERROR,symbolLine[symbolIndex-1]);
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
            genMidCode("func",typeString,identName,"()");
            getSymbol();
            if(symbolType == Lparen){
            LparenLOOP:
                paraTab();
                element[address].para = paraNum;
                if(symbolType == Rparen){
                RparenLOOP:
                    getSymbol();
                    if(symbolType == LBRACE){
                        RBRACELOOP:
                            comStatement();
                            if(symbolType == RBRACE){
                                return;
                            }
                            else{
                                error(rightBparSymMis_ERROR,symbolLine[symbolIndex-1]);
                                flashBack(1);
                                return;
                            }
                    }
                    else{
                        error(leftBparSymMis_ERROR,symbolLine[symbolIndex-1]);
                        flashBack(1);
                        goto RBRACELOOP;
                    }
                }
                else{
                    error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
                    flashBack(1);
                    goto RparenLOOP;
                    //printf("This is ERROR,no Rparen\n");
                }
            }
            else{
                error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
                flashBack(1);
                goto LparenLOOP;
                //printf("This is ERROR,no Lparen\n");
            }
        }
        else{
            error(symbolMissAfterAssign_ERROR,symbolLine[symbolIndex-1]);
            do{
                getSymbol();
            }while(symbolType != RBRACE);
            return ;
        }
    }
    else{
        error(idMissHead_ERROR,symbolLine[symbolIndex-1]);
        do{
            getSymbol();
        }while(symbolType != RBRACE);
        return ;
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
            LparenLoop:
                paraTab();
                element[address].para = paraNum;
                if(symbolType == Rparen){
                RparenLoop:
                    getSymbol();
                    if(symbolType == LBRACE){
                    LBRACELoop:
                        comStatement();
                        if(symbolType == RBRACE){
                            genMidCode("ret","","","0");
                            return;
                            //printf("This is unreturnFuncDef\n");
                        }
                        else{
                            error(rightBparSymMis_ERROR,symbolLine[symbolIndex-1]);
                            flashBack(1);
                            return;
                            //printf("This is ERROR,no RBRACE\n");
                        }
                    }
                    else{
                        error(leftBparSymMis_ERROR,symbolLine[symbolIndex-1]);
                        flashBack(1);
                        goto LBRACELoop;
                        //printf("This is ERROR,no LBRACE\n");
                    }
                }
                else{
                    error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
                    flashBack(1);
                    goto RparenLoop;
                }
            }
            else{
                error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
                flashBack(1);
                goto LparenLoop;
            }
        }
        else{
            error(symbolMissAfterAssign_ERROR,symbolLine[symbolIndex-1]);
            do{
                getSymbol();
            }while(symbolType != RBRACE);
            return ;
        }
    }
    else{
        error(voidMiss_ERROR,symbolLine[symbolIndex-1]);
        do{
            getSymbol();
        }while(symbolType != RBRACE);
        return ;
    }
}

void mainFunc(){
    level++;
    getSymbol();
    if(symbolType == VOIDSYM){
    VOIDSYMLoop:
        getSymbol();
        if(symbolType == MAINSYM){
        MAINSYMLoop:
            getSymbol();
            if(symbolType == Lparen){
            LparenLoop:
                getSymbol();
                if(symbolType == Rparen){
                RparenLoop:
                    getSymbol();
                    genMidCode("func","void","main","()");
                    if(symbolType == LBRACE){
                    LBRACELoop:
                        comStatement();
                        if(symbolType == RBRACE){
                            //printf("This is mainFunc\n");
                        }
                        else{
                            error(rightBparSymMis_ERROR,symbolLine[symbolIndex-1]);
                            flashBack(1);
                            //printf("This is ERROR,no RBRACE\n");
                        }
                    }
                    else{
                        error(leftBparSymMis_ERROR,symbolLine[symbolIndex-1]);
                        flashBack(1);
                        goto LBRACELoop;
                    }
                }
                else{
                    error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
                    flashBack(1);
                    goto RparenLoop;
                }
            }
            else{
                error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
                flashBack(1);
                goto LparenLoop;
            }
        }
        else{
            error(mainSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto MAINSYMLoop;
        }
    }
    else{
        error(voidMiss_ERROR,symbolLine[symbolIndex-1]);
        flashBack(1);
        goto VOIDSYMLoop;
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
                            getSymbol();
                        }
                        else{
                            error(afterOPNumMis_ERROR,symbolLine[symbolIndex-1]);
                            do{
                                getSymbol();
                            }while(symbolType != COMMA);
                        }
                    }
                    else if(symbolType == MINUS){
                        getSymbol();
                        if(symbolType == UNSIGNEDINTGER){
                            insertSymbolTab(identName,0,(0-atoi(symbolName)),0,level);
                            sprintf(result,"-%s",symbolName);
                            genMidCode("const","int",identName,result);
                            getSymbol();
                        }
                        else{
                            error(afterOPNumMis_ERROR,symbolLine[symbolIndex-1]);
                            do{
                                getSymbol();
                            }while(symbolType != COMMA);
                        }
                    }
                    else if(symbolType == UNSIGNEDINTGER){
                        insertSymbolTab(identName,0,atoi(symbolName),0,level);
                        genMidCode("const","int",identName,symbolName);
                        getSymbol();
                    }
                    else{
                        error(afterOPNumMis_ERROR,symbolLine[symbolIndex-1]);
                        do{
                            getSymbol();
                        }while(symbolType != COMMA);
                    }
                }
                else{
                    error(assignMiss_ERROR,symbolLine[symbolIndex-1]);
                    do{
                        getSymbol();
                    }while(symbolType != COMMA);
                }
            }
            else{
                error(symbolMissAfterConst_ERROR,symbolLine[symbolIndex-1]);
                do{
                    getSymbol();
                }while(symbolType != COMMA);
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
                        getSymbol();
                    }
                    else{
                        error(symbolMissAfterConst_ERROR,symbolLine[symbolIndex-1]);
                        do{
                            getSymbol();
                        }while(symbolType != COMMA);
                    }
                }
                else{
                    error(assignMiss_ERROR,symbolLine[symbolIndex-1]);
                    do{
                        getSymbol();
                    }while(symbolType != COMMA);
                }
            }
            else{
                error(symbolMissAfterConst_ERROR,symbolLine[symbolIndex-1]);
                do{
                    getSymbol();
                }while(symbolType != COMMA);
            }
        }while(symbolType == COMMA);
    }
    else{
        return;
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
                            error(rightMparSymMis_ERROR,symbolLine[symbolIndex-1]);
                            do{
                                getSymbol();
                            }while(symbolType != COMMA);
                        }
                    }
                    else {
                        error(afterOPNumMis_ERROR,symbolLine[symbolIndex-1]);
                        do{
                            getSymbol();
                        }while(symbolType != COMMA);
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
                error(symbolMissAfterAssign_ERROR,symbolLine[symbolIndex-1]);
                do{
                    getSymbol();
                }while(symbolType != COMMA);
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
                    error(symbolMissAfterAssign_ERROR,symbolLine[symbolIndex-1]);
                    do{
                        getSymbol();
                    }while(symbolType != COMMA);
                }
            }
            else{
                error(idMissHead_ERROR,symbolLine[symbolIndex-1]);
                do{
                    getSymbol();
                }while(symbolType != COMMA);
            }
        }
        while(symbolType == COMMA);
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
        error(Statement_ERROR,symbolLine[symbolIndex-1]);
        flashBack(1);
    }
}

void stateColumn(){
    while(symbolType == IFSYM || symbolType == WHILESYM || symbolType == LBRACE || symbolType == IDENT || symbolType == IFSYM ||
    symbolType == SCANFSYM || symbolType == PRINTFSYM || symbolType == SEMI || symbolType == SWITCHSYM ||symbolType == RETURNSYM){
        Statement();
        getSymbol();
    }
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
            return;
        }
        else{
            error(rightBparSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            return;
        }
    }
    else if(symbolType == IDENT){
        int i=0;
        int flag = 0;
        int type;
        i = isInSymbolTab(level,symbolName);
        if(i == -1){
            i = isInSymbolTab(0,symbolName);
            if(i == -1){
                error(unDefIdent_ERROR,symbolLine[symbolIndex-1]);
                do{
                    getSymbol();
                }while(symbolType != SEMI);
                return;
            }
            type = element[i].type;
        }
        else{
            type = element[i].type;
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
                return;
            }
            else{
                error(semicSymMis_ERROR,symbolLine[symbolIndex-1]);
                flashBack(1);
                return;
            }
        }
        else if(flag == 2){
            unreFuncCall();
            getSymbol();
            if(symbolType == SEMI){
                return;
            }
            else{
                error(semicSymMis_ERROR,symbolLine[symbolIndex-1]);
                flashBack(1);
                return;
            }
        }
        else{
            assignState(type);
            if(symbolType == SEMI){
                return;
            }
            else{
                error(semicSymMis_ERROR,symbolLine[symbolIndex-1]);
                flashBack(1);
                return;
            }
        }
    }
    else if(symbolType == SCANFSYM){
        scanfState();
        getSymbol();
        if(symbolType == SEMI){
            return;
        }
        else{
            error(semicSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            return;
        }
    }
    else if(symbolType == PRINTFSYM){
        printfState();
        getSymbol();
        if(symbolType == SEMI){
            return;
        }
        else{
            error(semicSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            return;
        }
    }
    else if(symbolType == SEMI){
    }
    else if(symbolType == SWITCHSYM){
        char switchLabel[500];
        genNewSwitch(switchLabel);
        switchState(switchLabel);
        genMidCode(switchLabel,":","","");
    }
    else if(symbolType == RETURNSYM){
        returnState();
        getSymbol();
        if(symbolType == SEMI){
            return;
        }
        else{
            error(semicSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            return;
        }
    }
    else{
        error(Statement_ERROR,symbolLine[symbolIndex-1]);
        flashBack(1);
        return;
    }
}

void conditionState(){
    char varTemp1[500];
    char varTemp2[500];
    char opType[5];
    char label1[500];
    char label2[500];
    char conditionLabel[500];
    getSymbol();
    if(symbolType == Lparen){
    LparenLoop:
        getSymbol();
        condition(varTemp1,varTemp2,opType,conditionLabel);
        if(symbolType == Rparen){
        RparenLoop:
            getSymbol();
            genNewLable(label1);
            genMidCode("BNZ","","",label1);
            genNewLable(label2);
            genMidCode("BZ","","",label2);
            genMidCode(label1,":","","");
            Statement();
            genMidCode(label2,":","","");
        }
        else{
            error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto RparenLoop;
        }
    }
    else{
        error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
        flashBack(1);
        goto LparenLoop;
    }

}

void loopState(){
    char varTemp1[500];
    char varTemp2[500];
    char opType[5];
    char label1[500];
    char label2[500];
    char conditionLabel[500];
    getSymbol();
    if(symbolType == Lparen){
    LparenLoop:
        getSymbol();
        condition(varTemp1,varTemp2,opType,conditionLabel);
        if(symbolType == Rparen){
        RparenLoop:
            getSymbol();
            genNewLable(label1);
            genMidCode("BNZ","","",label1);
            genNewLable(label2);
            genMidCode("BZ","","",label2);
            genMidCode(label1,":","","");
            Statement();
            genMidCode("j","","",conditionLabel);
            genMidCode(label2,":","","");
        }
        else{
            error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto RparenLoop;
        }
    }
    else{
        error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
        flashBack(1);
        goto LparenLoop;
    }
}

void reFuncCall(){
    char funcName[500];
    strcpy(funcName,symbolName);
    getSymbol();
    if(symbolType == Lparen){
    LparenLoop:
        getSymbol();
        valueParaTab(funcName);
        if(symbolType == Rparen){
            genMidCode("call","","",funcName);
        }
        else{
            error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
        }
    }
    else{
        error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
        flashBack(1);
        goto LparenLoop;
    }
}

void unreFuncCall(){
    char funcName[500];
    strcpy(funcName,symbolName);
    getSymbol();
    if(symbolType == Lparen){
    LparenLoop:
        getSymbol();
        valueParaTab(funcName);
        if(symbolType == Rparen){
            genMidCode("call","","",funcName);
        }
        else{
            error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
        }
    }
    else{
        error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
        flashBack(1);
        goto LparenLoop;
    }
}

void assignState(int type){
    char identName[500];
    char varTemp[500];
    strcpy(identName,symbolName);
    getSymbol();
    if(symbolType == ASSIGN){
        getSymbol();
        expression(varTemp);
        if(type != expressionType){
            error(assignState_ERROR,symbolLine[symbolIndex-1]);
        }
        //printf("This is assignState\n");
        genMidCode("=","",varTemp,identName);
    }
    else if(symbolType == LPAREN){
        getSymbol();
        expression(varTemp);
        if(symbolType == RPAREN){
        RPARENLoop:
            sprintf(identName,"%s[%s]",identName,varTemp);
            getSymbol();
            if(symbolType == ASSIGN){
            ASSIGNLoop:
                getSymbol();
                expression(varTemp);
                if(type != expressionType){
                    error(assignState_ERROR,symbolLine[symbolIndex-1]);
                }
                genMidCode("=","",varTemp,identName);
            }
            else{
                error(assignMiss_ERROR,symbolLine[symbolIndex-1]);
                flashBack(1);
                goto ASSIGNLoop;
            }
        }
        else{
            error(rightMparSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto RPARENLoop;
        }
    }
    else{
        error(assignState_ERROR,symbolLine[symbolIndex-1]);
        flashBack(1);
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
                error(symbolMissAfterAssign_ERROR,symbolLine[symbolIndex-1]);
                do{
                    getSymbol();
                }while(symbolType != COMMA);
            }
        }while(symbolType == COMMA);
    }
    else{
        error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
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
                    error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
                }
            }
            else if(symbolType == Rparen){
                genMidCode("printf",stringName,"","");
            }
            else{
                error(commaMiss_ERROR,symbolLine[symbolIndex-1]);
            }
        }
        else{
            expression(tempVar);
            if(symbolType == Rparen){
                genMidCode("printf","",tempVar,"");
            }
            else{
                error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
                //printf("This is ERROR,no Rparen\n");
            }
        }
    }
    else{
        error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
        //printf("This is ERROR,no Lparen\n");
    }

}

void switchState(char *switchLabel){
    caselevel++;
    char levelofcase[500];
    char varTemp[500];
    int defaultlevel;
    getSymbol();
    if(symbolType == Lparen){
    LparenLoop:
        getSymbol();
        expression(varTemp);
        if(symbolType == Rparen){
        RparenLoop:
            getSymbol();
            if(symbolType == LBRACE){
            LBRACELoop:
                getSymbol();
                defaultlevel = caselevel;
                caseTab(varTemp,switchLabel);
                if(symbolType == DEFUALTSYM){
                    sprintf(levelofcase,"%d",defaultlevel);
                    genMidCode("default",":","",levelofcase);
                    defaultFunc(switchLabel);
                    //printf("This is switchState\n");
                }
                else{
                    sprintf(levelofcase,"%d",defaultlevel);
                    genMidCode("default",":","",levelofcase);
                }
            }
            else{
                error(leftBparSymMis_ERROR,symbolLine[symbolIndex-1]);
                flashBack(1);
                goto LBRACELoop;
            }
        }
        else{
            error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto RparenLoop;
        }
    }
    else{
        error(leftParSymMis_ERROR,symbolLine[symbolIndex-1]);
        flashBack(1);
        goto LparenLoop;
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
        }
        else{
            error(rightParSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
        }
    }
    else if(symbolType == SEMI){
        genMidCode("ret","","","0");
        flashBack(1);
    }
    else{
        error(Statement_ERROR,symbolLine[symbolIndex-1]);
    }
}

void condition(char *varTemp1,char *varTemp2,char *opType,char *conditionLabel){
    genNewLable(conditionLabel);
    genMidCode(conditionLabel,":","","");
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
        genMidCode(opType,varTemp1,varTemp2,"");
    }
    else{
        genMidCode(opType,varTemp1,"0","");
    }
}

void valueParaTab(char *name){
    char varTemp[500];
    int para = 0;
    if(symbolType == Rparen){
    }
    else{
        expression(varTemp);
        genMidCode("push","","",varTemp);
        para++;
        while(symbolType == COMMA){
            getSymbol();
            expression(varTemp);
            genMidCode("push","","",varTemp);
            para++;
        }
    }
    if(para != element[isInSymbolTab(0,name)].para){
        error(paraNum_ERROR,symbolLine[symbolIndex-1]);
    }
}

void caseTab(char *varTemp,char *switchLabel){
    caseChildState(varTemp,switchLabel);
    getSymbol();
    while(symbolType == CASESYM){
        caseChildState(varTemp,switchLabel);
        getSymbol();
    }
}

void caseChildState(char *varTemp,char *switchLabel){
    char label1[500];
    char label2[500];
    char levelofcase[500];
    char opType = ' ';
    getSymbol();
    sprintf(levelofcase,"%d",caselevel);
    genMidCode("isCase","","",levelofcase);
    if(symbolType == CHAR){
        genMidCode("==",varTemp,symbolName,"");
        getSymbol();
        if(symbolType == COLON){
        COLONLoop:
            getSymbol();
            genNewLable(label1);
            genMidCode("BNZ","","",label1);
            genNewLable(label2);
            genMidCode("BZ","","",label2);
            genMidCode(label1,":","","");
            Statement();
            genMidCode("j","","",switchLabel);
            genMidCode(label2,":","","");
            //printf("This is caseChildState\n");
        }
        else{
            error(colonSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto COLONLoop;
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
            COLONLoop1:
                getSymbol();
                genNewLable(label1);
                genMidCode("BNZ","","",label1);
                genNewLable(label2);
                genMidCode("BZ","","",label2);
                genMidCode(label1,":","","");
                Statement();
                genMidCode("j","","",switchLabel);
                genMidCode(label2,":","","");
            }
            else{

            }
        }
        else{
            error(colonSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto COLONLoop1;
        }
    }
    else if(symbolType == UNSIGNEDINTGER){
        genMidCode("==",varTemp,symbolName,"");
        getSymbol();
        if(symbolType == COLON){
        COLONLoop2:
            getSymbol();
            genNewLable(label1);
            genMidCode("BNZ","","",label1);
            genNewLable(label2);
            genMidCode("BZ","","",label2);
            genMidCode(label1,":","","");
            Statement();
            genMidCode("j","","",switchLabel);
            genMidCode(label2,":","","");
            //printf("This is caseChildState\n");
        }
        else{
            error(colonSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto COLONLoop2;
        }
    }
    else{
        error(constMis_ERROR,symbolLine[symbolIndex-1]);
    }
}

void defaultFunc(char *switchLabel){
    getSymbol();
    if(symbolType == COLON){
        getSymbol();
        if(symbolType == RBRACE){
        }
        else{
            Statement();
            genMidCode("j","","",switchLabel);
            getSymbol();
            //printf("This is defaultState\n");
        }
    }
    else{
        error(colonSymMis_ERROR,symbolLine[symbolIndex-1]);
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
    int op = 0;
    if(symbolType == IDENT){
        expressionType = element[isInSymbolTab(level,symbolName)].type;
        strcpy(ident,symbolName);
        getSymbol();
        if(symbolType == LPAREN){
            getSymbol();
            expression(varTemp);
            if(symbolType == RPAREN){
            RPARENLoop:
                sprintf(ident,"%s[%s]",ident,varTemp);
                getSymbol();
            }
            else{
                error(rightMparSymMis_ERROR,symbolLine[symbolIndex-1]);
                flashBack(1);
                goto RPARENLoop;
            }
        }
        else if(symbolType == Lparen){
            flashBack(1);
            if(isInSymbolTab(0,symbolName) != -1){
            ReFuncLoop:
                expressionType = element[isInSymbolTab(level,symbolName)].type;
                reFuncCall();
                sprintf(varTemp,"$t%d",varIndex);
                varIndex++;
                genMidCode("funcRet","","",varTemp);
                strcpy(ident,varTemp);
                getSymbol();
            }
            else{
                error(unDefFunc_ERROR,symbolLine[symbolIndex-1]);
                goto ReFuncLoop;
            }
        }
        else{
        }
    }
    else if(symbolType == Lparen){
    LparenLoop:
        getSymbol();
        expression(varTemp);
        if(symbolType == Rparen){
            strcpy(ident,varTemp);
            getSymbol();
        }
        else{
            error(rightMparSymMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto LparenLoop;
        }
    }
    else if(symbolType == CHAR){
        expressionType = 3;
        strcpy(ident,symbolName);
        getSymbol();
    }
    else if(symbolType == UNSIGNEDINTGER){
        expressionType = 2;
        strcpy(ident,symbolName);
        getSymbol();
    }
    else if(symbolType == ADD || symbolType == MINUS){
        expressionType = 2;
        if(symbolType == MINUS){
            op = 1;
        }
        getSymbol();
        if(symbolType == UNSIGNEDINTGER){
        UNSIGNEDINTGERLoop:
            if(op){
                sprintf(ident,"-%s",symbolName);
            }
            else{
                strcpy(ident,symbolName);
            }
            getSymbol();
        }
        else{
            error(afterOPNumMis_ERROR,symbolLine[symbolIndex-1]);
            flashBack(1);
            goto UNSIGNEDINTGERLoop;
        }
    }
    else{
        error(afterOPNumMis_ERROR,symbolLine[symbolIndex-1]);
    }
}
