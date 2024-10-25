#pragma once

#include "typedef.h"

#define AT_NULL     0       /* End of vector */
#define AT_PHDR     3       /* Program headers for program */
#define AT_ENTRY    9       /* Entry point of program */

#define PT_LOAD     1       /* Loadable program segment */
#define PT_DYNAMIC  2       /* Dynamic linking information */
#define PT_INTERP   3       /* Program interpreter */

#define DT_NULL     0       /* Marks end of dynamic section */
#define DT_NEEDED   1       /* Name of needed library */
#define DT_PLTRELSZ	2       /* Size in bytes of PLT relocs */
#define DT_PLTGOT   3       /* Processor defined value */
#define DT_STRTAB   5       /* Address of string table */
#define DT_SYMTAB	6       /* Address of symbol table */
#define DT_JMPREL	23      /* Address of PLT relocs */

typedef struct
{
    uint32_t p_type;            /* Segment type */
    uint32_t p_flags;           /* Segment flags */
    uint64_t p_offset;          /* Segment file offset */
    uint64_t p_vaddr;           /* Segment virtual address */
    uint64_t p_paddr;           /* Segment physical address */
    uint64_t p_filesz;          /* Segment size in file */
    uint64_t p_memsz;           /* Segment size in memory */
    uint64_t p_align;           /* Segment alignment */
} PHDR;

typedef struct {
  int64_t d_tag;
  union {
      uint64_t d_val;
      uint64_t d_ptr;
  } d_un;
} Dynamic;

typedef struct
{
  uint64_t	r_offset;		/* Address */
  uint64_t	r_info;			/* Relocation type and symbol index */
  int64_t	r_addend;       /* Addend */
} Rela;

typedef struct
{
  uint32_t	st_name;        /* Symbol name (string tbl index) */
  unsigned char	st_info;    /* Symbol type and binding */
  unsigned char st_other;	/* Symbol visibility */
  uint16_t	st_shndx;       /* Section index */
  uint64_t	st_value;       /* Symbol value */
  uint64_t	st_size;        /* Symbol size */
} Sym;

typedef struct {
    void * r_jmprel;
    void * r_symtab;
    void * r_strtab;
    void * r_lib;
} Resolve;
