#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <stdlib.h>

#include "stack.h"

struct command_t
{
    const char* name;
    const char* str_code;
    size_t num_arg;
};

struct command_t array[] =
{
    {"PUSH", "1",   1},
    {"ADD",  "2",   0},
    {"SUBT", "3",   0},
    {"MULT", "4",   0},
    {"DIV",  "5",   0},
    {"OUT",  "6",   0},
    {"HLT",  "7",   0},
    {"JMP",  "8",   1},
    {"POP",  "9",   1},
    {"PUSH", "10",  1}
};

struct register_t
{
    const char* reg_name;
    const char* reg_code_str;
};

struct register_t reg_array[] =
{
    {"AX", "1"},
    {"BX", "2"},
    {"CX", "3"},
    {"DX", "4"}
};

char* create_file_buffer(const char* file_name, FILE* file);
void compilation(const char* source_file, const char* destination_file, const char* bin_code_file);
size_t define_file_size(const char* file_name);

int main(const int argc, const char* argv[])
{
    const char* code_file      = argv[1];
    const char* byte_code_file = argv[2];
    const char* bin_code_file  = argv[3];
    if (argc < 2)
    {
        printf("Not enough arguments\n");
        return 0;
    }

    compilation(code_file, byte_code_file, bin_code_file);

    return 0;
}

void compilation(const char* source_file, const char* destination_file, const char* destination_bin_file)
{
    FILE* code_file = fopen(source_file, "r");
    char* code_buffer          = create_file_buffer(source_file, code_file);
    char* byte_code_buffer     = (char*)calloc(define_file_size(source_file), sizeof(char));
    int*  bin_byte_code_buffer = (int*) calloc(define_file_size(source_file), sizeof(int));
    byte_code_buffer[0] = '\0';
    fclose(code_file);

    int mark_array[5] = {-1, -1, -1, -1, -1};
    char command_name[10] = {};
    char argument[10] = {};
    size_t code_index = 0;
    size_t bin_code_index = 0;
    size_t tem_increment = 0;

    while (sscanf(&code_buffer[code_index], "%s%n", command_name, &tem_increment) != EOF)
    {
        code_index += tem_increment;

        for (size_t j = 0; j < sizeof(array) / sizeof(array[0]); j++)
        {
            if (!strcmp(array[j].name, command_name))
            {
                if (array[j].num_arg == 0)
                {
                    strcat(byte_code_buffer, array[j].str_code);
                    strcat(byte_code_buffer, "\n");
                    bin_byte_code_buffer[bin_code_index++] = atoi(array[j].str_code);
                }

                else if (array[j].num_arg == 1)
                {
                    sscanf(&code_buffer[code_index], "%s%n", argument, &tem_increment);
                    code_index += tem_increment;


                    if (isalpha(argument[0]))
                    {
                        for (j = 8; j < sizeof(array) / sizeof(array[0]); j++)
                        {
                            if (!strcmp(array[j].name, command_name))
                                break;
                        }

                        size_t reg_index = 0;
                        for (; reg_index < sizeof(reg_array) / sizeof(reg_array[0]); reg_index++)
                        {
                            if (!strcmp(reg_array[reg_index].reg_name, argument))
                                break;
                        }

                        printf("%p %p", byte_code_buffer, array[j].str_code);
                        strcat(byte_code_buffer, array[j].str_code);
                        strcat(byte_code_buffer, " ");

                        printf("%p %p", byte_code_buffer, reg_array[reg_index].reg_code_str);
                        strcat(byte_code_buffer, reg_array[reg_index].reg_code_str);
                        strcat(byte_code_buffer, "\n");

                        bin_byte_code_buffer[bin_code_index++] = atoi(array[j].str_code);
                        bin_byte_code_buffer[bin_code_index++] = atoi(reg_array[reg_index].reg_code_str);

                    }

                    else
                    {
                        printf("%p %p", byte_code_buffer, array[j].str_code);
                        strcat(byte_code_buffer, array[j].str_code);
                        strcat(byte_code_buffer, " ");
                        printf("%p %p", byte_code_buffer, argument);
                        strcat(byte_code_buffer, argument);
                        strcat(byte_code_buffer, "\n");

                        bin_byte_code_buffer[bin_code_index++] = atoi(array[j].str_code);
                        bin_byte_code_buffer[bin_code_index++] = atoi(argument);
                    }
                }
            }
        }
    }

    FILE* byte_code_file = fopen(destination_file, "w");
    fprintf(byte_code_file, "%s", byte_code_buffer);
    fclose(byte_code_file);

    FILE* bin_code_file = fopen(destination_bin_file, "wb");
    fwrite(bin_byte_code_buffer, sizeof(int), bin_code_index, bin_code_file);
    fclose(bin_code_file);

    free(code_buffer);
    free(byte_code_buffer);
    free(bin_byte_code_buffer);
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


