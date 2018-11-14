#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char file[100000];//读取的文件数组
int filelen = 0;//最大长度
int count = 0;//词法分析计数器
char stayWord[13][15] = { "const","int","char","void","main","while","if","switch","case","default","scanf","printf","return" };

int isStayWord(char *str);//判断是否为关键字，并且返回其在数组中的位置
int isCalculate(char ch);//判断是否为运算符
int isSymbol(char ch);//判断是否为其他的合法字符
int isQuotation(char h);//判断是否为引号
int inSymbol(int current, char *array);//读取下一个单词
void Analysis();//词法分析程序

int isStayWord(char *str) {
	int i = 0;
	for (i = 0; i < 13; i++) {
		if (!strcmp(str, stayWord[i])) {
			break;
		}
	}
	return i;
}

int isCalculate(char ch) {
	if (ch == '+' || ch == '-')
		return 1;
	else if (ch == '*' || ch == '/')
		return 2;
	else if (ch == '>' || ch == '<' || ch == '=')
		return 3;
	else if (ch == '!')
		return 4;
	else
		return 0;
}

int isSymbol(char ch) {
	if(ch == ':')
		return 6;
	else if (ch == '{' || ch == '}')
		return 5;
	else if (ch == '[' || ch == ']')
		return 4;
	else if (ch == '(' || ch == ')')
		return 3;
	else if (ch == ';')
		return 2;
	else if (ch == ',')
		return 1;
	else
		return 0;
}

int isQuotation(char ch) {
	if (ch == '\"')
		return 2;
	else if (ch == '\'')
		return 1;
	else
		return 0;
}

int inSymbol(int current, char *array) {
	int length = 0;
	if (isQuotation(file[current]) == 2) {
		array[length++] = '\"';
		current++;
		while (file[current] != '\"' && current < filelen) {
			array[length++] = file[current++];
		}
		current++;
	}
	else if(isQuotation(file[current]) == 1){
		array[length++] = '\'';
		current++;
		while (file[current] != '\'' && current < filelen) {
			array[length++] = file[current++];
		}
		current++;
	}
	else {
		while (file[current] != ' ' && current < filelen) {
			if(file[current] == '\'' || file[current] == '\"')
				break;
			array[length++] = file[current++];
		}
	}
	return current;
}

/*int identType(char *str){
	int i=0,j=0;
	int length = strlen(str);
	for(i=0;i<length;i++){
		if(str[i] == '['){
			for(j=i+1;j<length-1;j++){
				if(str[i+1] <= '0' || str[length-1] != ']' || !isdigit(str[j])){
					return 0;
				}
			}
		}
	}
}*/

void Analysis() {
	char Str[1000];
	char copyStr[1000];
	int i, k, x;
	int length;
	i = 0;
	while (i < filelen) {
		length = 0;
		memset(Str, 0, 1000);
		while (file[i] == ' ' && i < filelen) {
			i++;
		}
		i = inSymbol(i, Str);
		length = strlen(Str);
		k = 0;
		while (k < length) {
			x = 0;
			if (isupper(Str[k]) || islower(Str[k])) {
				while (!isSymbol(Str[k]) && !isCalculate(Str[k]) && k < length) {
					copyStr[x++] = Str[k++];
				}
				count++;
				if (isStayWord(copyStr) >= 13)
					printf("%d IDENT %s\n",count,copyStr);
				else
					printf("%d KEYWORD %s\n",count,copyStr);
			}//发现字母开头
			else if (isdigit(Str[k])) {
				while (isdigit(Str[k]) && k < length) {
					copyStr[x++] = Str[k++];
				}
				count++;
				printf("%d UNSIGNEDINTGER %s\n",count,copyStr);
			}//发现数字开头
			else if (isCalculate(Str[k])) {
				while (isCalculate(Str[k]) && k < length) {
					copyStr[x++] = Str[k++];
				}
				if (x > 1) {
					count++;
					if (copyStr[0] == '!' && copyStr[1] == '=')
						printf("%d UNEQUAL %s\n", count, copyStr);
					else if (copyStr[0] == '=' && copyStr[1] == '=')
						printf("%d EQUAL %s\n", count, copyStr);
					else if (copyStr[0] == '>' && copyStr[1] == '=')
						printf("%d GEQUAL %s\n", count, copyStr);
					else if (copyStr[0] == '<' && copyStr[1] == '=')
						printf("%d SEQUAL %s\n", count, copyStr);
					else
						printf("%d ERROR %s\n", count, copyStr);
				}
				else {
					count++;
					if (isCalculate(copyStr[0]) == 1)
						printf("%d ADDOP %s\n",count,copyStr);
					else if (isCalculate(copyStr[0]) == 2)
						printf("%d MULTOP %s\n",count, copyStr);
					else if(isCalculate(copyStr[0]) == 3)
						switch (copyStr[0])
						{
						case '>':
							printf("%d GREATER %s\n", count, copyStr);
							break;
						case '<':
							printf("%d SMALLER %s\n", count, copyStr);
							break;
						default:
							printf("%d EQUAL %s\n", count, copyStr);
							break;
						}
				}
			}//发现计算符号开头		
			else if (isSymbol(Str[k])) {
				count++;
				switch (isSymbol(Str[k]))
				{
				case 6:
					printf("%d COLON %c\n", count, Str[k]);
					break;
				case 5:
					printf("%d BRACE %c\n", count, Str[k]);
					break;
				case 4:
					printf("%d PAREN %c\n", count, Str[k]);
					break;
				case 3:
					printf("%d paren %c\n", count, Str[k]);
					break;
				case 2:
					printf("%d SEMI %c\n", count, Str[k]);
					break;
				case 1:
					printf("%d COMMA %c\n", count, Str[k]);
					break;
				default:
					break;
				}
				k++;
			}//发现字符
			else if (isQuotation(Str[k])) {
				count++;
				int t;
				int flag = 0;
				if (isQuotation(Str[k]) == 2){
					for(t=k+1;t<length;t++){
						if(Str[t] != 32 && Str[t] != 33 && Str[t] < 35 && Str[t] >126){
							printf("%d ERROR %s\"\n", count, Str);
							flag = 1;
							break;
						}
					}
					if(!flag)
						printf("%d STRING %s\"\n", count, Str);
				}
				else{
					t=k+1;
					if(strlen(Str) != 2){
						printf("%d ERROR %s\'\n", count, Str);
					}
					else if(isCalculate(Str[t]) != 1 && isCalculate(Str[t]) != 2 
					&& !isupper(Str[t]) && !islower(Str[t]) && !isdigit(Str[2])){
						printf("%d ERROR %s\'\n", count, Str);
						flag = 1;
					}
					else
						printf("%d CHAR %s\'\n", count, Str);
				}	
				break;
			}//发现单引号和双引号开头
			else {
				k++;
				count++;
				printf("%d ERROR %s\n", count,copyStr);
			}//除此以外均进行报错
			memset(copyStr, 0, 1000);
		}
	}

}

int main() {
	FILE *fp;
	char ch;
	int i = 0;
	fp = fopen("D:\\compiler\\input.txt", "r");
	while ((ch = fgetc(fp)) != EOF) {
		file[filelen++] = ch;
	}
	for (i = 0; i < filelen; i++) {
		if (file[i] == '\n' || file[i] == '\t')
			file[i] = ' ';
	}//将所有换行符和转义符替换为空格
	Analysis();//启动词法分析程序
	fclose(fp);
	system("pause");
	return 0;
}
