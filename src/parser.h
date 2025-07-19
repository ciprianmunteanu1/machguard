#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach-o/reloc.h>
#include "../util/utils.h"

#define MAX_PATH_LEN 1024
#define SECT_CSTRING "__cstring"
#define SECT_STUBS "__stubs"

struct generic_section {
    char		sectname[16];
	char		segname[16];

	uint64_t	addr;
	uint64_t	size;
	uint32_t	offset;
	uint32_t	align;

	uint32_t	reloff;
	uint32_t	nreloc;

	uint32_t	flags;
};

struct mach_o_ctx {
    char *file_path;
    uint8_t *base;
    size_t file_size;

    bool is_64bit;
    uint32_t nmcds;

    struct generic_section text;
    struct generic_section stubs;
    struct generic_section cstring;
    struct generic_section data;
    
    struct symtab_command symtab;
    struct dysymtab_command dysymtab;

    struct entry_point_command entry_point;

    size_t dylib_count;
    struct dylib_command *dylibs;
};

void set_generic_section(struct generic_section *out, void *in, bool is_64bit);
void set_dylibs(struct mach_o_ctx *ctx, uint8_t *cmd_zone);
struct mach_o_ctx *parse_mach_file(char *file_path, uint8_t* base, size_t file_size);
