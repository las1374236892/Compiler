#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Compiler.h"

char fileName[100];//文件路径
char line[100000];

int main(){
    FILE *fp;
    FILE *f;
    int i = 0;
	memset(line, 0, 100000);
	scanf("%s",fileName);
	fp = fopen(fileName, "r");
	f = fopen("../result.txt","w");
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
                symbolLine[symbolArrayLength] = fileline;
                symbolArrayLength++;
            }
        }//将所有的标识符存入数组中
		memset(line, 0, 100000);
	}//每次读入一行
	/*for(i=0;i<count;i++){
        printf("%s\n",symbolArray[i]);
	}*/
    program();
    fprintf(f,".data\n");
    fprintf(f,"\tdata: .space 0x10000\n");
    for(i=0;i<stringCount;i++){
        fprintf(f,"\tstring%d: .asciiz %s\n",i,String[i]);
    }
    fprintf(f,".text\n");
    fprintf(f,"li $s6,4\n");
    getMips(f,"data");

    for(i=0;i<symbolTabAdress;i++){
        printf("%s %d %d %d %d %d\n",element[i].name,element[i].type,element[i].value,element[i].address,element[i].para,element[i].level);
    }
    /*for(i=0;i<midCodeIndex;i++){
        printf("%s %s %s %s\n",midElement[i].op,midElement[i].left_Num,midElement[i].right_Num,midElement[i].result);
    }*/
	//system("pause");
	return 0;
}
