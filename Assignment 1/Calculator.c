#include<ctype.h>
#include<limits.h>
#include<stdio.h>
#include<string.h>

#define MAX 100

int error=0;

int isOperator(char c);
int parseNumber(char *input, int *index, int length, int isNegative);
int applyOperation(int *stack, int *top, char op, int num);
void pushToStack(int *stack, int *top, int value);
int calculateResult(int *stack, int top);
int validateAndEvaluate(char *input);
void displayMenu();

int main(){
    char input[1000];
    int choice;
    printf("Welcome to Calculator!\n");
    do{
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d",&choice);
        getchar(); //consume newline

        if(choice==1){
            printf("Enter expression: ");
            fgets(input,1000,stdin);
            error=0;
            int result=validateAndEvaluate(input);

            if(!error){
                printf("Result: %d\n",result);
            }
        }
        else if(choice==2){
            printf("Exiting...");
        }
        else{
            printf("Invalid choice. Please enter 1 or 2.\n");
        }
    }while(choice!=2);
    return 0;
}

void displayMenu(){
    printf("\n----Calculator Menu----\n");
    printf("1. Evaluate Expression\n");
    printf("2. Exit\n");
}

int isOperator(char c){
    return (c=='+' || c=='-' || c=='*' || c=='/');
}

int parseNumber(char *input, int *index, int length, int isNegative){
    long long num=0;
    while(*index<length && input[*index]>='0' && input[*index]<='9'){
        num=num*10+(input[*index]-'0');
        if(!isNegative && num>INT_MAX){
            printf("Error: Number too large (overflow).");
            error=1;
            return 0;
        }
        if(isNegative && num>(long long)INT_MAX+1){
            printf("Error: Number too small (underflow).");
            error=1;
            return 0;
        }
        (*index)++;
    }
    if(isNegative){
        num=-num;
        if(num<INT_MIN){
            printf("Error: Number too small (underflow).");
            error=1;
            return 0;
        }
    }
    return (int)num;
}

void pushToStack(int *stack, int *top, int value){
    if(*top>=MAX-1){
        printf("Error: Stack overflow.");
        error=1;
        return;
    }
    (*top)++;
    stack[*top]=value;
}

int applyOperation(int *stack, int *top, char op, int num) {
    if(op=='/') {
        if(num==0){
            printf("Error: Division by zero.");
            error=1;
            return 0;
        }
        stack[*top]/=num;
    }
    else if(op=='*'){
        long long result=(long long)stack[*top]*num;
        if(result>INT_MAX || result<INT_MIN){
            printf("Error: Multiplication overflow.");
            error=1;
            return 0;
        }
        stack[*top]=(int)result;
    }
    else if(op=='+') {
        pushToStack(stack, top, num);
    }
    else if(op=='-') {
        pushToStack(stack, top, -num);
    }
    return 1;
}

int calculateResult(int *stack, int top){
    long long result=0;
    for(int j=0;j<=top;j++) {
        result+=stack[j];
        if(result>INT_MAX){
            printf("Error: Result overflow");
            error=1;
            return 0;
        }
        if(result<INT_MIN){
            printf("Error: Result underflow");
            error=1;
            return 0;
        }
    }
    return (int)result;
}

int validateAndEvaluate(char *input){
    int stack[MAX];
    int top=-1;
    char prevOp='+';
    int i=0;
    int length=strlen(input);
    int opPending=0;
    int makeNegative=0;
    
    while(i<length){
        char c=input[i];
        if(isspace(c)){
            i++;
            continue;
        }

        if(isdigit(c)){
            int num=parseNumber(input, &i, length, makeNegative);
            makeNegative=0;
            if(!applyOperation(stack, &top, prevOp, num)){
                return 0;
            }
            if(error) return 0;
            opPending=0;
            continue;
        }
        
        if(isOperator(c)){
            if(top==-1 && c!='-'){
                printf("Error: Expression cannot start with operator.");
                error=1;
                return 0;
            }
            if(opPending && c!='-'){
                printf("Error: Multiple operators between numbers.");
                error=1;
                return 0;
            }
            if(c=='-' && (top==-1 || opPending)){
                if(makeNegative) {
                    printf("Error: Double negative sign.");
                    error=1;
                    return 0;
                }
                makeNegative=1;
                i++;
                continue;
            }
            prevOp=c;
            i++;
            opPending=1;
        }
        else{
            printf("Error: Invalid character '%c'.", c);
            error=1;
            return 0;
        }
    }

    if(opPending){
        printf("Error: Expression cannot end with operator.");
        error=1;
        return 0;
    }

    if(top==-1){
        printf("Error: Empty expression.");
        error=1;
        return 0;
    }
    return calculateResult(stack, top);
}