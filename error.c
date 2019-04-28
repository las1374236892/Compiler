#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Compiler.h"

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
