#include<stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
#define size_s 20
typedef struct Stack{
	char *arr;
	int top;
}Stack;
typedef struct StackInt{
	int *arr;
	int top;
}StackInt;
typedef struct val{
	char name;
	int value;
}val;
val *values;
int ValIndex=0;
Stack* createStack();
bool isFull(Stack*);
bool isEmpty(Stack*);
void pushI(Stack*,int);
void pushC(Stack*,char);
void infix2postfix(char*);
void getDataFromFile(char*);
int findVal(char);
int popint(StackInt*);
void pushint(int,StackInt*);
int priority(char);

int main (){
	Stack* stack=createStack();
	FILE *f;
	values=(val*)calloc(sizeof(val),size_s);
	getDataFromFile("input.txt");
	return 0;
}

void getDataFromFile(char *file){
	FILE *f=fopen(file,"r");
	char buffer[255];
	while(fgets(buffer,255,f)!=NULL)
	{
		printf("*******INPUT LINE BEGIN***********\nInfixform: %s",buffer);
		printf("\nPOSTFIX FORM: %.*s",4,buffer);
		infix2postfix(buffer);
		memset(buffer, 0, sizeof(buffer));
	}
	fclose(f);
}

Stack* createStack(){
	Stack *s=(Stack*)malloc(sizeof(Stack));
	s->arr=(char*)calloc(sizeof(char),size_s);
	s->top=-1;
	return s;
}

bool isFull(Stack* s){
	return s->top==size_s-1 ? true : false;
}

bool isEmpty(Stack* s){
	return s->top==-1 ? true : false;
}

void pushC(Stack *s,char val){ // push char to  Stack
	s->arr[++s->top]=val;
}
int priority(char c) //define priority of symbols
{
    if(c == '(')
        return 0;
    if(c == '+' || c == '-')
        return 1;
    if(c == '*' || c == '/')
        return 2;
}
char popC(Stack* s){ //pop char from Stack
	if(!isEmpty(s)){
		return s->arr[s->top--];
	}
	return -1;
}

void infix2postfix(char *buffer){
	char x;
	val *values=(val*)calloc(sizeof(val),size_s);
	int valHead=0;
	char a;
	char postfixForm[50]="";
	Stack *s=createStack();
    char *e;
	e=strtok(buffer,"=");
	a=e[0];
	e=strtok(NULL," ");
    while(e[0] != ';') // loop till end of line
    {
    	if(isdigit(e[0])){ //if input is digit print it
    		sprintf(postfixForm,"%s %s",postfixForm,e);
		}
        else if(isalnum(e[0])){ //if input comes from alphebet print it
        	sprintf(postfixForm,"%s %s",postfixForm,e);
        	
		}
            
        else if(e[0] == '(') // if ( push it
            pushC(s,e[0]);
        else if(e[0] == ')') // if ) pop till == (
        {
            while((x = popC(s)) != '(')
                sprintf(postfixForm,"%s %c",postfixForm,x);
        }
        else
        {
        	if(isEmpty(s)){ //if it symbol and stack is empty push it else check priority
        		pushC(s,e[0]);
			}
			else{
				while(priority(s->arr[s->top]) >= priority(e[0]) && s->top>-1)
                sprintf(postfixForm,"%s %c",postfixForm,popC(s));
                pushC(s,e[0]);
			}
            
        }
      	e=strtok(NULL," "); // pass the next value
    }
    while(s->top != -1) // pop all remaining data from stack
    {
    	sprintf(postfixForm,"%s %c",postfixForm,popC(s));
    }
   	printf("%s\n",postfixForm);
	solvePostfixForm(postfixForm,a); // solve postfix

}
void solvePostfixForm(char *p,char name){
	int index=0;
	StackInt *stInt;
		stInt=(StackInt*)calloc(sizeof(StackInt),size_s);
	stInt->arr=(int*)calloc(sizeof(int),size_s);
	stInt->top=-1;
	char* token=strtok(p," ");
	while(token!=NULL){
		int y;
		
		if(isdigit(token[0])){ //if its digit push to stackInt
			pushint(atoi(token),stInt);
		}
		else{
			if(isalnum(token[0])){ //if its var find value and push it
				pushint(values[findVal(token[0])].value,stInt);
			}
			else{ // if its symbol pop 2 data and push result
				if(token[0]=='*'){ 
					int result=popint(stInt);
					result*=popint(stInt);
					pushint(result,stInt);
				}
				else if(token[0]=='/'){
					int tmp=popint(stInt);
					pushint(popint(stInt)/tmp,stInt);
				}
				else if(token[0]=='-'){
					int tmp=popint(stInt);
					pushint(popint(stInt)-tmp,stInt);
				}
				else if(token[0]=='+'){
					pushint((popint(stInt)+popint(stInt)),stInt);
				}
			}
		}
		printStackInt(stInt); // print stack status every step
		puts("\n");
		token=strtok(NULL," ");
		
	}
	val v;
	v.name=name;
	v.value=popint(stInt);
	int f=findVal(v.name);
	if(f!=-1){ //if value already exist change it else add new
		values[f]=v;
	}
	else
		values[ValIndex++]=v;
	printf("POSTFIX RESULT:\n");
	printf("%c= %d\n",name,v.value);
	puts("*******INPUT LINE END*************\n");
}
int findVal(char c){
	int i=0;
	while(values[i].name!=c && i<size_s){
	i++;	
	}
	if(i>=size_s)
		return -1;
	return i;
}
void pushint(int val,StackInt *stInt){
	stInt->arr[++stInt->top]=val;
}
int popint(StackInt *stInt){
	if(stInt->top!=-1)
		return stInt->arr[stInt->top--];
	return -1;
}
void printStackInt(StackInt *s){
	int y;
		puts("Stack: ");
		for(y=0;y<=s->top;y++){
			printf("%d | ",s->arr[y]);
		}
}
