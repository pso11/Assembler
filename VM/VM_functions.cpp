#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"
#include "VM.h"
#include "command_e.h"
#include "functions_array.h"

int* create_file_buffer(const char* file_name)
{
    FILE* file = fopen(file_name, "rb");

    size_t file_size = define_file_size(file_name);

    int* text_buffer = (int*)calloc(file_size + 1, sizeof(int));

    fread(text_buffer, sizeof(int), file_size, file);

    fclose(file);

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
    stack_push(&(processor -> stk), ((processor -> operand_2) * (processor -> operand_1)) / ACCURACY);
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

size_t define_file_size(const char* file_name)
{
    struct stat buff = {};
    stat(file_name, &buff);
    return buff.st_size;
}

void processor_jae(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    stack_pop(&(processor -> stk), &(processor -> operand_2));
    if ((processor -> operand_2) >= processor -> operand_1)
        processor_jmp(processor);
}

void processor_jb(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    stack_pop(&(processor -> stk), &(processor -> operand_2));
    if ((processor -> operand_2) < (processor -> operand_1))
        processor_jmp(processor);
}

void processor_jbe(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    stack_pop(&(processor -> stk), &(processor -> operand_2));
    if ((processor -> operand_2) <= (processor -> operand_1))
        processor_jmp(processor);
}

void processor_je(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    stack_pop(&(processor -> stk), &(processor -> operand_2));
    if ((processor -> operand_2) == (processor -> operand_1))
        processor_jmp(processor);
}

void processor_jne(struct processor_t* processor)
{
    stack_pop(&(processor -> stk), &(processor -> operand_1));
    stack_pop(&(processor -> stk), &(processor -> operand_2));
    if ((processor -> operand_2) != (processor -> operand_1))
        processor_jmp(processor);
}


void processor_hlt(struct processor_t* processor)
{
    ;
}


void processor_work(struct processor_t* processor)
{
    while (processor -> byte_code_buffer[processor -> IP] != HLT)
        function_array[processor -> byte_code_buffer[(processor -> IP)++] - 1](processor);
}
