#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <stdlib.h>

#include "stack.h"
#include "assembler.h"

int main(const int argc, const char* argv[])
{
    if (argc < 3)
    {
        printf("Not enough arguments\n");
        return 0;
    }

    struct asm_t assembler = {};
    assembler_init(argv[1], &assembler);

    compilation(&assembler);

    fill_byte_code_file(&assembler, argv[2]);
    fill_bin_byte_code_file(&assembler, argv[3]);

    assembler_destroy(&assembler);

    return 0;
}

void compilation(struct asm_t* assembler)
{
    assembler_first_pass(assembler);
    assembler_second_pass(assembler);
}

char* create_file_buffer(const char* file_name, FILE* file)
{
    size_t file_size = define_file_size(file_name);
    char* text_buffer = (char*)calloc(file_size + 1, sizeof(char));

    size_t real_number_elements = fread(text_buffer, sizeof(char), file_size, file);
    text_buffer[real_number_elements] = '\0';

    return text_buffer;
}

size_t define_file_size(const char* file_name)
{
    struct stat buff = {};
    stat(file_name, &buff);
    return buff.st_size;
}

void assembler_init(const char* source_code_file_name, struct asm_t* assembler)
{
    FILE* code_file = fopen(source_code_file_name, "r");
    assembler -> code_buffer           = create_file_buffer(source_code_file_name, code_file);
    assembler -> byte_code_buffer      = (char*)calloc(define_file_size(source_code_file_name), sizeof(char));
    assembler -> bin_byte_code_buffer  = (int*) calloc(define_file_size(source_code_file_name), sizeof(int));
    (assembler -> byte_code_buffer)[0] = '\0';
    fclose(code_file);

    assembler -> labels_array = (int*)calloc(LABELS_NUMBER, sizeof(int));
    for (size_t i = 0; i < LABELS_NUMBER; i++)
        (assembler -> labels_array)[i] = -1;

    assembler -> bin_code_index = 0;
}

void assembler_destroy(struct asm_t* assembler)
{
    free(assembler -> code_buffer);
    free(assembler -> byte_code_buffer);
    free(assembler -> bin_byte_code_buffer);
}

void fill_byte_code_file(struct asm_t* assembler, const char* byte_dile)
{
    FILE* byte_code_file = fopen(byte_dile, "w+");
    fprintf(byte_code_file, "%s", assembler -> byte_code_buffer);
    fclose(byte_code_file);
}

void fill_bin_byte_code_file(struct asm_t* assembler, const char* bin_byte_dile)
{
    FILE* bin_code_file = fopen(bin_byte_dile, "wb+");
    fwrite(assembler -> bin_byte_code_buffer, sizeof(int), assembler -> bin_code_index, bin_code_file);
    fclose(bin_code_file);
}

void assembler_first_pass(struct asm_t* assembler)
{
    size_t code_index = 0;
    size_t bin_code_index = 0;
    size_t tem_increment = 0;
    char command_name[MAX_COMMAND_LEN] = {};
    char argument[MAX_ARG_LEN] = {};
    while (sscanf(&assembler -> code_buffer[code_index], "%s%n", command_name, &tem_increment) != EOF)
    {
        code_index += tem_increment;

        if (command_name[0] == ':')
        {
            sscanf(&command_name[1], "%s", argument);
            assembler -> labels_array[atoi(argument) - 1] = bin_code_index;

            continue;
        }
        bin_code_index++;

        for (size_t command_idx = 0; command_idx < sizeof(command_array) / sizeof(command_array[0]); command_idx++)
        {
            if (!strcmp(command_array[command_idx].name, command_name))
            {
                if (ONE_ARG)
                {
                    sscanf(&assembler -> code_buffer[code_index], "%s%n", argument, &tem_increment);
                    code_index += tem_increment;
                    bin_code_index++;;
                }

                break;
            }
        }
    }
}

void assembler_second_pass(struct asm_t* assembler)
{
    size_t code_index = 0;
    size_t tem_increment = 0;
    char command_name[MAX_COMMAND_LEN] = {};
    char argument[MAX_ARG_LEN] = {};
    while (sscanf(&assembler -> code_buffer[code_index], "%s%n", command_name, &tem_increment) != EOF)
    {
        code_index += tem_increment;

        if (command_name[0] == ':')
        {
            sscanf(&command_name[1], "%s", argument);
            assembler -> labels_array[atoi(argument) - 1] = assembler -> bin_code_index;

            continue;
        }

        for (size_t command_idx = 0; command_idx < sizeof(command_array) / sizeof(command_array[0]); command_idx++)
        {
            if (!strcmp(command_array[command_idx].name, command_name))
            {
                if (ZERO_ARGS)
                {
                    strcat(assembler -> byte_code_buffer, command_array[command_idx].str_code);
                    strcat(assembler -> byte_code_buffer, "\n");
                    assembler -> bin_byte_code_buffer[assembler -> bin_code_index++] = atoi(command_array[command_idx].str_code);
                }

                else if (ONE_ARG)
                {
                    sscanf(&assembler -> code_buffer[code_index], "%s%n", argument, &tem_increment);
                    code_index += tem_increment;

                    if (isalpha(argument[0]))
                    {
                        for (command_idx = 7; command_idx < sizeof(command_array) / sizeof(command_array[0]); command_idx++)
                        {
                            if (!strcmp(command_array[command_idx].name, command_name))
                                break;
                        }

                        size_t reg_index = 0;
                        for (; reg_index < sizeof(reg_array) / sizeof(reg_array[0]); reg_index++)
                        {
                            if (!strcmp(reg_array[reg_index].reg_name, argument))
                                break;
                        }

                        strcat(assembler -> byte_code_buffer, command_array[command_idx].str_code);
                        strcat(assembler -> byte_code_buffer, " ");
                        strcat(assembler -> byte_code_buffer, reg_array[reg_index].reg_code_str);
                        strcat(assembler -> byte_code_buffer, "\n");

                        assembler -> bin_byte_code_buffer[assembler -> bin_code_index++] = atoi(command_array[command_idx].str_code);
                        assembler -> bin_byte_code_buffer[assembler -> bin_code_index++] = atoi(reg_array[reg_index].reg_code_str);

                    }

                    else if (LABLE_ARG)
                    {
                        strcat(assembler -> byte_code_buffer, command_array[command_idx].str_code);
                        strcat(assembler -> byte_code_buffer, " ");
                        sscanf(&argument[1], "%s%n", argument, &tem_increment);
                        sprintf(argument, "%d", assembler -> labels_array[atoi(argument) - 1]);
                        strcat(assembler -> byte_code_buffer, argument);
                        strcat(assembler -> byte_code_buffer, "\n");

                        assembler -> bin_byte_code_buffer[assembler -> bin_code_index++] = atoi(command_array[command_idx].str_code);
                        assembler -> bin_byte_code_buffer[assembler -> bin_code_index++] = atoi(argument);
                    }
                    else
                    {
                        strcat(assembler -> byte_code_buffer, command_array[command_idx].str_code);
                        strcat(assembler -> byte_code_buffer, " ");
                        strcat(assembler -> byte_code_buffer, argument);
                        strcat(assembler -> byte_code_buffer, "\n");

                        assembler -> bin_byte_code_buffer[assembler -> bin_code_index++] = atoi(command_array[command_idx].str_code);
                        assembler -> bin_byte_code_buffer[assembler -> bin_code_index++] = atoi(argument);
                    }
                }

                break;
            }
        }
    }

}
