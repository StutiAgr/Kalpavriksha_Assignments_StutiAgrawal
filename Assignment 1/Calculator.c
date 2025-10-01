#include<stdio.h>
#include<string.h>

#define MAX 100
int error=0;

int evaluate(char *input){
    int stack[MAX];
    int top=-1;
    char prevOp='+';
    int i=0;
    int length=strlen(input);
    //to check if the expr ends with an operator(invalid)
    //we'll make a bool to check whether each operator has been processed/pending
    //this variable is also for checking multiple operators in middle of 2 nums
    int opPending=0;
    //flag to track if next number should be negative
    int makeNegative=0;
    //store each number in num:
    int num=0;
    while(i<length){
        char c=input[i];

        //if space, then ignore
        if(c==' ' || c=='\n' || c=='\0' || c=='\t'){
            i++;
            continue;
        }

        //if digit
        else if(c>='0' && c<='9'){
            num=0;
            //handle multi-digit numbers
            while(i<length && input[i]>='0' && input[i]<='9'){
                num=(num*10+(input[i]-'0'));
                i++;
            }
            //if this number should be negative, multiply by -
            if(makeNegative){
                num=-num;
                makeNegative=0; //reset flag
            }
            
            //calculate division and multiplication first, store addition and subtraction for later
            switch(prevOp){
                case '/':
                    //handle division by 0
                    if(num==0){
                        printf("Error: Division by zero.");
                        error=1;
                        return 0;
                    }
                    stack[top]=stack[top]/num;
                    break;
                case '*':
                    stack[top]*=num;
                    break;
                case '+':
                    top++;
                    stack[top]=num;
                    break;
                case '-':
                    //put negative of that number for subtraction so that we can just add later
                    top++;
                    stack[top]=-num;
                    break;
            }
            opPending=0; //operator is processed
            continue;
        }
        
        else if(c=='+' || c=='-' || c=='*' || c=='/'){
            //if the expression is starting with an operator, then invalid expr.
            //except '-' for negative numbers
            if(top==-1 && c!='-'){
                printf("Error: Invalid expression.");
                error=1;
                return 0;
            }
            //if previous op is not processed, meaning multiple operators between 2 nums, ->invalid
            //except - for negative numbers
            if(opPending && c!='-'){
                printf("Error: Invalid expression.");
                error=1;
                return 0;
            }
            //if minus appears at start or after another operator, it's for negative number
            if(c=='-' && (top==-1 || opPending)){
                //if makeNegative is already set, that means there are 2 '-' together, making it invalid
                if(makeNegative){
                    printf("Error: Invalid expression.");
                    error=1;
                    return 0;
                }
                makeNegative = 1;
                i++;
                continue; //don't set prevOp or opPending for this minus
            }

            prevOp=c;
            i++;
            opPending=1; //operator processing pending
        }
        else{
            printf("Error: Invalid expression.");
            error=1;
            return 0;
        }
    }

    //ending with operator -> invalid
    if(opPending){
        printf("Error: Invalid expression.");
        error=1;
        return 0;
    }

    //if top==-1, it means string was empty or only had whitespaces,->invalid
    if(top==-1){
        printf("Error: Empty expression");
        error=1;
        return 0;
    }

    //div and mul have been calculated
    //add the numbers in stack (- already put for subtraction)
    int result=0;
    for(int j=0;j<=top;j++){
        result+=stack[j];
    }
    return result;
}

int main(){
    char input[MAX];
    fgets(input,MAX,stdin);
    int result=evaluate(input);
    if(!error) printf("%d",result);
    return 0;
}


/*
    things handled:
    -division by zero
    -invalid character
    -starting with operator
    -ending with operator
    -multiple operators in between 2 numbers
    -expressions that are empty or only contain whitespaces
    -negative numbers, both at start and after operators
*/