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
#include <elf.h>
#include <elf32.h>
#include <elf64.h>
#include <string.h>
#include <stdio.h>

/* ELF header functions */
int
elf_newFile(void *file, size_t size, elf_t *res)
{
    return elf_newFile_maybe_unsafe(file, size, true, true, res);
}

int
elf_newFile_maybe_unsafe(void *file, size_t size, bool check_pht, bool check_st, elf_t *res)
{
    elf_t new_file = {
        .elfFile = file,
        .elfSize = size
    };

    int status = elf_checkFile(&new_file);
    if (status < 0) {
        return status;
    }

    if (check_pht) {
        status = elf_checkProgramHeaderTable(&new_file);
        if (status < 0) {
            return status;
        }
    }

    if (check_st) {
        status = elf_checkSectionTable(&new_file);
        if (status < 0) {
            return status;
        }
    }

    if (res) {
        *res = new_file;
    }

    return status;
}

int
elf_check_magic(char *file)
{
    if (memcmp(file, ELFMAG, SELFMAG) != 0) {
        return -1;
    }

    return 0;
}

/*
 * Checks that elfFile points to a valid elf file. Returns 0 if the elf
 * file is valid, < 0 if invalid.
 */
int
elf_checkFile(elf_t *elfFile)
{
    int res = elf32_checkFile(elfFile);
    if (res == 0) {
        return 0;
    }

    res = elf64_checkFile(elfFile);
    if (res == 0) {
        return 0;
    }

    return -1;
}

int
elf_checkProgramHeaderTable(elf_t *elfFile)
{
    if (elf_isElf32(elfFile)) {
        return elf32_checkProgramHeaderTable(elfFile);
    } else {
        return elf64_checkProgramHeaderTable(elfFile);
    }
}

int
elf_checkSectionTable(elf_t *elfFile)
{
    if (elf_isElf32(elfFile)) {
        return elf32_checkSectionTable(elfFile);
    } else {
        return elf64_checkSectionTable(elfFile);
    }
}

uintptr_t
elf_getEntryPoint(elf_t *elfFile)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getEntryPoint(elfFile);
    } else {
        return elf64_getEntryPoint(elfFile);
    }
}

size_t
elf_getNumProgramHeaders(elf_t *elfFile)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getNumProgramHeaders(elfFile);
    } else {
        return elf64_getNumProgramHeaders(elfFile);
    }
}

size_t
elf_getNumSections(elf_t *elfFile)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getNumSections(elfFile);
    } else {
        return elf64_getNumSections(elfFile);
    }
}

size_t
elf_getSectionStringTableIndex(elf_t *elf)
{
    if (elf_isElf32(elf)) {
        return elf32_getSectionStringTableIndex(elf);
    } else {
        return elf64_getSectionStringTableIndex(elf);
    }
}

const char *
elf_getStringTable(elf_t *elf, size_t string_segment)
{
    const char *string_table = (const char*) elf_getSection(elf, string_segment);
    if (string_table == NULL) {
        return NULL; /* no such section */
    }

    if (elf_getSectionType(elf, string_segment) != SHT_STRTAB) {
        return NULL; /* not a string table */
    }

    size_t size = elf_getSectionSize(elf, string_segment);
    if (string_table[size - 1] != 0) {
        return NULL; /* string table is not null-terminated */
    }

    return string_table;
}

const char *
elf_getSectionStringTable(elf_t *elf)
{
    size_t index = elf_getSectionStringTableIndex(elf);
    return elf_getStringTable(elf, index);
}


/* Section header functions */
void *
elf_getSection(elf_t *elf, size_t i)
{
    if (i == 0 || i >= elf_getNumSections(elf)) {
        return NULL; /* no such section */
    }

    size_t section_offset = elf_getSectionOffset(elf, i);
    size_t section_size = elf_getSectionSize(elf, i);
    if (section_size == 0) {
        return NULL; /* section is empty */
    }

    size_t section_end = section_offset + section_size;
    /* possible wraparound - check that section end is not before section start */
    if (section_end > elf->elfSize || section_end < section_offset) {
        return NULL;
    }

    return (uint8_t*) elf->elfFile + section_offset;
}

void *
elf_getSectionNamed(elf_t *elfFile, const char *str, size_t *id)
{
    size_t numSections = elf_getNumSections(elfFile);
    for (size_t i = 0; i < numSections; i++) {
        if (strcmp(str, elf_getSectionName(elfFile, i)) == 0) {
            if (id != NULL) {
                *id = i;
            }
            return elf_getSection(elfFile, i);
        }
    }
    return NULL;
}

const char *
elf_getSectionName(elf_t *elf, size_t i)
{
    size_t str_table_idx = elf_getSectionStringTableIndex(elf);
    const char *str_table = elf_getStringTable(elf, str_table_idx);
    size_t offset = elf_getSectionNameOffset(elf, i);
    size_t size = elf_getSectionSize(elf, str_table_idx);

    if (str_table == NULL || offset > size) {
        return "<corrupted>";
    }

    return str_table + offset;
}

size_t elf_getSectionNameOffset(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionNameOffset(elfFile, i);
    } else {
        return elf64_getSectionNameOffset(elfFile, i);
    }
}

uint32_t
elf_getSectionType(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionType(elfFile, i);
    } else {
        return elf64_getSectionType(elfFile, i);
    }
}

size_t
elf_getSectionFlags(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionFlags(elfFile, i);
    } else {
        return elf64_getSectionFlags(elfFile, i);
    }
}

uintptr_t
elf_getSectionAddr(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionAddr(elfFile, i);
    } else {
        return elf64_getSectionAddr(elfFile, i);
    }
}

size_t
elf_getSectionOffset(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionOffset(elfFile, i);
    } else {
        return elf64_getSectionOffset(elfFile, i);
    }
}

size_t
elf_getSectionSize(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionSize(elfFile, i);
    } else {
        return elf64_getSectionSize(elfFile, i);
    }
}

uint32_t
elf_getSectionLink(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionLink(elfFile, i);
    } else {
        return elf64_getSectionLink(elfFile, i);
    }
}

uint32_t
elf_getSectionInfo(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionInfo(elfFile, i);
    } else {
        return elf64_getSectionInfo(elfFile, i);
    }
}

size_t
elf_getSectionAddrAlign(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionAddrAlign(elfFile, i);
    } else {
        return elf64_getSectionAddrAlign(elfFile, i);
    }
}

size_t
elf_getSectionEntrySize(elf_t *elfFile, size_t i)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getSectionEntrySize(elfFile, i);
    } else {
        return elf64_getSectionEntrySize(elfFile, i);
    }
}


/* Program headers function */
void *
elf_getProgramSegment(elf_t *elf, size_t ph)
{
    size_t offset = elf_getProgramHeaderOffset(elf, ph);
    size_t file_size = elf_getProgramHeaderFileSize(elf, ph);
    size_t segment_end = offset + file_size;
    /* possible wraparound - check that segment end is not before segment start */
    if (elf->elfSize < segment_end || segment_end < offset) {
        return NULL;
    }

    return (uint8_t*) elf->elfFile + offset;
}

uint32_t
elf_getProgramHeaderType(elf_t *elfFile, size_t ph)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getProgramHeaderType(elfFile, ph);
    } else {
        return elf64_getProgramHeaderType(elfFile, ph);
    }
}

size_t
elf_getProgramHeaderOffset(elf_t *elfFile, size_t ph)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getProgramHeaderOffset(elfFile, ph);
    } else {
        return elf64_getProgramHeaderOffset(elfFile, ph);
    }
}

uintptr_t
elf_getProgramHeaderVaddr(elf_t *elfFile, size_t ph)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getProgramHeaderVaddr(elfFile, ph);
    } else {
        return elf64_getProgramHeaderVaddr(elfFile, ph);
    }
}

uintptr_t
elf_getProgramHeaderPaddr(elf_t *elfFile, size_t ph)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getProgramHeaderPaddr(elfFile, ph);
    } else {
        return elf64_getProgramHeaderPaddr(elfFile, ph);
    }
}

size_t
elf_getProgramHeaderFileSize(elf_t *elfFile, size_t ph)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getProgramHeaderFileSize(elfFile, ph);
    } else {
        return elf64_getProgramHeaderFileSize(elfFile, ph);
    }
}

size_t
elf_getProgramHeaderMemorySize(elf_t *elfFile, size_t ph)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getProgramHeaderMemorySize(elfFile, ph);
    } else {
        return elf64_getProgramHeaderMemorySize(elfFile, ph);
    }
}

uint32_t
elf_getProgramHeaderFlags(elf_t *elfFile, size_t ph)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getProgramHeaderFlags(elfFile, ph);
    } else {
        return elf64_getProgramHeaderFlags(elfFile, ph);
    }
}

size_t
elf_getProgramHeaderAlign(elf_t *elfFile, size_t ph)
{
    if (elf_isElf32(elfFile)) {
        return elf32_getProgramHeaderAlign(elfFile, ph);
    } else {
        return elf64_getProgramHeaderAlign(elfFile, ph);
    }
}


/* Utility functions */
int
elf_getMemoryBounds(elf_t *elfFile, elf_addr_type_t addr_type, uintptr_t *min, uintptr_t *max)
{
    uintptr_t mem_min = UINTPTR_MAX;
    uintptr_t mem_max = 0;
    size_t i;

    for(i = 0; i < elf_getNumProgramHeaders(elfFile); i++) {
        uintptr_t sect_min, sect_max;

        if (elf_getProgramHeaderMemorySize(elfFile, i) == 0) {
            continue;
        }

        if (addr_type == PHYSICAL) {
            sect_min = elf_getProgramHeaderPaddr(elfFile, i);
        } else {
            sect_min = elf_getProgramHeaderVaddr(elfFile, i);
        }

        sect_max = sect_min + elf_getProgramHeaderMemorySize(elfFile, i);

        if (sect_max > mem_max) {
            mem_max = sect_max;
        }
        if (sect_min < mem_min) {
            mem_min = sect_min;
        }
    }
    *min = mem_min;
    *max = mem_max;

    return 1;
}

int
elf_vaddrInProgramHeader(elf_t *elfFile, size_t ph, uintptr_t vaddr)
{
    uintptr_t min = elf_getProgramHeaderVaddr(elfFile, ph);
    uintptr_t max = min + elf_getProgramHeaderMemorySize(elfFile, ph);
    if (vaddr >= min && vaddr < max) {
        return 1;
    } else {
        return 0;
    }
}

uintptr_t
elf_vtopProgramHeader(elf_t *elfFile, size_t ph, uintptr_t vaddr)
{
    uintptr_t ph_phys = elf_getProgramHeaderPaddr(elfFile, ph);
    uintptr_t ph_virt = elf_getProgramHeaderVaddr(elfFile, ph);
    uintptr_t paddr;

    paddr = vaddr - ph_virt + ph_phys;

    return paddr;
}

int
elf_loadFile(elf_t *elf, elf_addr_type_t addr_type)
{
    size_t i;

    for(i = 0; i < elf_getNumProgramHeaders(elf); i++) {
        /* Load that section */
        uintptr_t dest, src;
        size_t len;
        if (addr_type == PHYSICAL) {
            dest = elf_getProgramHeaderPaddr(elf, i);
        } else {
            dest = elf_getProgramHeaderVaddr(elf, i);
        }
        len = elf_getProgramHeaderFileSize(elf, i);
        src = (uintptr_t) elf->elfFile + elf_getProgramHeaderOffset(elf, i);
        memcpy((void *) dest, (void *) src, len);
        dest += len;
        memset((void *) dest, 0, elf_getProgramHeaderMemorySize(elf, i) - len);
    }

    return 1;
}
