/* @LICENSE(UNSW_OZPLB) */

/*
 * Australian Public Licence B (OZPLB)
 *
 * Version 1-0
 *
 * Copyright (c) 2004 University of New South Wales
 *
 * All rights reserved.
 *
 * Developed by: Operating Systems and Distributed Systems Group (DiSy)
 *               University of New South Wales
 *               http://www.disy.cse.unsw.edu.au
 *
 * Permission is granted by University of New South Wales, free of charge, to
 * any person obtaining a copy of this software and any associated
 * documentation files (the "Software") to deal with the Software without
 * restriction, including (without limitation) the rights to use, copy,
 * modify, adapt, merge, publish, distribute, communicate to the public,
 * sublicense, and/or sell, lend or rent out copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimers.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimers in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of University of New South Wales, nor the names of its
 *       contributors, may be used to endorse or promote products derived
 *       from this Software without specific prior written permission.
 *
 * EXCEPT AS EXPRESSLY STATED IN THIS LICENCE AND TO THE FULL EXTENT
 * PERMITTED BY APPLICABLE LAW, THE SOFTWARE IS PROVIDED "AS-IS", AND
 * NATIONAL ICT AUSTRALIA AND ITS CONTRIBUTORS MAKE NO REPRESENTATIONS,
 * WARRANTIES OR CONDITIONS OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO ANY REPRESENTATIONS, WARRANTIES OR CONDITIONS
 * REGARDING THE CONTENTS OR ACCURACY OF THE SOFTWARE, OR OF TITLE,
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT,
 * THE ABSENCE OF LATENT OR OTHER DEFECTS, OR THE PRESENCE OR ABSENCE OF
 * ERRORS, WHETHER OR NOT DISCOVERABLE.
 *
 * TO THE FULL EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL
 * NATIONAL ICT AUSTRALIA OR ITS CONTRIBUTORS BE LIABLE ON ANY LEGAL
 * THEORY (INCLUDING, WITHOUT LIMITATION, IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY CLAIM, LOSS, DAMAGES OR OTHER
 * LIABILITY, INCLUDING (WITHOUT LIMITATION) LOSS OF PRODUCTION OR
 * OPERATION TIME, LOSS, DAMAGE OR CORRUPTION OF DATA OR RECORDS; OR LOSS
 * OF ANTICIPATED SAVINGS, OPPORTUNITY, REVENUE, PROFIT OR GOODWILL, OR
 * OTHER ECONOMIC LOSS; OR ANY SPECIAL, INCIDENTAL, INDIRECT,
 * CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES, ARISING OUT OF OR IN
 * CONNECTION WITH THIS LICENCE, THE SOFTWARE OR THE USE OF OR OTHER
 * DEALINGS WITH THE SOFTWARE, EVEN IF NATIONAL ICT AUSTRALIA OR ITS
 * CONTRIBUTORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH CLAIM, LOSS,
 * DAMAGES OR OTHER LIABILITY.
 *
 * If applicable legislation implies representations, warranties, or
 * conditions, or imposes obligations or liability on University of New South
 * Wales or one of its contributors in respect of the Software that
 * cannot be wholly or partly excluded, restricted or modified, the
 * liability of University of New South Wales or the contributor is limited, to
 * the full extent permitted by the applicable legislation, at its
 * option, to:
 * a.  in the case of goods, any one or more of the following:
 * i.  the replacement of the goods or the supply of equivalent goods;
 * ii.  the repair of the goods;
 * iii. the payment of the cost of replacing the goods or of acquiring
 *  equivalent goods;
 * iv.  the payment of the cost of having the goods repaired; or
 * b.  in the case of services:
 * i.  the supplying of the services again; or
 * ii.  the payment of the cost of having the services supplied again.
 *
 * The construction, validity and performance of this licence is governed
 * by the laws in force in New South Wales, Australia.
 */
#pragma once

#include <stdint.h>
#include <elf.h>

/* ELF header functions */
int elf64_checkFile(elf_t *elf);

int elf64_checkProgramHeaderTable(elf_t *elf);

int elf64_checkSectionTable(elf_t *elf);

static inline bool
elf_isElf64(elf_t *elf)
{
    return elf->elfClass == ELFCLASS64;
}

static inline Elf64_Ehdr
elf64_getHeader(elf_t *elf)
{
    return *(Elf64_Ehdr *) elf->elfFile;
}

static inline uintptr_t
elf64_getEntryPoint(elf_t *file)
{
    return elf64_getHeader(file).e_entry;
}

static inline Elf64_Phdr *
elf64_getProgramHeaderTable(elf_t *file)
{
    return (Elf64_Phdr*) ((uint8_t*) file->elfFile + elf64_getHeader(file).e_phoff);
}

static inline Elf64_Shdr *
elf64_getSectionTable(elf_t *file)
{
    return (Elf64_Shdr*) ((uint8_t*) file->elfFile + elf64_getHeader(file).e_shoff);
}

static inline size_t
elf64_getNumProgramHeaders(elf_t *file)
{
    return elf64_getHeader(file).e_phnum;
}

static inline size_t
elf64_getNumSections(elf_t *elf) {
    return elf64_getHeader(elf).e_shnum;
}

static inline size_t
elf64_getSectionStringTableIndex(elf_t *elf)
{
    return elf64_getHeader(elf).e_shstrndx;
}


/* Section header functions */
static inline size_t
elf64_getSectionNameOffset(elf_t *elf, size_t s)
{
    return elf64_getSectionTable(elf)[s].sh_name;
}

static inline uint32_t
elf64_getSectionType(elf_t *file, size_t s)
{
    return elf64_getSectionTable(file)[s].sh_type;
}

static inline size_t
elf64_getSectionFlags(elf_t *file, size_t s)
{
    return elf64_getSectionTable(file)[s].sh_flags;
}

static inline uintptr_t
elf64_getSectionAddr(elf_t *elf, size_t i)
{
    return elf64_getSectionTable(elf)[i].sh_addr;
}

static inline size_t
elf64_getSectionOffset(elf_t *elf, size_t i)
{
    return elf64_getSectionTable(elf)[i].sh_offset;
}

static inline size_t
elf64_getSectionSize(elf_t *elf, size_t i)
{
    return elf64_getSectionTable(elf)[i].sh_size;
}

static inline uint32_t
elf64_getSectionLink(elf_t *elf, size_t i)
{
    return elf64_getSectionTable(elf)[i].sh_link;
}

static inline uint32_t
elf64_getSectionInfo(elf_t *elf, size_t i)
{
    return elf64_getSectionTable(elf)[i].sh_info;
}

static inline size_t
elf64_getSectionAddrAlign(elf_t *elf, size_t i)
{
    return elf64_getSectionTable(elf)[i].sh_addralign;
}

static inline size_t
elf64_getSectionEntrySize(elf_t *elf, size_t i)
{
    return elf64_getSectionTable(elf)[i].sh_entsize;
}


/* Program header functions */
static inline uint32_t
elf64_getProgramHeaderType(elf_t *file, size_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_type;
}

static inline size_t
elf64_getProgramHeaderOffset(elf_t *file, size_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_offset;
}

static inline uintptr_t
elf64_getProgramHeaderVaddr(elf_t *file, size_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_vaddr;
}

static inline uintptr_t
elf64_getProgramHeaderPaddr(elf_t *file, size_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_paddr;
}

static inline size_t
elf64_getProgramHeaderFileSize(elf_t *file, size_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_filesz;
}

static inline size_t
elf64_getProgramHeaderMemorySize(elf_t *file, size_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_memsz;
}

static inline uint32_t
elf64_getProgramHeaderFlags(elf_t *file, size_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_flags;
}

static inline size_t
elf64_getProgramHeaderAlign(elf_t *file, size_t ph)
{
    return elf64_getProgramHeaderTable(file)[ph].p_align;
}
