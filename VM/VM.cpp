#include <stdio.h>
#include <string.h>

#include "stack.h"

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
        else if (!strcmp(command_name, "SUBTRACT"))
        {
            stack_pop(&stk, &operand_1);
            stack_pop(&stk, &operand_2);
            stack_push(&stk, operand_2 - operand_1);
        }
        else if (!strcmp(command_name, "OUT"))
        {
            stack_pop(&stk, &operand_1);
            printf("%d\n", operand_1);
        }
        else if (!strcmp(command_name, "HLT"))
            return 0;
    }

    return 0;
}
