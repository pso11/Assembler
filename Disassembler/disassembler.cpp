// #include <TXLib.h>
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
void decompilation(const char* source_file, const char* destination_file);
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

    decompilation(code_file, byte_code_file);

    return 0;
}

void decompilation(const char* destination_file , const char* source_file)
{
    FILE* byte_code_file = fopen(source_file, "r");

    char command_code[10] = {};
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
    char* byte_code_buffer = create_text_buffer(windows_number_elements, byte_code_file);
    char* code_buffer = (char*)calloc(windows_number_elements, sizeof(char));
    code_buffer[0] = '\0';
    fclose(byte_code_file);
    // printf("%s", byte_code_buffer);

    size_t byte_code_index = 0;
    size_t tem_increment = 0;
    while (sscanf(byte_code_buffer + byte_code_index, "%s%n", command_code, &tem_increment) != EOF)
    {
        byte_code_index += tem_increment;
        printf("<%s>\n", command_code);
        for (size_t j = 0; j < sizeof(array)/ sizeof(array[0]); j++)
        {
            if (!strcmp(array[j].code, command_code))
            {
                if (!strcmp("1", command_code))
                {
                    sscanf(byte_code_buffer + byte_code_index, "%s%n", argument, &tem_increment);
                    printf("%s\n", argument);
                    byte_code_index += tem_increment;
                    strcat(code_buffer, array[j].name);
                    strcat(code_buffer, " ");
                    strcat(code_buffer, argument);
                    strcat(code_buffer, "\n");
                    break;
                }
                else
                {
                    strcat(code_buffer, array[j].name);
                    strcat(code_buffer, "\n");
                    break;
                }
            }
        }
    }
    FILE* code_file = fopen(destination_file, "w");
    fprintf(code_f ile, "%s", code_buffer);
    printf("good");
    free(code_buffer);
    free(byte_code_buffer);
    fclose(code_file);
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
