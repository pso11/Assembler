g++ -g -DSTACK_USE_INT -DSTACK_DEBUG VM_main.cpp VM_functions.cpp ..\..\Stack\stack.cpp -o VM.exe -I..\..\Stack -I..
VM.exe ..\Assembler\factorial.bin
