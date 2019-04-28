#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Compiler.h"

#define unKnownSym_ERROR 0//词法分析无法识别的符号
#define unDefFunc_ERROR 1//未定义函数
#define unDefIdent_ERROR 2 //未定义的标识符
#define unMatchSinQuoto_ERROR 3 //单引号不匹配
#define unMatchDouQuoto_ERROR 4 //双引号不匹配
#define funcNameConplitc_ERROR 5 //函数名冲突
#define varNameConplict_ERROR 6 //变量名冲突
#define idMissHead_ERROR 7 // 声明头部缺少标识符(缺int和char)
#define symbolMissAfterAssign_ERROR 8 // 在声明头部后方缺少标识符
#define symbolMissAfterConst_ERROR 9 // 常量定义之后缺少声明的标识符
#define constType_ERROR 10 // 常量类型错误
#define semicSymMis_ERROR 11 //缺失分号
#define varDefArrayIndex_ERROR 12 //数组的下标不为已经定义为数字的变量
#define varDefIdent_ERROR 13 // 变量定义标识符错误
#define mainSymMis_ERROR 14 //主函数缺失main
#define leftParSymMis_ERROR 15 // 缺失左括号’(’
#define rightParSymMis_ERROR 16 // 缺失右括号’)’
#define leftBparSymMis_ERROR 17 // 缺失左花括号’{’
#define rightBparSymMis_ERROR 18 // 缺失右花括号’}’
#define rightMparSymMis_ERROR 19 // 缺少右中括号’]’
#define afterOPNumMis_ERROR 20 // 在符号之后的数字或者标识符缺失
#define assignState_ERROR 21 // 赋值错误
#define Statement_ERROR 22 // 语句错误
#define conditionState_ERROR 23 // 条件错误
#define caseSymMis_ERROR 24 // 情况子语句缺少case
#define colonSymMis_ERROR 25 // 情况子语句或者缺省语句缺少冒号’:’
#define voidMiss_ERROR 26//缺少void
#define assignMiss_ERROR 27//缺少=
#define commaMiss_ERROR 28//缺少,
#define constMis_ERROR 29//缺少数字常量和字符常量
#define paraNum_ERROR 30

void error(int errorCode,int line){
    switch(errorCode){
    case unKnownSym_ERROR:
        printf("Unknow symbol in line:%d\n",line);
        break;
    case unDefFunc_ERROR:
        printf("This function is undefined in line:%d\n",line);
        break;
    case unDefIdent_ERROR:
        printf("This ident is undefined in line:%d\n",line);
        break;
    case unMatchSinQuoto_ERROR:
        printf("Unmatched \' in line:%d\n",line);
        break;
    case unMatchDouQuoto_ERROR:
        printf("Unmatched \" in line:%d\n",line);
        break;
    case funcNameConplitc_ERROR:
        printf("This function has been defined in line:%d\n",line);
        break;
    case varNameConplict_ERROR:
        printf("This ident has been defined in line:%d\n",line);
        break;
    case idMissHead_ERROR:
        printf("No int or char in line:%d\n",line);
        break;
    case symbolMissAfterAssign_ERROR:
        printf("No ident in line:%d\n",line);
        break;
    case symbolMissAfterConst_ERROR:
        printf("No ident in line:%d\n",line);
        break;
    case constType_ERROR:
        printf("Constant type is error in line:%d\n",line);
        break;
    case semicSymMis_ERROR:
        printf("No semi in line:%d\n",line);
        break;
    case varDefArrayIndex_ERROR:
        printf("Array index is not number in line:%d\n",line);
        break;
    case varDefIdent_ERROR:
        printf("Var ident is error in line:%d\n",line);
        break;
    case mainSymMis_ERROR:
        printf("No main in line:%d\n",line);
        break;
    case leftParSymMis_ERROR:
        printf("No ( in line:%d\n",line);
        break;
    case rightParSymMis_ERROR:
        printf("No ) in line:%d\n",line);
        break;
    case leftBparSymMis_ERROR:
        printf("No { in line:%d\n",line);
        break;
    case rightBparSymMis_ERROR:
        printf("No } in line:%d\n",line);
        break;
    case rightMparSymMis_ERROR:
        printf("No ] in line:%d\n",line);
        break;
    case afterOPNumMis_ERROR:
        printf("No number or ident in line:%d\n",line);
        break;
    case assignState_ERROR:
        printf("Assign statement error in line:%d\n",line);
        break;
    case Statement_ERROR:
        printf("Statement error in line:%d\n",line);
        break;
    case conditionState_ERROR:
        printf("Condition error in line:%d\n",line);
        break;
    case caseSymMis_ERROR:
        printf("No case in line:%d\n",line);
        break;
    case colonSymMis_ERROR:
        printf("No : in line:%d\n",line);
        break;
    case voidMiss_ERROR:
        printf("No void in line:%d\n",line);
        break;
    case assignMiss_ERROR:
        printf("No = in line:%d\n",line);
        break;
    case commaMiss_ERROR:
        printf("No , in line:%d\n",line);
        break;
    case constMis_ERROR:
        printf("No number or character in line:%d\n",line);
        break;
    case paraNum_ERROR:
        printf("paraNum error in line:%d\n",line);
        break;
    default:
        break;
    }
}
