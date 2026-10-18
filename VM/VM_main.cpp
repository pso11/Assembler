#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"
#include "VM.h"

#ifdef STACK_DEBUG
struct source_location dump_file = {};
#endif

int main(const int argc, const char* argv[])
{
    struct processor_t processor = {};
    processor_init(&processor);

    processor.byte_code_buffer = create_file_buffer(argv[1]);

    processor_work(&processor);

    processor_destroy(&processor);

    return 0;
}

