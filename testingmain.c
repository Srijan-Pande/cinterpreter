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
    if(fgets(BUFFER,SIZE,stdin)==NULL)//Take user input into the buffer
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
    if(BUFFER==NULL)return false;

    size_t i = 0;
    //for(size_t i = 0; i<SIZE; i++)
    while(BUFFER[i]!='\0')
    {
        if(strchr(ACCEPTED_SYMBOLS_MATHEMATICS,BUFFER[i])==NULL)
        {
            return false;
        }
        i++;
    }
    return true;
}

bool BracketsBalanced(const char *BUFFER) //Checking if all of the brackets are balanced in the user input
{
    if(BUFFER==NULL)return false;

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


//Linked List used to represent user input
typedef enum //enumerator to indicate whether the element in the linked list is an operator or an operand
{
    operator_type=0,
    operand_type=1
} element_type;

typedef struct element //node of Linked List
{
    element_type type;
    union{
        char operator_element;
        float operand_element;
    };
    struct element* ahead;
    struct element* behind;
}element;

element *operator_create(char C)//constructor for operator node
{
    element *node = (element *)malloc(sizeof(element));
    if(node==nullptr)return nullptr;
    *node = (element)
    {
        .type=operator_type,
        .operator_element=C,
        .ahead=nullptr,
        .behind=nullptr
    };
    return node;
}

element *operand_create(float F)//constructor for operand node
{
    element *node = (element *)malloc(sizeof(element));
    if(node==nullptr)return nullptr;
    *node = (element)
    {
        .type=operand_type,
        .operand_element=F,
        .ahead=nullptr,
        .behind=nullptr
    };
    return node;
}

//The below function must be used very carefully as it may leave its neighbours pointing to garbage values
void free_node(element *node)//simple destructor for node of linked list
{
    if(node!=nullptr)
    {
        free(node);
    }
    return;
}

void free_element(element *node)
{
    if(node!=nullptr)
    {
        if(node->behind!=nullptr)
        {
            node->behind->ahead = node->ahead;
        }
        if(node->ahead!=nullptr)
        {
            node->ahead->behind = node->behind;
        }
        free_node(node);
    }
    return;
}

element *free_element_with_return(element *node)
{
    if(node==nullptr)return nullptr;
    element *return_element = node->ahead;
    if(node->behind!=nullptr)
        {
            node->behind->ahead = node->ahead;
        }
    if(node->ahead!=nullptr)
        {
            node->ahead->behind = node->behind;
        }
    free_node(node);
    return return_element;
}

//REMEMBER to set head = nullptr; after using this below given function
void free_user_input(element* head)//deletes the entire linked list comprising the user input. 
{
    if(head==nullptr)return;
    element *current = head;
    element *next_element;
    while(current!=nullptr)
    {
        next_element=current->ahead;
        free_node(current);
        current=next_element;
    }
    return;
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