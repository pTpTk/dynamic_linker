#pragma once

#include <stdint.h>

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