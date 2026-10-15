//бинарный файл
//массив указателей на функции
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys\stat.h>
#include <stdlib.h>

#include "stack.h"

enum command_code_t
{
    PUSH   = 1,
    ADD    = 2,
    SUBT   = 3,
    MULT   = 4,
    DIV    = 5,
    OUT    = 6,
    HLT    = 7,
    POP_R  = 8,
    PUSH_R = 9,
    JMP    = 10,
};

struct processor_t
{
    struct stack_t stk;
    size_t stack_capacity;
    stack_elem_t argument_value;
    stack_elem_t operand_1;
    stack_elem_t operand_2;
    int* byte_code_buffer;
    size_t IP;
    stack_elem_t* reg_array;
};

#define ACCURACY 100000
#define REGISTER_NUMBER 4

void processor_init(struct processor_t* processor);
void processor_push(struct processor_t* processor);
void processor_add(struct processor_t* processor);
void processor_subt(struct processor_t* processor);
void processor_mult(struct processor_t* processor);
void processor_div(struct processor_t* processor);
void processor_out(struct processor_t* processor);
void processor_jmp(struct processor_t* processor);
void processor_destroy(struct processor_t* processor);
void processor_push_reg(struct processor_t* processor);
void processor_pop_reg(struct processor_t* processor);
int* create_file_buffer(const char* file_name, FILE* file);

struct source_location dump_file = {};

int main(const int argc, const char* argv[])
{
    struct processor_t processor = {};
    processor_init(&processor);

    FILE* file = fopen(argv[1], "rb");
    processor.byte_code_buffer = create_file_buffer(argv[1], file);
    fclose(file);

    do
    {
        switch (processor.byte_code_buffer[processor.IP++])
        {
            case PUSH:
                processor_push(&processor);
                break;
            case ADD:
                processor_add(&processor);
                break;
            case SUBT:
                processor_subt(&processor);
                break;
            case MULT:
                processor_mult(&processor);
                break;
            case DIV:
                processor_div(&processor);
                break;
            case OUT:
                processor_out(&processor);
                break;
            case HLT:
                processor_destroy(&processor);
                return 0;
            case JMP:
                processor_jmp(&processor);
                break;
            case PUSH_R:
                processor_push_reg(&processor);
                break;
            case POP_R:
                processor_pop_reg(&processor);
                break;
        }

    } while (processor.byte_code_buffer[processor.IP] != HLT);

    processor_destroy(&processor);
    return 0;
}

int* create_file_buffer(const char* file_name, FILE* file)
{
    struct stat buff = {};
    stat(file_name, &buff);

    int* text_buffer = (int*)calloc(buff.st_size + 1, sizeof(int));

    size_t real_number_elements = fread(text_buffer, sizeof(int), buff.st_size, file);

    return text_buffer;
}

void processor_init(struct processor_t* processor)
{
    processor -> stack_capacity = 10;
    processor -> stk = {};
    stack_init(&(processor -> stk), processor -> stack_capacity);

    processor -> reg_array = (stack_elem_t*)calloc(REGISTER_NUMBER, sizeof(stack_elem_t));

    processor -> argument_value = 0;
    processor -> operand_1      = 0;
    processor -> operand_2      = 0;
    processor -> IP             = 0;
}

void processor_push(struct processor_t* processor)
{
    processor -> argument_value = (processor -> byte_code_buffer)[processor -> IP++];
    stack_push(&(processor -> stk), (processor -> argument_value) * ACCURACY);
}

void processor_add(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    stack_pop(&(processor -> stk), &(processor -> operand_2));
    stack_push(&(processor -> stk), (processor -> operand_1) + (processor -> operand_2));
}

void processor_subt(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    stack_pop(&(processor -> stk), &(processor -> operand_2));
    stack_push(&(processor -> stk), (processor -> operand_2) - (processor -> operand_1));
}

void processor_mult(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    stack_pop(&(processor -> stk), &(processor -> operand_2));
    stack_push(&(processor -> stk), (processor -> operand_2) * (processor -> operand_1) / ACCURACY);
}

void processor_div(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    stack_pop(&(processor -> stk), &(processor -> operand_2));
    stack_push(&(processor -> stk), (stack_elem_t)(((double)(processor -> operand_2) / (double)(processor -> operand_1)) * ACCURACY));
}

void processor_out(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    printf("%lg\n", ((double)(processor -> operand_1) / ACCURACY));
}

void processor_destroy(struct processor_t* processor)
{
    free(processor -> byte_code_buffer);
    free(processor -> reg_array);
    free(&(processor -> stk));
}

void processor_jmp(struct processor_t* processor)
{
    processor -> IP = (processor -> byte_code_buffer)[processor -> IP];
}

void processor_push_reg(struct processor_t* processor)
{
    processor -> argument_value = (processor -> byte_code_buffer)[processor -> IP++];
    stack_push(&(processor -> stk), (processor -> reg_array)[(processor -> argument_value) - 1]);
}

void processor_pop_reg(struct processor_t* processor)
{
    processor -> argument_value = (processor -> byte_code_buffer)[processor -> IP++];
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    *((processor -> reg_array) + (processor -> argument_value) - 1) = processor -> operand_1;
}
