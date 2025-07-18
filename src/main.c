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

typedef struct {
    uint8_t *data;
    size_t size;
    uint32_t file_offset;
} RawZone;

typedef struct {
    struct relocation_info *entries;
    uint32_t count;
    uint32_t file_offset;
} RelocZone;

typedef struct {
    const char *file_path;
    uint8_t *base;
    size_t file_size;

    bool is_64bit;
    void* header; // mach-o header

    uint32_t nmcds;
    uint8_t *cmds_zone; // pointer to load cmds zone

    RawZone text, stubs, cstring, data, symtab, dysymtab;
    RelocZone ext_relocs, int_relocs;
} MachOContext;

MachOContext *parse_mach_file(char *file_path, uint8_t* base, size_t file_size) {
    MachOContext *ctx = malloc(sizeof(MachOContext));
    DIE(!ctx, "[ERROR]: malloc() error.");

    ctx->file_path = strdup(file_path);
    DIE(!ctx->file_path, "[ERROR]: strdup() error.");
    ctx->base = base;
    ctx->file_size = file_size;

    // Identify file as Mach-O
    uint32_t magic_le = *(uint32_t *)base;
    uint32_t magic_be = __builtin_bswap32(magic_le); // convert to big endian

    // Check architecture
    if (magic_be == MH_MAGIC || magic_be == MH_CIGAM) {
        ctx->is_64bit = false;
    } else if (magic_be == MH_MAGIC_64 || magic_be == MH_CIGAM_64) {
        ctx->is_64bit = true;
    } else {
        free(ctx);
        return NULL;
    }

    // Extract header and load commands zone
    if (ctx->is_64bit) {
        ctx->header = (struct mach_header_64 *)base;
        ctx->nmcds = ((struct mach_header_64 *)ctx->header)->ncmds;
        ctx->cmds_zone = base + sizeof(struct mach_header_64);
    } else {
        ctx->header = (struct mach_header *)base;
        ctx->nmcds = ((struct mach_header *)ctx->header)->ncmds;
        ctx->cmds_zone = base + sizeof(struct mach_header);
    }

    // Extract segments zone
    uint8_t *cmd_zone_ptr = ctx->cmds_zone;
    for (uint32_t i = 0; i < ctx->nmcds; i++) {
        struct load_command *load_cmd = (struct load_command *)cmd_zone_ptr;
        switch (load_cmd->cmd) {
            case LC_SEGMENT: {
                printf("[*] LC_SEGMENT (32-bit)\n");

                struct segment_command *seg_cmd = (struct segment_command *)cmd_zone_ptr;
                uint32_t nsects = seg_cmd->nsects;

                struct section *sect = (struct section *)(seg_cmd + 1);

                for (uint32_t j = 0; j < nsects; j++) {
                    if (strcmp(sect[j].sectname, SECT_TEXT) == 0) {
                        ctx->text.data = base + sect[j].offset;
                        ctx->text.size = sect[j].size;
                        ctx->text.file_offset = sect[j].offset;
                    } else if (strcmp(sect[j].sectname, SECT_CSTRING) == 0) {
                        ctx->cstring.data = base + sect[j].offset;
                        ctx->cstring.size = sect[j].size;
                        ctx->cstring.file_offset = sect[j].offset;
                    } else if (strcmp(sect[j].sectname, SECT_STUBS) == 0) {
                        ctx->stubs.data = base + sect[j].offset;
                        ctx->stubs.size = sect[j].size;
                        ctx->stubs.file_offset = sect[j].offset;
                    } else if (strcmp(sect[j].sectname, SECT_DATA) == 0) {
                        ctx->data.data = base + sect[j].offset;
                        ctx->data.size = sect[j].size;
                        ctx->data.file_offset = sect[j].offset;
                    }
                }

                break;
            }
            case LC_SYMTAB:
                printf("[*] LC_SYMTAB\n");
                break;
            case LC_SYMSEG:
                printf("[*] LC_SYMSEG (obsolete)\n");
                break;
            case LC_THREAD:
                printf("[*] LC_THREAD\n");
                break;
            case LC_UNIXTHREAD:
                printf("[*] LC_UNIXTHREAD\n");
                break;
            case LC_LOADFVMLIB:
                printf("[*] LC_LOADFVMLIB\n");
                break;
            case LC_IDFVMLIB:
                printf("[*] LC_IDFVMLIB\n");
                break;
            case LC_IDENT:
                printf("[*] LC_IDENT (obsolete)\n");
                break;
            case LC_FVMFILE:
                printf("[*] LC_FVMFILE (internal use)\n");
                break;
            case LC_PREPAGE:
                printf("[*] LC_PREPAGE (internal use)\n");
                break;
            case LC_DYSYMTAB:
                printf("[*] LC_DYSYMTAB\n");
                break;
            case LC_LOAD_DYLIB:
                printf("[*] LC_LOAD_DYLIB\n");
                break;
            case LC_ID_DYLIB:
                printf("[*] LC_ID_DYLIB\n");
                break;
            case LC_LOAD_DYLINKER:
                printf("[*] LC_LOAD_DYLINKER\n");
                break;
            case LC_ID_DYLINKER:
                printf("[*] LC_ID_DYLINKER\n");
                break;
            case LC_PREBOUND_DYLIB:
                printf("[*] LC_PREBOUND_DYLIB\n");
                break;
            case LC_ROUTINES:
                printf("[*] LC_ROUTINES\n");
                break;
            case LC_SUB_FRAMEWORK:
                printf("[*] LC_SUB_FRAMEWORK\n");
                break;
            case LC_SUB_UMBRELLA:
                printf("[*] LC_SUB_UMBRELLA\n");
                break;
            case LC_SUB_CLIENT:
                printf("[*] LC_SUB_CLIENT\n");
                break;
            case LC_SUB_LIBRARY:
                printf("[*] LC_SUB_LIBRARY\n");
                break;
            case LC_TWOLEVEL_HINTS:
                printf("[*] LC_TWOLEVEL_HINTS\n");
                break;
            case LC_PREBIND_CKSUM:
                printf("[*] LC_PREBIND_CKSUM\n");
                break;
            case (LC_REQ_DYLD | 0x18): // LC_LOAD_WEAK_DYLIB
                printf("[*] LC_LOAD_WEAK_DYLIB\n");
                break;
            case LC_SEGMENT_64: {
                printf("[*] LC_SEGMENT_64\n");

                struct segment_command_64 *seg_cmd = (struct segment_command_64 *)cmd_zone_ptr;
                uint32_t nsects = seg_cmd->nsects;

                struct section_64 *sect = (struct section_64 *)(seg_cmd + 1);

                for (uint32_t j = 0; j < nsects; j++) {
                    if (strcmp(sect[j].sectname, SECT_TEXT) == 0) {
                        ctx->text.data = base + sect[j].offset;
                        ctx->text.size = sect[j].size;
                        ctx->text.file_offset = sect[j].offset;
                    } else if (strcmp(sect[j].sectname, SECT_CSTRING) == 0) {
                        ctx->cstring.data = base + sect[j].offset;
                        ctx->cstring.size = sect[j].size;
                        ctx->cstring.file_offset = sect[j].offset;
                    } else if (strcmp(sect[j].sectname, SECT_STUBS) == 0) {
                        ctx->stubs.data = base + sect[j].offset;
                        ctx->stubs.size = sect[j].size;
                        ctx->stubs.file_offset = sect[j].offset;
                    } else if (strcmp(sect[j].sectname, SECT_DATA) == 0) {
                        ctx->data.data = base + sect[j].offset;
                        ctx->data.size = sect[j].size;
                        ctx->data.file_offset = sect[j].offset;
                    }
                }

                break;
            }
            case LC_ROUTINES_64:
                printf("[*] LC_ROUTINES_64\n");
                break;
            case LC_UUID:
                printf("[*] LC_UUID\n");
                break;
            case (LC_REQ_DYLD | 0x1c): // LC_RPATH
                printf("[*] LC_RPATH\n");
                break;
            case LC_CODE_SIGNATURE:
                printf("[*] LC_CODE_SIGNATURE\n");
                break;
            case LC_SEGMENT_SPLIT_INFO:
                printf("[*] LC_SEGMENT_SPLIT_INFO\n");
                break;
            case (LC_REQ_DYLD | 0x1f): // LC_REEXPORT_DYLIB
                printf("[*] LC_REEXPORT_DYLIB\n");
                break;
            case LC_LAZY_LOAD_DYLIB:
                printf("[*] LC_LAZY_LOAD_DYLIB\n");
                break;
            case LC_ENCRYPTION_INFO:
                printf("[*] LC_ENCRYPTION_INFO\n");
                break;
            case LC_DYLD_INFO:
                printf("[*] LC_DYLD_INFO\n");
                break;
            case (LC_REQ_DYLD | 0x22): // LC_DYLD_INFO_ONLY
                printf("[*] LC_DYLD_INFO_ONLY\n");
                break;
            case (LC_REQ_DYLD | 0x23): // LC_LOAD_UPWARD_DYLIB
                printf("[*] LC_LOAD_UPWARD_DYLIB\n");
                break;
            case LC_VERSION_MIN_MACOSX:
                printf("[*] LC_VERSION_MIN_MACOSX\n");
                break;
            case LC_VERSION_MIN_IPHONEOS:
                printf("[*] LC_VERSION_MIN_IPHONEOS\n");
                break;
            case LC_FUNCTION_STARTS:
                printf("[*] LC_FUNCTION_STARTS\n");
                break;
            case LC_DYLD_ENVIRONMENT:
                printf("[*] LC_DYLD_ENVIRONMENT\n");
                break;
            case (LC_REQ_DYLD | 0x28): // LC_MAIN
                printf("[*] LC_MAIN\n");
                break;
            case LC_DATA_IN_CODE:
                printf("[*] LC_DATA_IN_CODE\n");
                break;
            case LC_SOURCE_VERSION:
                printf("[*] LC_SOURCE_VERSION\n");
                break;
            case LC_DYLIB_CODE_SIGN_DRS:
                printf("[*] LC_DYLIB_CODE_SIGN_DRS\n");
                break;
            case LC_ENCRYPTION_INFO_64:
                printf("[*] LC_ENCRYPTION_INFO_64\n");
                break;
            case LC_LINKER_OPTION:
                printf("[*] LC_LINKER_OPTION\n");
                break;
            case LC_LINKER_OPTIMIZATION_HINT:
                printf("[*] LC_LINKER_OPTIMIZATION_HINT\n");
                break;
            case LC_VERSION_MIN_TVOS:
                printf("[*] LC_VERSION_MIN_TVOS\n");
                break;
            case LC_VERSION_MIN_WATCHOS:
                printf("[*] LC_VERSION_MIN_WATCHOS\n");
                break;
            case LC_NOTE:
                printf("[*] LC_NOTE\n");
                break;
            case LC_BUILD_VERSION:
                printf("[*] LC_BUILD_VERSION\n");
                break;
            case (LC_REQ_DYLD | 0x33): // LC_DYLD_EXPORTS_TRIE
                printf("[*] LC_DYLD_EXPORTS_TRIE\n");
                break;
            case (LC_REQ_DYLD | 0x34): // LC_DYLD_CHAINED_FIXUPS
                printf("[*] LC_DYLD_CHAINED_FIXUPS\n");
                break;
            case (LC_REQ_DYLD | 0x35): // LC_FILESET_ENTRY
                printf("[*] LC_FILESET_ENTRY\n");
                break;
            case 0x36:
                printf("[*] LC_ATOM_INFO\n");
                break;
            case 0x37:
                printf("[*] LC_FUNCTION_VARIANTS\n");
                break;
            case 0x38:
                printf("[*] LC_FUNCTION_VARIANT_FIXUPS\n");
                break;
            case 0x39:
                printf("[*] LC_TARGET_TRIPLE\n");
                break;
            default:
                printf("[*] Unknown or unhandled load command: 0x%x\n", load_cmd->cmd);
                break;
        }


        cmd_zone_ptr += load_cmd->cmdsize;
    }

    return ctx;
    
}


int main(int argc, char **argv) {
    DIE(argc == 1, "[ERROR]: Invalid number of arguments.");

    // Open binary file
    int fdin = open(argv[1], O_RDONLY);
    DIE(fdin < 0, "[ERROR]: open() error.");

    // Get file size
    struct stat statbuf;
    int ret = fstat(fdin, &statbuf);
    DIE(ret < 0, "[ERROR]: fstat() error.");

    // mmap input file
    uint8_t *base = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fdin, 0);
    DIE(base == MAP_FAILED, "[ERROR]: mmap() error.");

    MachOContext *ctx = parse_mach_file(argv[1], base, statbuf.st_size);
    if (!ctx) {
        fprintf(stderr, "[ERROR]: Failed to parse Mach-O file.\n");
        return 1;
    }

    
    return 0;
}
