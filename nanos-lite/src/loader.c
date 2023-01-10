#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf_ehdr;
  int elf_file = fs_open(filename, 0, 0);
  fs_read(elf_file, &elf_ehdr, sizeof(elf_ehdr));

  assert(*(uint32_t *)elf_ehdr.e_ident == 0x464c457f);

  #if defined(__ISA_AM_NATIVE__)
  # define EXPECT_TYPE EM_X86_64
  #elif defined(__ISA_X86__)
  # define EXPECT_TYPE EM_X86_64
  #elif defined(__ISA_MIPS32__)
  # define EXPECT_TYPE EM_MIPS
  #elif defined(__ISA_RISCV32__) || defined(__ISA_RISCV64__)
  # define EXPECT_TYPE EM_RISCV
  #else
  # error Unsupported ISA
  #endif

  assert(EXPECT_TYPE == elf_ehdr.e_machine);


  for(int i = 0; i < elf_ehdr.e_phnum; i++){
    Elf_Phdr elf_phdr;
    fs_lseek(elf_file, elf_ehdr.e_phoff + elf_ehdr.e_phentsize * i, SEEK_SET);
    fs_read(elf_file, &elf_phdr, sizeof(elf_phdr));

    if(elf_phdr.p_type == PT_LOAD){
      fs_lseek(elf_file, elf_phdr.p_offset, SEEK_SET);
      fs_read(elf_file, (void *)elf_phdr.p_vaddr, elf_phdr.p_filesz);
      memset((void *)elf_phdr.p_vaddr + elf_phdr.p_filesz, 0, elf_phdr.p_memsz - elf_phdr.p_filesz);
    }
  }
  fs_close(elf_file);
  return elf_ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void (*entry)(void *), void *arg){
  Area kstack;
  kstack.start = (void *)pcb;
  kstack.end = kstack.start + STACK_SIZE;
  pcb->cp = kcontext(kstack, entry, arg);
}

// void context_uload(PCB *pcb, const char *filename){
void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]){
  // char *string_area = (char *)heap.end;
  char *string_area = (char *)new_page(8);
  int argv_num = 0, envp_num = 0;
  if(argv) while(argv[argv_num]) argv_num++;
  if(envp) while(envp[envp_num]) envp_num++;
  // printf("argv_num=%d, envp_num=%d\n", argv_num, envp_num);
  // if(argv_num == 2)  printf("argv[0]=%s, argv[1]=%s\n", argv[0], argv[1]);
  char *argv_c[argv_num];
  char *envp_c[envp_num];
  for(int i = 0; i < argv_num; i++){
    string_area -= ROUNDUP(strlen(argv[i]) + 1, 4); // +1 for the '\0'
    strcpy(string_area, argv[i]);
    argv_c[i] = string_area;
  }
  for(int i = 0; i < envp_num; i++){
    string_area -= ROUNDUP(strlen(envp[i]) + 1, 4); // +1 for the '\0'
    char *t = string_area; // test
    strcpy(string_area, envp[i]);
    envp_c[i] = string_area;
    assert(t == envp[i]); // test
  }
  uintptr_t *string_a = (uintptr_t *)string_area;
  string_a--; *string_a = (uintptr_t)NULL;

  for(int i = envp_num - 1; i >= 0; i--){
    string_a--;
    *string_a = (uintptr_t)envp_c[i];
  }  
  string_a--; *string_a = (uintptr_t)NULL;

  for(int i = argv_num - 1; i >= 0; i--){
    string_a--;
    *string_a = (uintptr_t)argv_c[i];
  }

  string_a--;
  *string_a = (uintptr_t)argv_num; 

  Area kstack;
  kstack.start = (void *)pcb;
  kstack.end = kstack.start + STACK_SIZE;
  pcb->cp = ucontext(NULL, kstack, (void *)loader(pcb, filename));
  pcb->cp->GPRx = (uintptr_t)string_a;
}

