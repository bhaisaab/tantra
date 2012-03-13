#ifndef ELF_H
#define ELF_H

#include <types.h>

#define ELF_MAGIC 0x464C457FU /* "\x7FELF" in little endian */

// ELF file header
struct elfhdr {
  uint32_t magic;  // must equal ELF_MAGIC
  uint8_t elf[12];
  uint16_t type;
  uint16_t machine;
  uint32_t version;
  uint32_t entry;
  uint32_t phoff;
  uint32_t shoff;
  uint32_t flags;
  uint16_t ehsize;
  uint16_t phentsize;
  uint16_t phnum;
  uint16_t shentsize;
  uint16_t shnum;
  uint16_t shstrndx;
};

// Program section header
struct proghdr {
  uint32_t type;
  uint32_t off;
  uint32_t vaddr;
  uint32_t paddr;
  uint32_t filesz;
  uint32_t memsz;
  uint32_t flags;
  uint32_t align;
};

struct Secthdr {
  uint32_t sh_name;
  uint32_t sh_type;
  uint32_t sh_flags;
  uint32_t sh_addr;
  uint32_t sh_offset;
  uint32_t sh_size;
  uint32_t sh_link;
  uint32_t sh_info;
  uint32_t sh_addralign;
  uint32_t sh_entsize;
};

// Values for proghdr::type
#define ELF_PROG_LOAD           1

// Flag bits for Proghdr::p_flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4

// Values for Secthdr::sh_type
#define ELF_SHT_NULL            0
#define ELF_SHT_PROGBITS        1
#define ELF_SHT_SYMTAB          2
#define ELF_SHT_STRTAB          3

// Values for Secthdr::sh_name
#define ELF_SHN_UNDEF           0

#endif
