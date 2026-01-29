#define __USE_MINGW_ANSI_STDIO 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>
#define MAXIMUM_USER_INPUT_SIZE 1024

bool BracketsBalanced(const char *BUFFER)
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
}

int main(int argc, char *argv[])
{
    char* test_case[] = {
        "(", //0
        ")",
        "()",
        "(())",
        ")(",
        "(((())))", //5
        "((())))",
        "( )",
        "   ()",
        "()   ",
        "(   )", //10
        ")   ",
        "(   ",
        "   )",
        "   (",
        "( ())", //15
        "abcd1234",
        "(1+3)",
        "(4+ ",
        "-4 )"
    };

    bool expected_output[] = {
        false, //0
        false,
        true,
        true,
        false,
        true, //5
        false,
        true,
        true,
        true,
        true, //10
        false,
        false,
        false,
        false,
        true, //15
        true,
        true,
        false,
        false
    };

    size_t test_size;
    for(size_t i = 0; i<sizeof(expected_output); i++)
    {
        bool output = BracketsBalanced(test_case[i]);
        
        printf("TEST CASE %zu\ntest string: %s\ntest size: %zu\nBrackets Balanced = %s\nExpected Value = %s\nTest Case %s\n\n",
            i+1, //TEST CASE
            test_case[i], //test string:
            test_size, //test size:
            output==true?"true":"false", //Brackets Balanced = true/false
            expected_output[i]==true?"true":"false", //Expected Value = true/false
            output==expected_output[i]?"Passed":"Failed" //Test Case Passed/Failed
        );
    }
    return EXIT_SUCCESS;
}