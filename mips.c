#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Compiler.h"

int index = 0;
int newlevel = 0;
int lastCaseLevel = -1;
int functionType = 0;
int returnType = 0;
char String[500][500];

int isRegister(char *str){
    int i,length;
    length = strlen(str);
    for(i=0;i<length;i++){
        if(str[i] == '$')
            return 1;
    }
    return 0;
}

int Array(char *str){
    int i,length;
    length = strlen(str);
    for(i=0;i<length;i++){
        if(str[i] == '[')
            return i;
    }
    return 0;
}

int isSwitchLabel(char *str){
    if(str[0] == 's' && str[1] == 'w' && str[2] == 'i' && str[3] == 't' && str[4] == 'c' && str[5] == 'h'){
        return 1;
    }
    return 0;
}

int isLabel(char *str){
    if(str[0] == 'l' && str[1] == 'a' && str[2] == 'b' && str[3] == 'e' && str[4] == 'l'){
        return 1;
    }
    return 0;
}

int isInString(char *str){
    int i;
    for(i=0;i<stringCount;i++){
        if(!strcmp(str,String[i])){
            return i;
        }
    }
    return -1;
}

int isDigit(char *str){
    int i=0;
    int length;
    length = strlen(str);
    for(i=1;i<length;i++){
        if(str[i] < '0' || str[i] > '9'){
            return 0;
        }
    }
    if(str[0] == '+' || str[0] == '-' || isdigit(str[0]))
        return 1;
    else
        return 0;
}

void dealExpression(FILE *f,char *data,char *left_num,char *right_num,int type){
    int address;
    int minus_flag = 0,array_minus_flag = 0;
    int isArray = 0;
    int i,length;
    char num[500];
    char arrayName[500];
    char arrayIndex[500];
    memset(num,0,500);
    memset(arrayName,0,500);
    memset(arrayIndex,0,500);
    minus_flag = (right_num[0] == '-') ? 1 : 0;
    length = strlen(right_num);
    if(right_num[0] == '+' || right_num[0] == '-'){
        for(i=0;i<length-1;i++){
            num[i] = right_num[i+1];
        }
    }//删除+,-
    else{
        strcpy(num,right_num);
    }
    //printf("%s\n",num);
    length = strlen(num);
    if(!isDigit(num)){
        for(i=0;i<length;i++){
            if(num[i] == '['){
                int j;
                isArray = 1;
                for(j=0;j<i;j++){
                    arrayName[j] = num[j];
                }
                for(j=0;num[j+i+1]!=']';j++){
                    if(num[j+i+1] == '+' || num[j+i+1] == '-'){
                        array_minus_flag = (num[j+i-1] == '-') ? 1 : 0;
                        j--;
                        i++;
                        continue;
                    }//删除数组下标中的+,-
                    arrayIndex[j] = num[j+i+1];
                }
                break;
            }
        }
        if(isArray){
            int indexLength;
            int arrayType;
            indexLength = strlen(arrayIndex);
            address = isInSymbolTab(newlevel,arrayName);
            arrayType = (element[address].type == 2) ? 1 : 0;//1为int 2为char
            if(!isDigit(arrayIndex)){
                if(isInSymbolTab(newlevel,arrayIndex) == -1){
                    for(i=0;i<indexLength;i++){
                        if(arrayIndex[i] == '\''){
                            address += (int)arrayIndex[i+1];
                            address *= 4;
                            fprintf(f,"lw $t1 %s+%d\n",data,address);
                            break;
                        }//是字符
                        if(arrayIndex[i] == '$'){
                            fprintf(f,"addi $sp $sp 4\n");
                            fprintf(f,"lw $t1 0($sp)\n");
                            fprintf(f,"mult $t1 $s6\n");
                            fprintf(f,"mflo $t1\n");
                            fprintf(f,"addi $t1 $t1 %d\n",4*address);
                            fprintf(f,"lw $t1 %s($t1)\n",data);
                            break;
                        }
                    }//是t
                }
                else{
                    fprintf(f,"lw $t1 %s+%d\n",data,4*isInSymbolTab(newlevel,arrayIndex));
                    if(array_minus_flag){
                        fprintf(f,"sub $t1 $zero $t1\n");
                    }
                    fprintf(f,"mult $t1 $s6\n");
                    fprintf(f,"mflo $t1\n");
                    fprintf(f,"addi $t1 $t1 %d\n",4*address);
                    fprintf(f,"lw $t1 %s($t1)\n",data);
                }//是标识符
            }
            else{
                address += atoi(arrayIndex);
                address *= 4;
                fprintf(f,"lw $t1 %s+%d\n",data,address);
            }//全部是数字
            if(type){
                if(arrayType){
                    fprintf(f,"li $v0 1\n");
                    fprintf(f,"move $a0 $t1\n");
                    fprintf(f,"syscall\n");
                }
                else{
                    fprintf(f,"li $v0 11\n");
                    fprintf(f,"move $a0 $t1\n");
                    fprintf(f,"syscall\n");
                }
            }
        }
        else{
            int varType = 1;
            if(isInSymbolTab(newlevel,num) == -1){
                for(i=0;i<length;i++){
                    if(num[i] == '\''){
                        fprintf(f,"li $t1 %d\n",(int)num[i+1]);
                        varType = 0;
                        if(minus_flag)
                            fprintf(f,"sub $t1 $zero $t1\n");
                        break;
                    }
                    if(num[i] == '$'){
                        varType = 1;
                        fprintf(f,"addi $sp $sp 4\n");
                        fprintf(f,"lw $t1 0($sp)\n");
                        if(returnType)
                            varType = 0;
                        if(minus_flag)
                            fprintf(f,"sub $t1 $zero $t1\n");
                        break;
                    }
                }
            }
            else{
                fprintf(f,"lw $t1 %s+%d\n",data,4*element[isInSymbolTab(newlevel,num)].address);
                if(element[isInSymbolTab(newlevel,num)].type == 3 || element[isInSymbolTab(newlevel,num)].type == 1 || element[isInSymbolTab(newlevel,num)].type == 8)
                    varType = 0;
                if(minus_flag)
                    fprintf(f,"sub $t1 $zero $t1\n");
            }
            if(type && length != 0){
                if(varType){
                    fprintf(f,"li $v0 1\n");
                    fprintf(f,"move $a0 $t1\n");
                    fprintf(f,"syscall\n");
                }
                else{
                    fprintf(f,"li $v0 11\n");
                    fprintf(f,"move $a0 $t1\n");
                    fprintf(f,"syscall\n");
                }
            }
        }
    }
    else{
        fprintf(f,"li $t1 %d\n",atoi(num));
        if(minus_flag)
            fprintf(f,"sub $t1 $zero $t1\n");
        if(type){
            fprintf(f,"li $v0 1\n");
            fprintf(f,"move $a0 $t1\n");
            fprintf(f,"syscall\n");
        }
    }//全部是数字

    memset(arrayName,0,500);
    memset(arrayIndex,0,500);
    memset(num,0,500);
    isArray = 0;
    minus_flag = (left_num[0] == '-') ? 1 : 0;
    length = strlen(left_num);
    if(left_num[0] == '+' || left_num[0] == '-'){
        for(i=0;i<length-1;i++){
            num[i] = left_num[i+1];
        }
    }//删除+,-
    else{
        strcpy(num,left_num);
    }
    length = strlen(num);
    if(!isDigit(num)){
        for(i=0;i<length;i++){
            if(num[i] == '['){
                int j;
                isArray = 1;
                for(j=0;j<i;j++){
                    arrayName[j] = num[j];
                }
                for(j=0;num[j+i+1]!=']';j++){
                    if(num[j+i+1] == '+' || num[j+i+1] == '-'){
                        array_minus_flag = (num[j+i-1] == '-') ? 1 : 0;
                        j--;
                        i++;
                        continue;
                    }//删除数组下标中的+,-
                    arrayIndex[j] = num[j+i+1];
                }
                break;
            }
        }
        if(isArray){
            int indexLength;
            indexLength = strlen(arrayIndex);
            address = isInSymbolTab(newlevel,arrayName);
            if(!isDigit(arrayIndex)){
                if(isInSymbolTab(newlevel,arrayIndex) == -1){
                    for(i=0;i<indexLength;i++){
                        if(arrayIndex[i] == '\''){
                            address += (int)arrayIndex[i+1];
                            address *= 4;
                            fprintf(f,"lw $t0 %s+%d\n",data,address);
                            break;
                        }//是字符
                        if(arrayIndex[i] == '$'){
                            fprintf(f,"addi $sp $sp 4\n");
                            fprintf(f,"lw $t0 0($sp)\n");
                            fprintf(f,"mult $t0 $s6\n");
                            fprintf(f,"mflo $t0\n");
                            fprintf(f,"addi $t0 $t0 %d\n",4*address);
                            fprintf(f,"lw $t0 %s($t0)\n",data);
                            break;
                        }//是t
                    }
                }
                else{
                    fprintf(f,"lw $t0 %s+%d\n",data,4*isInSymbolTab(newlevel,arrayIndex));
                    if(array_minus_flag){
                        fprintf(f,"sub $t0 $zero $t0\n");
                    }
                    fprintf(f,"mult $t0 $s6\n");
                    fprintf(f,"mflo $t0\n");
                    fprintf(f,"addi $t0 $t0 %d\n",4*address);
                    fprintf(f,"lw $t0 %s($t0)\n",data);
                }//是标识符
            }
            else{
                address += atoi(arrayIndex);
                address *= 4;
                fprintf(f,"lw $t0 %s+%d\n",data,address);
            }//全部是数字
        }
        else{
            if(isInSymbolTab(newlevel,num) == -1){
                for(i=0;i<length;i++){
                    if(num[i] == '\''){
                        fprintf(f,"li $t0 %d\n",(int)num[i+1]);
                        if(minus_flag)
                            fprintf(f,"sub $t0 $zero $t0\n");
                        break;
                    }
                    if(num[i] == '$'){
                        fprintf(f,"addi $sp $sp 4\n");
                        fprintf(f,"lw $t0 0($sp)\n");
                        if(minus_flag)
                            fprintf(f,"sub $t0 $zero $t0\n");
                        break;
                    }
                }
            }
            else{
                fprintf(f,"lw $t0 %s+%d\n",data,4*element[isInSymbolTab(newlevel,num)].address);
                if(minus_flag)
                    fprintf(f,"sub $t0 $zero $t0\n");
            }
        }
    }
    else{
        fprintf(f,"li $t0 %d\n",atoi(num));
        if(minus_flag)
            fprintf(f,"sub $t0 $zero $t0\n");
    }//全部是数字
}

void dealCondition(FILE *f,char *data,char *left_num,char *right_num,int isCase,int caseLevel){
    dealExpression(f,data,left_num,right_num,0);
    if(isCase){
        int i;
        int flag = 0;
        for(i=0;i<strlen(left_num);i++){
            if(left_num[i] == '$'){
                flag = 1;
                break;
            }
        }
        i = index;
        if(caseLevel <= lastCaseLevel && !flag){
            fprintf(f,"addi $sp $sp 4\n");
            fprintf(f,"lw $t0 0($sp)\n");
        }
        while(strcmp(midElement[i].op,"isCase") != 0 || (!strcmp(midElement[i].op,"isCase") && caseLevel != atoi(midElement[i].result))){
            i++;
            if(!strcmp(midElement[i].op,"default") && caseLevel == atoi(midElement[i].result)){
                break;
            }
        }
        if(!strcmp(midElement[i].op,"isCase")){
            lastCaseLevel = caseLevel;
            fprintf(f,"move $s7 $t0\n");
        }
    }
    fprintf(f,"sub $t0 $t0 $t1\n");
}

void getMips(FILE *f,char *data){
    int address = 0;
    int funcType = 0;
    int funcAddress = 0;
    int para = 0;
    int recursion = 0;//递归标识符
    int isCase = 0;//switch标志
    int push_Num = 0;
    int levelOfCase;
    char funcName[500];
    char conditionType[5];
    while(index <= midCodeIndex){
        if(!strcmp(midElement[index].op,"const")){
            address = isInSymbolTab(newlevel,midElement[index].right_Num);
            fprintf(f,"li $t1 %s\n",midElement[index].result);
            fprintf(f,"sw $t1 %s+%d\n",data,4*address);
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"func")){
            int numOfPara;
            int i;
            newlevel++;
            numOfPara = element[isInSymbolTab(newlevel,midElement[index].right_Num)].para;
            i = index+numOfPara;
            strcpy(funcName,midElement[index].right_Num);
            para = numOfPara;
            funcAddress = isInSymbolTab(newlevel,midElement[index].right_Num);
            if(!strcmp(midElement[index].left_Num,"void") && strcmp(midElement[index].right_Num,"main")!=0){
                funcType = 1;
            }
            if(newlevel == 1)
                fprintf(f,"j main\n");
            fprintf(f,"%s:\n",midElement[index].right_Num);
            while(i>index){
                fprintf(f,"addi $sp $sp 4\n");
                fprintf(f,"lw $t0 0($sp)\n");
                address = 4*isInSymbolTab(newlevel,midElement[i].result);
                fprintf(f,"sw $t0 %s+%d\n",data,address);
                i--;
            }//倒序取数
            if(strcmp(midElement[index].right_Num,"main") != 0){
                fprintf(f,"sw $ra 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"+")||!strcmp(midElement[index].op,"*")||!strcmp(midElement[index].op,"/")||!strcmp(midElement[index].op,"-")){
            if(!strcmp(midElement[index].op,"+")){
                dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num,0);
                fprintf(f,"add $t0 $t0 $t1\n");
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            if(!strcmp(midElement[index].op,"-")){
                dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num,0);
                fprintf(f,"sub $t0 $t0 $t1\n");
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            if(!strcmp(midElement[index].op,"*")){
                dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num,0);
                fprintf(f,"mult $t0 $t1\n");
                fprintf(f,"mflo $t0\n");
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            if(!strcmp(midElement[index].op,"/")){
                dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num,0);
                fprintf(f,"div $t0 $t1\n");
                fprintf(f,"mflo $t0\n");
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"=")){
            char arrayName[500];
            char arrayIndex[500];
            memset(arrayName,0,500);
            memset(arrayIndex,0,500);
            int i,j;
            dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num,0);
            if(Array(midElement[index].result)!=0){
                fprintf(f,"move $s5 $t1\n");
                for(j=0;j<Array(midElement[index].result);j++){
                    arrayName[j] = midElement[index].result[j];
                }
                j = Array(midElement[index].result)+1;
                for(i=j;midElement[index].result[i]!=']';i++){
                    arrayIndex[i-j] = midElement[index].result[i];
                }
                dealExpression(f,data,arrayIndex,"",0);
                fprintf(f,"move $t1 $s5\n");
                address = isInSymbolTab(newlevel,arrayName);
                fprintf(f,"mult $t0 $s6\n");
                fprintf(f,"mflo $t0\n");
                fprintf(f,"addi $t0 $t0 %d\n",4*address);
                fprintf(f,"sw $t1 %s($t0)\n",data);
            }
            else{
                address = 4*(isInSymbolTab(newlevel,midElement[index].result));
                fprintf(f,"sw $t1 %s+%d\n",data,address);
            }
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"push")){
            push_Num++;
            if(push_Num == 1){
                int i = index;
                while(strcmp(midElement[i].op,"call") != 0){
                    i++;
                }
                if(!strcmp(midElement[i].result,funcName)){
                    i = 1;
                    recursion = 1;
                }
                if(recursion){
                    if(isRegister(midElement[index].result)){
                        fprintf(f,"addi $sp $sp 4\n");
                        fprintf(f,"lw $s4 0($sp)\n");
                    }
                    while(i <= para){
                        address = 4*(funcAddress + i);
                        fprintf(f,"lw $t0 %s+%d\n",data,address);
                        fprintf(f,"sw $t0 0($sp)\n");
                        fprintf(f,"subi $sp $sp 4\n");
                        i++;
                    }
                    if(isRegister(midElement[index].result)){
                        fprintf(f,"sw $s4 0($sp)\n");
                        fprintf(f,"subi $sp $sp 4\n");
                    }
                }
            }
            //确认为递归
            dealExpression(f,data,"",midElement[index].result,0);
            fprintf(f,"sw $t1 0($sp)\n");
            fprintf(f,"subi $sp $sp 4\n");
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"call")){
            push_Num = 0;
            functionType = element[isInSymbolTab(0,midElement[index].result)].type;
            fprintf(f,"jal %s\n",midElement[index].result);
            if(recursion){
                int i = para;
                //printf("funcRetpara:%d",para);
                while(i>0){
                    fprintf(f,"addi $sp $sp 4\n");
                    fprintf(f,"lw $t0 0($sp)\n");
                    address = 4*(funcAddress+i);
                    fprintf(f,"sw $t0 %s+%d\n",data,address);
                    i--;
                }//倒序取数
            }
            recursion = 0;
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"ret")){
            int i;
            int length;
            length = strlen(midElement[index].result);
            /*fprintf(f,"addi $sp $sp 4\n");
            fprintf(f,"lw $ra 0($sp)\n");*/
            if(!funcType){
                if(isDigit(midElement[index].result)){
                    fprintf(f,"li $v0 %s\n",midElement[index].result);
                }
                else if(isInSymbolTab(newlevel,midElement[index].result) != -1){
                    address = 4*isInSymbolTab(newlevel,midElement[index].result);
                    fprintf(f,"lw $t0 %s+%d\n",data,address);
                    fprintf(f,"move $v0 $t0\n");
                }
                else{
                    for(i=0;i<length;i++){
                        if(midElement[index].result[i] == '\''){
                            fprintf(f,"li $v0 %s\n",midElement[index].result);
                            break;
                        }
                        if(midElement[index].result[i] == '$'){
                            if(midElement[index].result[i+1] == 't'){
                                fprintf(f,"addi $sp $sp 4\n");
                                fprintf(f,"lw $v0 0($sp)\n");
                            }
                            break;
                        }
                    }
                }
            }
            funcType = 0;
            if(strcmp(funcName,"main") != 0){
                fprintf(f,"addi $sp $sp 4\n");
                fprintf(f,"lw $ra 0($sp)\n");
                fprintf(f,"jr $ra\n");
            }
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,">")||!strcmp(midElement[index].op,"<")||!strcmp(midElement[index].op,">=")||!strcmp(midElement[index].op,"<=")
                || !strcmp(midElement[index].op,"!=") || !strcmp(midElement[index].op,"==")){
            dealCondition(f,data,midElement[index].left_Num,midElement[index].right_Num,isCase,levelOfCase);
            strcpy(conditionType,midElement[index].op);
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"BNZ")){
            if(!strcmp(conditionType,">"))
                fprintf(f,"bgt $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,"<"))
                fprintf(f,"blt $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,">="))
                fprintf(f,"bge $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,"<="))
                fprintf(f,"ble $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,"!="))
                fprintf(f,"bne $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,"=="))
                fprintf(f,"beq $t0 0 %s\n",midElement[index].result);
            else{
            }
            if(isCase){
                int i;
                i = index;
                while(strcmp(midElement[i].op,"isCase") != 0 || (!strcmp(midElement[i].op,"isCase") && levelOfCase != atoi(midElement[i].result))){
                    i++;
                    if(!strcmp(midElement[i].op,"default") && levelOfCase == atoi(midElement[i].result)){
                        break;
                    }
                }
                if(!strcmp(midElement[i].op,"isCase")){
                    fprintf(f,"sw $s7 0($sp)\n");
                    fprintf(f,"subi $sp $sp 4\n");
                }
                isCase = 0;
            }
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"BZ")){
            if(!strcmp(conditionType,">"))
                fprintf(f,"ble $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,"<"))
                fprintf(f,"bge $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,">="))
                fprintf(f,"blt $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,"<="))
                fprintf(f,"bgt $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,"!="))
                fprintf(f,"beq $t0 0 %s\n",midElement[index].result);
            else if(!strcmp(conditionType,"=="))
                fprintf(f,"bne $t0 0 %s\n",midElement[index].result);
            else{
            }
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"scanf")){
            int type;
            type = element[isInSymbolTab(newlevel,midElement[index].result)].type;
            address = 4*isInSymbolTab(newlevel,midElement[index].result);
            if(type == 2){
                fprintf(f,"li $v0 5\n");
                fprintf(f,"syscall\n");
                fprintf(f,"sw $v0 %s+%d\n",data,address);
            }//var_int
            else if(type == 3){
                fprintf(f,"li $v0 12\n");
                fprintf(f,"syscall\n");
                fprintf(f,"sw $v0 %s+%d\n",data,address);
            }//var_char
            else{
            }
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"printf")){
            if(isInString(midElement[index].left_Num) != -1){
                address = isInString(midElement[index].left_Num);
                fprintf(f,"li $v0 4\n");
                fprintf(f,"la $a0 string%d\n",address);
                fprintf(f,"syscall\n");
            }
            dealExpression(f,data,"",midElement[index].right_Num,1);
            fprintf(f,"li $s3 \'\\n\'\n");
            fprintf(f,"li $v0 11\n");
            fprintf(f,"move $a0 $s3\n");
            fprintf(f,"syscall\n");
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(isLabel(midElement[index].op)){
            fprintf(f,"%s:\n",midElement[index].op);
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(isSwitchLabel(midElement[index].op)){
            fprintf(f,"%s:\n",midElement[index].op);
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"funcRet")){
            if(functionType == 5){
                returnType = 1;
            }
            else{
                returnType = 0;
            }
            fprintf(f,"sw $v0 0($sp)\n");
            fprintf(f,"subi $sp $sp 4\n");
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"j")){
            fprintf(f,"%s %s\n",midElement[index].op,midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"isCase")){
            isCase = 1;
            levelOfCase = atoi(midElement[index].result);
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        else{
            fprintf(f,"#%s %s %s %s\n",midElement[index].op,midElement[index].left_Num,midElement[index].right_Num,midElement[index].result);
        }
        index++;
    }
}
