#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Compiler.h"

int index = 0;
int newlevel = 0;
char String[500][500];

int Array(char *str){
    int i,length;
    length = strlen(str);
    for(i=0;i<length;i++){
        if(str[i] == '[')
            return i;
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
    for(i=0;i<length;i++){
        if(str[i] < '0' || str[i] > '9'){
            return 0;
        }
    }
    return 1;
}

void dealCondition(FILE *f,char *data,char *left_num,char *right_num){
    int i,left_length,right_length;
    int address;
    left_length = strlen(left_num);
    right_length = strlen(right_num);
    if(isDigit(right_num)){
        fprintf(f,"li $t1 %s\n",right_num);
    }
    else{
        for(i=0;i<right_length;i++){
            if(right_num[i] == '$'){
                fprintf(f,"addi $sp $sp 4\n");
                fprintf(f,"lw $t1 0($sp)\n");
                break;
            }
            if(isInSymbolTab(newlevel,right_num) != -1){
                address = 4*isInSymbolTab(newlevel,right_num);
                fprintf(f,"lw $t1 %s+%d\n",data,address);
                break;
            }
            if(right_num[i] == '\''){
                fprintf(f,"li $t1 %s\n",right_num);
            }
        }
    }
    if(isDigit(left_num)){
        fprintf(f,"li $t0 %s\n",left_num);
    }
    else{
        for(i=0;i<left_length;i++){
            if(left_num[i] == '$'){
                fprintf(f,"addi $sp $sp 4\n");
                fprintf(f,"lw $t0 0($sp)\n");
                break;
            }
            if(isInSymbolTab(newlevel,left_num) != -1){
                address = 4*isInSymbolTab(newlevel,left_num);
                fprintf(f,"lw $t0 %s+%d\n",data,address);
                break;
            }
            if(left_num[i] == '\''){
                fprintf(f,"li $t0 %s\n",left_num);
            }
        }
    }
    fprintf(f,"sub $t0 $t0 $t1\n");
}

void dealExpression(FILE *f,char *data,char *left_num,char *right_num){
    int address;
    int minus_flag = 0,array_minus_flag = 0;
    int isArray = 0;
    int i,length;
    char num[500];
    char arrayName[500];
    char arrayIndex[500];
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
                    if(array_minus_flag)
                        address -= element[isInSymbolTab(newlevel,arrayIndex)].value;
                    else
                        address += element[isInSymbolTab(newlevel,arrayIndex)].value;
                    address *= 4;
                    fprintf(f,"lw $t1 %s+%d\n",data,address);
                }//是标识符
            }
            else{
                address += atoi(arrayIndex);
                address *= 4;
                fprintf(f,"lw $t1 %s+%d\n",data,address);
            }//全部是数字
        }
        else{
            if(isInSymbolTab(newlevel,num) == -1){
                for(i=0;i<length;i++){
                    if(num[i] == '\''){
                        fprintf(f,"li $t1 %d\n",(int)num[i+1]);
                        if(minus_flag)
                            fprintf(f,"sub $t1 $zero $t1\n");
                        break;
                    }
                    if(num[i] == '$'){
                        fprintf(f,"addi $sp $sp 4\n");
                        fprintf(f,"lw $t1 0($sp)\n");
                        if(minus_flag)
                            fprintf(f,"sub $t1 $zero $t1\n");
                        break;
                    }
                }
            }
            else{
                fprintf(f,"lw $t1 %s+%d\n",data,4*element[isInSymbolTab(newlevel,num)].address);
                if(minus_flag)
                    fprintf(f,"sub $t1 $zero $t1\n");
            }
        }
    }
    else{
        fprintf(f,"li $t1 %d\n",atoi(num));
        if(minus_flag)
            fprintf(f,"sub $t1 $zero $t1\n");
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
                            fprintf(f,"lw $t0 %s+%d\n../16061023_test.txt",data,address);
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
                        }
                    }//是t
                }
                else{
                    if(array_minus_flag)
                        address -= element[isInSymbolTab(newlevel,arrayIndex)].value;
                    else
                        address += element[isInSymbolTab(newlevel,arrayIndex)].value;
                    address *= 4;
                    fprintf(f,"lw $t0 %s+%d\n",data,address);
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
                            fprintf(f,"sub $t0 $zero $t1\n");
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



void getMips(FILE *f,char *data){
    int address = 0;
    char conditionType[5];
    while(index <= midCodeIndex){
        if(!strcmp(midElement[index].op,"const")){
            address = isInSymbolTab(newlevel,midElement[index].right_Num);
            fprintf(f,"li $t1 %s\n",midElement[index].result);
            fprintf(f,"sw $t1 %s+%d\n",data,4*address);
        }
        else if(!strcmp(midElement[index].op,"func")){
            int numOfPara;
            int i;
            newlevel++;
            numOfPara = element[isInSymbolTab(newlevel,midElement[index].right_Num)].para;
            i = index+numOfPara;
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
            /*if(strcmp(midElement[index].right_Num,"main") != 0){
                fprintf(f,"sw $ra 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }*/
        }
        else if(!strcmp(midElement[index].op,"+")||!strcmp(midElement[index].op,"*")||!strcmp(midElement[index].op,"/")||!strcmp(midElement[index].op,"-")){
            if(!strcmp(midElement[index].op,"+")){
                dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num);
                fprintf(f,"add $t0 $t0 $t1\n");
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            if(!strcmp(midElement[index].op,"-")){
                dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num);
                fprintf(f,"sub $t0 $t0 $t1\n");
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            if(!strcmp(midElement[index].op,"*")){
                dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num);
                fprintf(f,"mult $t0 $t1\n");
                fprintf(f,"mflo $t0\n");
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            if(!strcmp(midElement[index].op,"/")){
                dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num);
                fprintf(f,"div $t0 $t1\n");
                fprintf(f,"mflo $t0\n");
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
        }
        else if(!strcmp(midElement[index].op,"=")){
            char arrayName[500];
            memset(arrayName,0,500);
            int j;
            dealExpression(f,data,midElement[index].left_Num,midElement[index].right_Num);
            if(Array(midElement[index].result)!=0){
                for(j=0;j<Array(midElement[index].result);j++){
                    arrayName[j] = midElement[index].result[j];
                }
                address = isInSymbolTab(newlevel,arrayName);
                fprintf(f,"addi $sp $sp 4\n");
                fprintf(f,"lw $t0 0($sp)\n");
                fprintf(f,"mult $t0 $s6\n");
                fprintf(f,"mflo $t0\n");
                fprintf(f,"addi $t0 $t0 %d\n",4*address);
                fprintf(f,"sw $t1 %s($t0)\n",data);
            }
            else{
                address = 4*(isInSymbolTab(newlevel,midElement[index].result));
                fprintf(f,"sw $t1 %s+%d\n",data,address);
            }
        }
        else if(!strcmp(midElement[index].op,"push")){
            /*int i;
            int funcAddress = 0;
            i = index;
            while(strcmp(midElement[i].op,"call") != 0){
                i++;
            }
            funcAddress = isInSymbolTab(0,midElement[i].result);
            address = 4*(funcAddress+element[funcAddress].para+1-i+index);*/
            dealExpression(f,data,"",midElement[index].result);
            fprintf(f,"sw $t1 0($sp)\n");
            fprintf(f,"subi $sp $sp 4\n");
        }
        else if(!strcmp(midElement[index].op,"call")){
            fprintf(f,"jal %s\n",midElement[index].result);
        }
        else if(!strcmp(midElement[index].op,"ret")){
            int i;
            int length;
            length = strlen(midElement[index].result);
            /*fprintf(f,"addi $sp $sp 4\n");
            fprintf(f,"lw $ra 0($sp)\n");*/
            if(isDigit(midElement[index].result)){
                fprintf(f,"li $t0 %s\n",midElement[index].result);
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            else if(isInSymbolTab(newlevel,midElement[index].result) != -1){
                address = 4*isInSymbolTab(newlevel,midElement[index].result);
                fprintf(f,"lw $t0 %s+%d\n",data,address);
                fprintf(f,"sw $t0 0($sp)\n");
                fprintf(f,"subi $sp $sp 4\n");
            }
            else{
                for(i=0;i<length;i++){
                    if(midElement[index].result[i] == '\''){
                        fprintf(f,"li $t0 %s\n",midElement[index].result);
                        fprintf(f,"sw $t0 0($sp)\n");
                        fprintf(f,"subi $sp $sp 4\n");
                        break;
                    }
                }
            }
            fprintf(f,"jr $ra\n");
        }
        else if(!strcmp(midElement[index].op,">")||!strcmp(midElement[index].op,"<")||!strcmp(midElement[index].op,">=")||!strcmp(midElement[index].op,"<=")
                || !strcmp(midElement[index].op,"!=") || !strcmp(midElement[index].op,"==")){
            dealCondition(f,data,midElement[index].left_Num,midElement[index].right_Num);
            strcpy(conditionType,midElement[index].op);
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
        }
        else if(!strcmp(midElement[index].op,"printf")){
            int length;
            int i;
            length = strlen(midElement[index].right_Num);
            if(isInString(midElement[index].left_Num) != -1){
                address = isInString(midElement[index].left_Num);
                fprintf(f,"li $v0 4\n");
                fprintf(f,"la $a0 string%d\n",address);
                fprintf(f,"syscall\n");
            }
            for(i=0;i<length;i++){
                if(midElement[index].right_Num[i] == '$'){
                    fprintf(f,"addi $sp $sp 4\n");
                    fprintf(f,"lw $t0 0($sp)\n");
                    fprintf(f,"li $v0 1\n");
                    fprintf(f,"move $a0 $t0\n");
                    fprintf(f,"syscall\n");
                    break;
                }
                if(isInSymbolTab(newlevel,midElement[index].right_Num) != -1){
                    address = isInSymbolTab(newlevel,midElement[index].right_Num);
                    fprintf(f,"lw $t0 data+%d\n",4*address);
                    if(element[address].type == 1 || element[address].type == 3 || element[address].type == 5){
                        fprintf(f,"li $v0 11\n");
                        fprintf(f,"move $a0 $t0\n");
                        fprintf(f,"syscall\n");
                    }
                    else{
                        fprintf(f,"li $v0 1\n");
                        fprintf(f,"move $a0 $t0\n");
                        fprintf(f,"syscall\n");
                    }
                    break;
                }
            }
        }
        else if(isLabel(midElement[index].op)){
            fprintf(f,"%s:\n",midElement[index].op);
        }
        else{
        }
        index++;
    }
}

