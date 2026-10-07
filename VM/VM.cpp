#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stack.h"

#define ACCURACY 100000
int main(void)
{
    struct stack_t stk = {};
    char command_name[10] = {};
    int argument_value = 0;
    stack_elem_t operand_1 = 0;
    stack_elem_t operand_2 = 0;

    stack_init(&stk, 10);

    while (scanf("%s", command_name) != EOF)
    {
        if (!strcmp(command_name, "PUSH"))
        {
            if (!scanf("%d", &argument_value))
            {
                printf("Wrong number of arguments in PUSH");
                return 0;
            }
            stack_push(&stk, argument_value);
        }
        else if (!strcmp(command_name, "ADD"))
        {
            stack_pop(&stk, &operand_1);
            stack_pop(&stk, &operand_2);
            stack_push(&stk, operand_1 + operand_2);
        }
        else if (!strcmp(command_name, "SUBT"))
        {
            stack_pop(&stk, &operand_1);
            stack_pop(&stk, &operand_2);
            stack_push(&stk, operand_2 - operand_1);
        }
        else if (!strcmp(command_name, "MULT"))
        {
            stack_pop(&stk, &operand_1);
            stack_pop(&stk, &operand_2);
            stack_push(&stk, operand_2 * operand_1);
        }
        else if (!strcmp(command_name, "DIV"))
        {
            stack_pop(&stk, &operand_1);
            stack_pop(&stk, &operand_2);
            stack_push(&stk, (int)(((double)operand_2 / (double)operand_1) * ACCURACY));
        }
        else if (!strcmp(command_name, "SQRT"))
        {
            stack_pop(&stk, &operand_1);
            if (operand_1 < 0)
            {
                printf("Negative radicant!");
                return 0;
            }
            stack_push(&stk, (double)(sqrt(operand_1) * ACCURACY));
        }
        else if (!strcmp(command_name, "SIN"))
        {
            stack_pop(&stk, &operand_1);
            stack_push(&stk, (double)(sin(operand_1) * ACCURACY));
        }
        else if (!strcmp(command_name, "COS"))
        {
            stack_pop(&stk, &operand_1);
            stack_push(&stk, (double)(cos(operand_1) * ACCURACY));
        }
        else if (!strcmp(command_name, "TAN"))
        {
            stack_pop(&stk, &operand_1);
            stack_push(&stk, (double)(tan(operand_1) * ACCURACY));
        }
        else if (!strcmp(command_name, "ACOS"))
        {
            stack_pop(&stk, &operand_1);
            if (fabs(operand_1) > 1)
            {
                printf("Wrong value of cos");
                return 0;
            }
            stack_push(&stk, (double)(acos(operand_1) * ACCURACY));
        }
        else if (!strcmp(command_name, "ASIN"))
        {
            stack_pop(&stk, &operand_1);
            if (fabs(operand_1) > 1)
            {
                printf("Wrong value of sin");
                return 0;
            }
            stack_push(&stk, (double)(asin(operand_1) * ACCURACY));
        }
        else if (!strcmp(command_name, "ATAN"))
        {
            stack_pop(&stk, &operand_1);
            stack_push(&stk, (double)(atan(operand_1) * ACCURACY));
        }
        else if (!strcmp(command_name, "OUT"))
        {
            stack_pop(&stk, &operand_1);
            printf("%lg\n", ((double)operand_1 / ACCURACY));
        }
        else if (!strcmp(command_name, "HLT"))
            return 0;
    }

    return 0;
}
