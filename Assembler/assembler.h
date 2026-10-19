struct command_t
{
    const char* name;
    const char* str_code;
    size_t num_arg;
};

struct command_t command_array[] =
{
    {"PUSH", "1",   1},
    {"ADD",  "2",   0},
    {"SUBT", "3",   0},
    {"MULT", "4",   0},
    {"DIV",  "5",   0},
    {"OUT",  "6",   0},
    {"HLT",  "7",   0},
    {"JMP",  "8",   1},
    {"JAE",  "9",   1},
    {"JB",   "10",  1},
    {"JBE",  "11",  1},
    {"JE",   "12",  1},
    {"JNE",  "13",  1},
    {"POP",  "14",  1},
    {"IN",   "15",  0},
    {"CALL", "16",  1},
    {"RET",  "17",  0},
    {"SQRT", "18",  0},
    {"JA",   "19",  1}
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
    {"DX", "4"},
    {"FX", "5"}
};

struct asm_t
{
    char*  code_buffer;
    char*  byte_code_buffer;
    int*   bin_byte_code_buffer;
    int*   labels_array;
    size_t bin_code_index;
    size_t reg_index;
    char* argument;
    size_t command_idx;
};

#define LABELS_NUMBER   20
#define MAX_COMMAND_LEN 10
#define MAX_ARG_LEN     10
#define ZERO_ARGS command_array[assembler -> command_idx].num_arg == 0
#define ONE_ARG   command_array[assembler -> command_idx].num_arg == 1
#define LABLE_ARG (assembler -> argument)[0] == ':'

char* create_file_buffer(const char* file_name, FILE* file);
void compilation(struct asm_t* assembler);
size_t define_file_size(const char* file_name);
void assembler_init(const char* source_code_file_name, struct asm_t* assembler);
void assembler_destroy(struct asm_t* assembler);
void move_along_code_array(struct asm_t* assembler);
void fill_byte_code_file(struct asm_t* assembler, const char* byte_dile);
void fill_bin_byte_code_file(struct asm_t* assembler, const char* bin_byte_dile);
void assembler_first_pass(struct asm_t* assembler);
void assembler_second_pass(struct asm_t* assembler);
void fwrite_if_alpha(struct asm_t* assembler, const char* type_name);
void fwrite_if_digit(struct asm_t* assembler, const char* type_name);
void fwrite_if_label(struct asm_t* assembler);
void find_register(struct asm_t* assembler);
void fwrite_if_zero_args(struct asm_t* assembler);
