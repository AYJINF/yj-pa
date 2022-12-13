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

  return elf_ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

