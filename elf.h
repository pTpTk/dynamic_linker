#pragma once

#include "typedef.h"

#define AT_NULL		0		/* End of vector */
#define AT_PHDR		3		/* Program headers for program */
#define AT_ENTRY	9		/* Entry point of program */

#define PT_LOAD		1		/* Loadable program segment */
#define PT_DYNAMIC	2		/* Dynamic linking information */
#define PT_INTERP	3		/* Program interpreter */

#define DT_NULL		0		/* Marks end of dynamic section */
#define DT_NEEDED	1		/* Name of needed library */
#define DT_STRTAB	5		/* Address of string table */

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