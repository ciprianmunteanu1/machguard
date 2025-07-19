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

struct raw_zone {
    char *name;
    size_t size;
    uint32_t file_offset;
};

struct mach_o_ctx {
    char *file_path;
    uint8_t *base;
    size_t file_size;

    bool is_64bit;
    uint32_t nmcds;

    struct raw_zone text;
    struct raw_zone stubs;
    struct raw_zone cstring;
    struct raw_zone data;
    
    struct symtab_command symtab;
    struct dysymtab_command dysymtab;

    struct entry_point_command entry_point;

    size_t dylib_count;
    struct dylib_command *dylibs;
};

void set_raw_zone(struct raw_zone *zone, const char *name, size_t size, uint32_t file_offset);
void set_dylibs(struct mach_o_ctx *ctx, uint8_t *cmd_zone);
struct mach_o_ctx *parse_mach_file(char *file_path, uint8_t* base, size_t file_size);
