#define __USE_MINGW_ANSI_STDIO 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>

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

element* operator_create(char C)//constructor for operator node
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

element* operand_create(float F)//constructor for operand node
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
    
    return EXIT_SUCCESS;
}