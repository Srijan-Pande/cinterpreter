#define __USE_MINGW_ANSI_STDIO 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>
//SRIJAN PANDE 28/01/2026 - 29/01/2026

#define MAXIMUM_USER_INPUT_SIZE 1024 //Dynamically Sized User Input must be implemented in the future

//Function to take user input into the buffer
int UserInputPutInBuffer(char *BUFFER, size_t SIZE)
{
    printf(">>> ");
    if(fgets(BUFFER,SIZE,stdin)==nullptr)//Take user input into the buffer
        {
            if(feof(stdin))
            {
                //Need to add Code to log failure
                return EXIT_FAILURE;
            }
            if(ferror(stdin))
            {
                //Need to add Code to log failure
                return EXIT_FAILURE;
            }
        }
        
    char* NEWLINE_POINT = strchr(BUFFER,'\n');
    if(NEWLINE_POINT)
    {
        //User Input was not truncated and has been fully captured by the buffer. Replace \n with \0.
        *NEWLINE_POINT = '\0';
    }
    else 
    {
        if(strlen(BUFFER)==SIZE-1)
        {
            //User Input was truncated and has not been caught by the buffer. Dump the remaining contents and ask the user how they wish to proceed.
            int c;
            while((c=getchar())!='\n' && (c != EOF)){} //Dumps the remaining contents
            //Code to ask user how they wish to proceed
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

//Add % to ACCEPTED_SYMBOLS_MATHEMATICS later
const char* ACCEPTED_SYMBOLS_MATHEMATICS = "1234567890()^/*+- ";//if the user wants to do math, their input should be comprised of these characters
bool MathematicalExpressionCharacterValidity(const char *BUFFER)//Checking if only mathemtical symbols specified in ACCEPTED_SYMBOLS_MATHEMATICS are present in the user input
//Implement a Lookup Table later on
{
    if(BUFFER==nullptr)return false;

    size_t i = 0;
    while(BUFFER[i]!='\0')
    {
        if(strchr(ACCEPTED_SYMBOLS_MATHEMATICS,BUFFER[i])==nullptr)
        {
            return false;
        }
        i++;
    }
    return true;
}

int Evaluator(const char *BUFFER)
{
    if(BUFFER==nullptr)return '\0';
}

bool BracketsBalanced(const char *BUFFER) //Checking if all of the brackets are balanced in the user input
{
    if(BUFFER==nullptr)return false;

    size_t i = 0;
    int balance = 0;
    while(BUFFER[i]!='\0')
    {
        if(BUFFER[i]=='(')
        {
            balance++;
        }
        else if(BUFFER[i]==')')
        {
            balance--;
            if(balance<0)
            {
                return false;
            }
        }
        i++;
    }
    return balance==0; 
    //Replace the above code with a stack to account for (),[],{} all at once
}

typedef enum 
{
    operator_type=0,
    operand_type=1
} element_type;

typedef struct element
{
    element_type type;
    union{
        char operator_element;
        float operand_element;
    };
    struct element* ahead;
    struct element* behind;
}element;

element* operator_create(char C)
{
    element *node = (element *)malloc(sizeof(element));
    if(node!=nullptr)
    {
        node->type=operator_type;
        node->operator_element=C;
        node->ahead=nullptr;
        node->behind=nullptr;
    }
    return node;
}

element* operand_create(float F)
{
    element *node = (element *)malloc(sizeof(element));
    if(node!=nullptr)
    {
        node->type=operand_type;
        node->operand_element=F;
        node->ahead=nullptr;
        node->behind=nullptr;
    }
    return node;
}

int main(int argc, char *argv[])
{
    char BUFFER[MAXIMUM_USER_INPUT_SIZE];//buffer to store user input 
    bool EXECUTE = true;//boolean flag variable which makes the program execute

    while(EXECUTE)
    {
        if(UserInputPutInBuffer(BUFFER,sizeof(BUFFER))!=EXIT_SUCCESS)//Code for taking User Input
        {
            //Code for dealing with errors while taking user input in the buffer
            EXECUTE = false;
            return EXIT_FAILURE;
        }
        //TESTING_COMMENTSprintf("%s\n",BUFFER);

        if(stricmp(BUFFER,"exit")==0)//Code for checking if the user wants to exit the program
        {
            EXECUTE=false;
            break;
        }//Improve exit options (EXIT should also exit the program)

        if(stricmp(BUFFER,"clear")==0)//Code for checking if the user wants to clear their screen
        {
            system("clear");
            continue;
        }
        
        if(MathematicalExpressionCharacterValidity(BUFFER)==false||BracketsBalanced(BUFFER)==false)//Potential Improvement: Both conditions can be combined to be tested by one function in order to iterate only once through the BUFFER containing user input
        {
            puts("Invalid Input");//Improvement to be made: Write code to highlight where and why the Input is Invalid
        }
    }
    return EXIT_SUCCESS;
}