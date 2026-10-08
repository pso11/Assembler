#include <stdio.h>
#include <string.h>
#include <sys\stat.h>
#include <stdlib.h>

#include "stack.h"

#define ACCURACY 100000

enum command_code_t
{
    PUSH = 1,
    ADD  = 2,
    SUBT = 3,
    MULT = 4,
    DIV  = 5,
    OUT  = 6,
    HLT  = 7
};

struct command_t
{
    const char* name;
    short code;
};

char* create_text_buffer(const size_t windows_number_elements, FILE* file);
void compilation(const char* source_file, const char* destination_file);
size_t define_file_size(const char* file);
void counting(struct stack_t stk, const char* byte_code_file);

#ifdef STACK_DEBUG
    struct source_location dump_file = {};
#endif

int main(const int argc, const char* argv[])
{
    struct stack_t stk = {};
    stack_init(&stk, 25);

    const char* code_file = argv[1];
    const char* byte_code_file = argv[2];
    if (argc < 2)
    {
        printf("Not enough arguments");
        return 0;
    }

    compilation(code_file, byte_code_file);
    counting(stk, byte_code_file);

    stack_destroy(&stk);
    return 0;
}

void compilation(const char* source_file, const char* destination_file)
{
    FILE* code_file = fopen(source_file, "r");
    FILE* byte_code_file = fopen(destination_file, "w+");

    char command_name[10] = {};
    int  argument = 0;

    struct command_t array[] =
    {
        {"PUSH", PUSH},
        {"ADD",  ADD},
        {"SUBT", SUBT},
        {"MULT", MULT},
        {"DIV",  DIV},
        {"OUT",  OUT},
        {"HLT",  HLT}
    };

    while (fscanf(code_file, "%s", command_name) != EOF)
    {
        if (!strcmp("PUSH", command_name))
        {
            fscanf(code_file, "%d", &argument);
            fprintf(byte_code_file, "%hd %d\n", array[0].code, argument);
        }
        else
        {
            for (size_t j = 0; j < sizeof(array)/ sizeof(array[0]); j++)
            {
                if (!strcmp(array[j].name, command_name))
                {
                    fprintf(byte_code_file, "%hi\n", array[j].code);
                    break;
                }
            }
        }
    }

//     const size_t windows_number_elements = define_file_size(source_file);
//     char* text_buffer = create_text_buffer(windows_number_elements, code_file);
//     char* byte_code_buffer = (char*)calloc(windows_number_elements, sizeof(char));
//
//     size_t read_byte = 0;
//     char command_name[10] = {};
//
//     while (sscanf(text_buffer + read_byte, "%s%n", command_name, &read_byte) != EOF)
//     {
//         for (size_t j = 0; j < sizeof(array)/ sizeof(array[0]); j++)
//         {
//             if (!strcmp(array[j].name, command_name))
//             {
//                 byte_code_buffer[
//                 break;
//             }
//         }
//     }


    free(text_buffer);
    fclose(code_file);
    fclose(byte_code_file);
}

void counting(struct stack_t stk, const char* byte_code_file)
{
    FILE* file = fopen(byte_code_file, "r");

    short command_code = 0;
    stack_elem_t argument_value = 0;
    stack_elem_t operand_1 = 0;
    stack_elem_t operand_2 = 0;

    while (fscanf(file, "%hi", &command_code) != EOF)
    {
        switch (command_code)
        {
            case PUSH:
                fscanf(file, SPECIFICATOR, &argument_value);
                stack_push(&stk, argument_value);
                break;
            case ADD:
                stack_pop(&stk, &operand_1);
                stack_pop(&stk, &operand_2);
                stack_push(&stk, (operand_1 + operand_2) * ACCURACY);
                break;
            case SUBT:
                stack_pop(&stk, &operand_1);
                stack_pop(&stk, &operand_2);
                stack_push(&stk, (operand_2 - operand_1) * ACCURACY);
                break;
            case MULT:
                stack_pop(&stk, &operand_1);
                stack_pop(&stk, &operand_2);
                stack_push(&stk, (operand_2 * operand_1) * ACCURACY);
                break;
            case DIV:
                stack_pop(&stk, &operand_1);
                stack_pop(&stk, &operand_2);
                stack_push(&stk, (stack_elem_t)(((double)operand_2 / (double)operand_1) * ACCURACY));
                break;
            case OUT:
                stack_pop(&stk, &operand_1);
                printf("%lg\n", ((double)operand_1 / ACCURACY));
                break;
            case HLT:
                fclose(file);
                return;
            default:
                fclose(file);
                return;
        }
    }
    fclose(file);
}

size_t define_file_size(const char* file)
{
    struct stat buff = {};
    stat(file, &buff);

    return buff.st_size;
}

char* create_text_buffer(const size_t windows_number_elements, FILE* file)
{
    char* text_buffer = (char*)calloc(windows_number_elements + 1, sizeof(char));

    size_t real_number_elements = fread(text_buffer, sizeof(char), windows_number_elements, file);
    text_buffer[real_number_elements] = '\0';

    return text_buffer;
}
