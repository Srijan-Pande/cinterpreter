#define __USE_MINGW_ANSI_STDIO 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>
#include<ctype.h>
#include<math.h>
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
const char* ACCEPTED_SYMBOLS_MATHEMATICS = "1234567890()^/*%+-. ";//if the user wants to do math, their input should be comprised of these characters
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

//Floating Point Arithmetic yields poor results for certain test cases
//_DECIMAL or some other solution is required to be implemented
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

//Tokenizer for converting user input in BUFFER into a linked list for evaluation
element* tokenizer(const char *BUFFER)
{
    if(BUFFER==nullptr)return nullptr;

    //Create pointers for head and tail 
    element *head = nullptr;
    element *tail = nullptr;
    size_t i = 0;

    while(BUFFER[i]!='\0')
    {
        if(isspace(BUFFER[i]))//Ignore whitespace
        {
            i++;
            continue;
        }
        element *new_element = nullptr;
        if(isdigit(BUFFER[i]) || BUFFER[i]=='.')//Check for numbers or . decimal points. If encountered, convert the string into floating point numbers and use the endptr to calculate the next point to continue iterating from
        {
            char *endptr;
            float value = strtof(&BUFFER[i],&endptr);
            new_element = operand_create(value);
            i+=(endptr-&BUFFER[i]);
        }
        else//If it is not a whitespace or a number, it must be an operator as we put in this restriction in the function MathematicalExpressionCharacterValidity
        {
            new_element = operator_create(BUFFER[i]);
            i++;
        }
        if (new_element != nullptr) //update head and tail appropriately assuming the node was created i.e the dynamic allocation of memory to it did not fail.
        {
            if (head == nullptr) 
            {
                head = new_element;
                tail = new_element;
            } 
            else 
            {
                tail->ahead = new_element;
                new_element->behind = tail;
                tail = new_element;
            }
        }
    }
    return head;
}

//Print the linked list. Just for testing.
void print_tokenizer_output(element *head) 
{
    if(head==nullptr) return;

    element *current = head;
    while (current != nullptr) {
        if (current->type == operand_type) {
            printf("%f", current->operand_element);
        } else {
            printf("%c", current->operator_element);
        }
        if (current->ahead != nullptr) printf(" ");
        current = current->ahead;
    }
    printf("\n");
}

//Helper function to safely perform the mathematical operations
float compute(float X, float Y, char O)
{
    switch(O)
    {
        case '+':
        return X+Y;

        case '-':
        return X-Y;

        case '*':
        return X*Y;

        case '/':
        return Y!=0.0f?X/Y:0.0f;

        case '%':
        return fmodf(X,Y);

        case '^':
        return powf(X,Y);

        default:return 0;
    }
}

// Function to perform mathematics in a list segment and reduce it appropriately
void reduce_segment(element **head_ref) {
    if (*head_ref == nullptr) return;

    //Manage Unary Operators namely + and -
    element *curr = *head_ref;
    while (curr != nullptr) {
        if (curr->type == operator_type && (curr->operator_element == '-' || curr->operator_element == '+')) {
            //For simplicity, only if a + or - is encountered after another operator or encountered before an operand, will it be considered for transforming into a unary operator
            if (curr->behind == nullptr || curr->behind->type == operator_type) {
                if (curr->ahead && curr->ahead->type == operand_type) {
                    if (curr->operator_element == '-') {
                        curr->ahead->operand_element *= -1;
                    }
                    element *to_free = curr;
                    curr = curr->ahead;
                    if (to_free == *head_ref) *head_ref = curr;
                    free_element(to_free);
                    continue;
                }
            }
        }
        curr = curr->ahead;
    }

    //Reduce/Resolve based on PEMDAS
    char *precedence[] = {"^", "*/%", "+-"};
    for (int i = 0; i < 3; i++) {
        curr = *head_ref;
        while (curr != nullptr) {
            if (curr->type == operator_type && strchr(precedence[i], curr->operator_element)) {
                element *left = curr->behind;
                element *right = curr->ahead;

                if (left && right && left->type == operand_type && right->type == operand_type) {
                    float res = compute(left->operand_element, right->operand_element, curr->operator_element);
                    left->operand_element = res;
                    
                    // Remove operator and right operand
                    curr = free_element_with_return(curr); // removes op
                    free_element(curr); // removes right operand
                    curr = left; // Continue from result node
                }
            }
            curr = curr->ahead;
        }
    }
}

element* find_matching_parentheses(element *start)//Find innermost parentheses
{
    int count = 0;
    element *current = start;
    while (current != nullptr) 
    {
        if (current->type == operator_type && current->operator_element == '(') count++;

        if (current->type == operator_type && current->operator_element == ')') 
        {
            count--;
            if (count == 0) return current;
        }
        current = current->ahead;
    }
    return nullptr;
}

void resolve_parentheses(element **head_reference)//Resolve the parentheses
{
    element *current = *head_reference;
    while (current != nullptr) 
    {
        if (current->type == operator_type && current->operator_element == '(') 
        {
            element *open_p = current;
            element *close_p = find_matching_parentheses(open_p);

            if (close_p) 
            {
                //Isolate the sub-list
                element *sub_list = open_p->ahead;
                sub_list->behind = nullptr;
                close_p->behind->ahead = nullptr;

                //Recursively solve the sub-list
                resolve_parentheses(&sub_list);
                reduce_segment(&sub_list);

                //Replace with the result
                element *result_node = sub_list;
                
                //Link result back into the main list
                result_node->behind = open_p->behind;
                if (open_p->behind) open_p->behind->ahead = result_node;
                else *head_reference = result_node;

                result_node->ahead = close_p->ahead;
                if (close_p->ahead) close_p->ahead->behind = result_node;

                //Free the '(' and ')' element nodes
                free(open_p);
                free(close_p);
                
                //Scan again as needed
                current = result_node;
                continue;
            }
        }
        current = current->ahead;
    }
}

float evaluate(element **head)//Evaluator Function taking input of address of head of user-input linked list
{
    if (*head == nullptr) return 0.0f;
    resolve_parentheses(head);
    reduce_segment(head);
    return ((*head)->type == operand_type) ? (*head)->operand_element : 0.0f;
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
        else
        {
            element *activity = tokenizer(BUFFER);
            //print_tokenizer_output(activity);
            float output = evaluate(&activity);
            printf("%f\n",output);
        }
    }
    return EXIT_SUCCESS;
}