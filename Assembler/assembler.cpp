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

    compilation(argv[1], argv[2], argv[3]);

    return 0;
}

void compilation(const char* source_code_file_name, const char* destination_byte_file, const char* destination_bin_file)
{
    struct asm_t assembler = {};
    assembler_init(source_code_file_name, &assembler);

    while (sscanf(&assembler.code_buffer[assembler.code_index], "%s%n", assembler.command_name, &assembler.tem_increment) != EOF)
    {
        assembler.code_index += assembler.tem_increment;

        if (assembler.command_name[0] == ':')
        {
            sscanf(&command_name[1], "%s", argument);
            labels_array[atoi(argument) - 1] = bin_code_index;

            continue;
        }

        for (size_t j = 0; j < sizeof(command_array) / sizeof(command_array[0]); j++)
        {
            if (!strcmp(command_array[j].name, command_name))
            {
                if (command_array[j].num_arg == 0)
                {
                    strcat(byte_code_buffer, command_array[j].str_code);
                    strcat(byte_code_buffer, "\n");
                    bin_byte_code_buffer[bin_code_index++] = atoi(command_array[j].str_code);
                }

                else if (command_array[j].num_arg == 1)
                {
                    sscanf(&code_buffer[code_index], "%s%n", argument, &tem_increment);
                    code_index += tem_increment;

                    if (isalpha(argument[0]))
                    {
                        for (j = 7; j < sizeof(command_array) / sizeof(command_array[0]); j++)
                        {
                            if (!strcmp(command_array[j].name, command_name))
                                break;
                        }

                        size_t reg_index = 0;
                        for (; reg_index < sizeof(reg_array) / sizeof(reg_array[0]); reg_index++)
                        {
                            if (!strcmp(reg_array[reg_index].reg_name, argument))
                                break;
                        }

                        strcat(byte_code_buffer, command_array[j].str_code);
                        strcat(byte_code_buffer, " ");
                        strcat(byte_code_buffer, reg_array[reg_index].reg_code_str);
                        strcat(byte_code_buffer, "\n");

                        bin_byte_code_buffer[bin_code_index++] = atoi(command_array[j].str_code);
                        bin_byte_code_buffer[bin_code_index++] = atoi(reg_array[reg_index].reg_code_str);

                    }

                    else if (argument[0] == ':')
                    {
                        strcat(byte_code_buffer, command_array[j].str_code);
                        strcat(byte_code_buffer, " ");
                        sscanf(&argument[1], "%s%n", argument, &tem_increment);
                        sprintf(argument, "%d", labels_command_array[atoi(argument) - 1]);
                        strcat(byte_code_buffer, argument);
                        strcat(byte_code_buffer, "\n");

                        bin_byte_code_buffer[bin_code_index++] = atoi(command_array[j].str_code);
                        bin_byte_code_buffer[bin_code_index++] = atoi(argument);
                    }
                    else
                    {
                        strcat(byte_code_buffer, command_array[j].str_code);
                        strcat(byte_code_buffer, " ");
                        strcat(byte_code_buffer, argument);
                        strcat(byte_code_buffer, "\n");

                        bin_byte_code_buffer[bin_code_index++] = atoi(command_array[j].str_code);
                        bin_byte_code_buffer[bin_code_index++] = atoi(argument);
                    }
                }

                break;
            }
        }
    }

    FILE* byte_code_file = fopen(destination_byte_file, "w");
    fprintf(byte_code_file, "%s", byte_code_buffer);
    fclose(byte_code_file);

    FILE* bin_code_file = fopen(destination_byte_file, "wb");
    fwrite(bin_byte_code_buffer, sizeof(int), bin_code_index, bin_code_file);
    fclose(bin_code_file);

    assembler_destroy(&assembler);
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

    assembler -> command_name = (char*)calloc(MAX_COMMAND_LEN, sizeof(char)); MAX_ARG_LEN
    assembler -> argument     = (char*)calloc(MAX_ARG_LEN, sizeof(char))
    assembler -> code_index = 0;
    assembler -> bin_code_index = 0;
    assembler -> tem_increment = 0;
}

void assembler_destroy(struct asm_t* assembler)
{
    free(assembler -> code_buffer);
    free(assembler -> byte_code_buffer);
    free(assembler -> bin_byte_code_buffer);
    free(assembler -> labels_array);
    free(assembler -> command_name);
    free(assembler -> argument);
}
