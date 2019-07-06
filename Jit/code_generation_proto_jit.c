#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

typedef long(*fn)(long);

fn compile_identiy(void)
{
    char *memory = mmap(NULL,             // address
                      4096,             // size
                      PROT_READ | PROT_WRITE | PROT_EXEC,
                      MAP_PRIVATE | MAP_ANONYMOUS,
                      -1,               // fd (not used here)
                      0);               // offset (not used here)
    if (!memory) {
        perror("failed to allocate memory");
        exit(1);
    }

    int i = 0;

    // mov %rdi, %rax
    memory[i++] = 0x48; // REX.W prefix
    memory[i++] = 0x8b; // mov opcode,register/register
    memory[i++] = 0xc7; // mod/rm for $rdi->rax;

    // ret
    memory[i++] = 0xc3;           // RET opcode

    return (fn) memory;

}

int main()
{
  fn f = compile_identiy();
  int i;
  for (i = 0; i < 10; ++i)
    printf("f(%d) = %ld\n", i, (*f)(i));
  munmap(f, 4096);
}