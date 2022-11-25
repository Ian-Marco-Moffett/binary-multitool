#include <elfinfo.h>
#include <stdint.h>
#include <stdlib.h>
#include <elf.h>


static void machine_type(Elf64_Ehdr* eh) {
  switch (eh->e_machine) {
    case EM_386:
      printf("ELFINFO: MachineType => Intel 80386\n");
      break;
    case EM_MIPS:
      printf("ELFINFO: MachineType => MIPS RS3000\n");
      break;
    case EM_PPC:
      printf("ELFINFO: MachineType => PowerPC\n");
      break;
    case EM_ARM:
      printf("ELFINFO: MachineType => ARM\n");
      break;
    case EM_X86_64:
      printf("ELFINFO: MachineType => AMD64\n");
      break;
  }
}



static inline void program_info(Elf64_Ehdr* eh) {
  printf("ELFINFO: Program header table entry size => %d\n", eh->e_phentsize);
  printf("ELFINFO: Program header entries: %d\n", eh->e_phnum);
  printf("ELFINFO: Section header offset => 0x%X\n", eh->e_shoff);
  printf("ELFINFO: Program header table offset => 0x%X\n", eh->e_phoff);
}


static void read_phdrs(Elf64_Ehdr* eh) {
  size_t phdrs_size = eh->e_phnum * eh->e_phentsize;
  Elf64_Phdr* phdrs = malloc(phdrs_size);

  const char* ptr = (const char*)eh + eh->e_phoff;

  for (uint64_t i = 0; i < phdrs_size; ++i) {
    ((char*)phdrs)[i] = ptr[i];
  }

  for (Elf64_Phdr* phdr = phdrs; (char*)phdr < (char*)phdrs + eh->e_phnum * eh->e_phentsize; phdr = (Elf64_Phdr*)((char*)phdr + eh->e_phentsize)) {
    switch (phdr->p_type) {
      case PT_LOAD:
        printf("ELFINFO: Found loadable segment @0x%X with size %d\n", phdr->p_vaddr, phdr->p_memsz);
        break; 
    }
  }

  free(phdrs);
}


static void read_elf(const char* const ORIG_BUF_PTR) {
  Elf64_Ehdr* eh = (Elf64_Ehdr*)ORIG_BUF_PTR;

  uint8_t is_magic_correct = 
       eh->e_ident[EI_MAG0] == ELFMAG0
    && eh->e_ident[EI_MAG1] == ELFMAG1
    && eh->e_ident[EI_MAG2] == ELFMAG2
    && eh->e_ident[EI_MAG3] == ELFMAG3;

  if (!(is_magic_correct)) {
    printf("ELFINFO_ERROR: This is not a valid ELF (magic bytes incorrect)!\n");
    return;
  }

  machine_type(eh);
  program_info(eh);
  read_phdrs(eh);
}


void dump_elf(FILE* fp) {
  fseek(fp, 0, SEEK_END);
  size_t fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char* orig_buf_ptr = malloc(sizeof(char) * fsize);
  fread(orig_buf_ptr, sizeof(char), fsize, fp);

  read_elf(orig_buf_ptr);
  free(orig_buf_ptr);
}
