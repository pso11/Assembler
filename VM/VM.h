struct processor_t
{
    struct stack_t data_stk;
    struct stack_t pointers_stk;
    size_t stack_capacity;
    stack_elem_t argument_value;
    stack_elem_t operand_1;
    stack_elem_t operand_2;
    int* byte_code_buffer;
    size_t IP;
    stack_elem_t* reg_array;
};

#define ACCURACY 100
#define REGISTER_NUMBER 5
#define STACK_CAPACITY 10

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
void processor_jae(struct processor_t* processor);
void processor_jb(struct processor_t* processor);
void processor_jbe(struct processor_t* processor);
void processor_je(struct processor_t* processor);
void processor_jne(struct processor_t* processor);
void processor_hlt(struct processor_t* processor);
void processor_work(struct processor_t* processor);
void processor_in(struct processor_t* processor);
void processor_call(struct processor_t* processor);
void processor_ret(struct processor_t* processor);
void processor_sqrt(struct processor_t* processor);
void processor_ja(struct processor_t* processor);

void processor_init(struct processor_t* processor);
int* create_file_buffer(const char* file_name);
size_t define_file_size(const char* file_name);



