#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Compiler.h"

int index = 0;
int t_RegisterIndex = 0;
int v_RegisterIndex = 0;
int a_RegisterIndex = 0;
int s_RegisterIndex = 0;
char reg[500];//¼Ä´æÆ÷Êý×é

void dealExpression(char *data,char *left_num,char *right_num){
    int address;
    int flag = 0;
    int isArray = 0;
    flag = (left_num[0] == '-') ? 1 : 0;
    address = isInSymbolTab(num);
    if(address > -1){
        printf("lw $t0 data+%d\n",address);
        if(flag)
            printf("sub $t0 $zero $t0\n");
        }
     }
     else{
        if(num[0] == '$'){
            printf("addi $sp $sp 4\n");
            printf("lw $t0 0($sp)\n");
            printf()
        }
        else if(isdigit(num[0]))//unsignednum
            return 3;
        else if(num[0] == '\'')//char
            return 4;
        else{

        }
     }

}

void getMips(char *data){
    int address = 0;
    while(index <= midCodeIndex){
        if(!strcmp(midElement[index].op,"const")){
            address = isInSymbolTab(MidCode.right_Num);
            printf("li $t0 %s\n",MidCode.result);
            printf("sw $t0 %s+%d\n",data,4*address);
        }
        /*else if(!strcmp(midElement[index].op,"func")){
            address = 4*element[isInSymbolTab(MidCode.right_Num)].para;
            printf("%s:\n",MidCode.right_Num);
            if(!strcmp(MidCode.left_Num,"int")||(!strcmp(MidCode.left_Num,"char"))){
            }
            else{
            }
        }*/
        else if(!strcmp(midElement[index].op,"+")||!strcmp(midElement[index].op,"*")||!strcmp(midElement[index].op,"/")||!strcmp(midElement[index].op,"-")
                !strcmp(midElement[index].op,"push")){
            while(strcmp(midElement[index].op,"=") != 0){
                if(!strcmp(midElement[index].op,"+")){
                    int leftType,rightType;
                    leftType = numInTab(midElement[index].left_Num);
                    rightType = numInTab(midElement[index].right_Num);

                }

                index++;
            }
        }
        else if(!strcmp(midElement[index].op,"push")){

        }
        index++;
    }
}
