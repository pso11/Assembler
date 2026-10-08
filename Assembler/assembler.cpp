#include <stdio.h>
#include <string.h>
#include <sys\stat.h>
#include <stdlib.h>

#include "stack.h"

struct command_t
{
    const char* name;
    const char* code;
};

char* create_text_buffer(const size_t windows_number_elements, FILE* file);
void compilation(const char* source_file, const char* destination_file);
size_t define_file_size(const char* file);
void counting(struct stack_t stk, const char* byte_code_file);

int main(const int argc, const char* argv[])
{
    const char* code_file = argv[1];
    const char* byte_code_file = argv[2];
    if (argc < 2)
    {
        printf("Not enough arguments");
        return 0;
    }

    compilation(code_file, byte_code_file);

    return 0;
}

void compilation(const char* source_file, const char* destination_file)
{
    FILE* code_file = fopen(source_file, "r");
    FILE* byte_code_file = fopen(destination_file, "w+");

    char command_name[10] = {};
    char argument[10] = {};

    struct command_t array[] =
    {
        {"PUSH", "1"},
        {"ADD",  "2"},
        {"SUBT", "3"},
        {"MULT", "4"},
        {"DIV",  "5"},
        {"OUT",  "6"},
        {"HLT",  "7"}
    };

    const size_t windows_number_elements = define_file_size(source_file);
    char* code_buffer = create_text_buffer(windows_number_elements, code_file);
    char* byte_code_buffer = (char*)calloc(windows_number_elements, sizeof(char));
    byte_code_buffer[0] = '\0';

    size_t code_index = 0;
    size_t tem_increment = 0;
    while (sscanf(code_buffer + code_index, "%s%n", command_name, &tem_increment) != EOF)
    {
        code_index += tem_increment;

        for (size_t j = 0; j < sizeof(array)/ sizeof(array[0]); j++)
        {
            if (!strcmp(array[j].name, command_name))
            {
                if (!strcmp("PUSH", command_name))
                {
                    sscanf(code_buffer + code_index, "%s%n", argument, &tem_increment);
                    code_index += tem_increment;
                    strcat(byte_code_buffer, array[j].code);
                    strcat(byte_code_buffer, " ");
                    strcat(byte_code_buffer, argument);
                    strcat(byte_code_buffer, "\n");
                    break;
                }
                else
                {
                    strcat(byte_code_buffer, array[j].code);
                    strcat(byte_code_buffer, "\n");
                    break;
                }
            }
        }
    }

    fprintf(byte_code_file, "%s", byte_code_buffer);
    free(code_buffer);
    free(byte_code_buffer);
    fclose(code_file);
    fclose(byte_code_file);
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
