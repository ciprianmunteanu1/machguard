#include "parser.h"

void set_raw_zone(struct raw_zone *zone, const char *name, size_t size, uint32_t file_offset) {
    zone->name = strdup(name);
    DIE(!zone->name, "[ERROR]: strdup() failed.");
    zone->size = size;
    zone->file_offset = file_offset;
}

void set_dylibs(struct mach_o_ctx *ctx, uint8_t *cmd_zone) {
    if (ctx->dylib_count == 0) {
        ctx->dylibs = malloc(sizeof(struct dylib_command));
        DIE(!ctx->dylibs, "[ERROR]: malloc() failed.");
    } else {
        ctx->dylibs = realloc(ctx->dylibs, (ctx->dylib_count + 1) * sizeof(struct dylib_command));
        DIE(!ctx->dylibs, "[ERROR]: realloc() failed.");
    }
    ctx->dylibs[ctx->dylib_count] = *(struct dylib_command *)cmd_zone;
    ctx->dylib_count++;
}

struct mach_o_ctx *parse_mach_file(char *file_path, uint8_t* base, size_t file_size) {
    struct mach_o_ctx *ctx = malloc(sizeof(struct mach_o_ctx));
    DIE(!ctx, "[ERROR]: malloc() failed.");

    ctx->file_path = strdup(file_path);
    DIE(!ctx->file_path, "[ERROR]: strdup() failed.");
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
        free(ctx->file_path);
        free(ctx);
        return NULL;
    }

    // Extract header and load commands zone
    uint8_t *cmd_zone;
    if (ctx->is_64bit) {
        ctx->nmcds = ((struct mach_header_64 *)base)->ncmds;
        cmd_zone = base + sizeof(struct mach_header_64);
    } else {
        ctx->nmcds = ((struct mach_header *)base)->ncmds;
        cmd_zone = base + sizeof(struct mach_header);
    }

    // Extract segments zone
    ctx->dylib_count = 0;
    for (uint32_t i = 0; i < ctx->nmcds; i++) {
        struct load_command *load_cmd = (struct load_command *)cmd_zone;
        switch (load_cmd->cmd) {
            case LC_SEGMENT: {
                struct segment_command *seg_cmd = (struct segment_command *)cmd_zone;
                uint32_t nsects = seg_cmd->nsects;

                struct section *sect = (struct section *)(seg_cmd + 1);
                for (uint32_t j = 0; j < nsects; j++) {
                    if (strcmp(sect[j].sectname, SECT_TEXT) == 0) {
                        set_raw_zone(&ctx->text, "text", sect[j].size, sect[j].offset);
                    } else if (strcmp(sect[j].sectname, SECT_CSTRING) == 0) {
                        set_raw_zone(&ctx->cstring, "cstring", sect[j].size, sect[j].offset);
                    } else if (strcmp(sect[j].sectname, SECT_STUBS) == 0) {
                        set_raw_zone(&ctx->stubs, "stubs", sect[j].size, sect[j].offset);
                    } else if (strcmp(sect[j].sectname, SECT_DATA) == 0) {
                        set_raw_zone(&ctx->data, "data", sect[j].size, sect[j].offset);
                    }
                }
                break;
            }
            case LC_SEGMENT_64: {
                struct segment_command_64 *seg_cmd = (struct segment_command_64 *)cmd_zone;
                uint32_t nsects = seg_cmd->nsects;

                struct section_64 *sect = (struct section_64 *)(seg_cmd + 1);
                for (uint32_t j = 0; j < nsects; j++) {
                    if (strcmp(sect[j].sectname, SECT_TEXT) == 0) {
                        set_raw_zone(&ctx->text, "text", sect[j].size, sect[j].offset);
                    } else if (strcmp(sect[j].sectname, SECT_CSTRING) == 0) {
                        set_raw_zone(&ctx->cstring, "cstring", sect[j].size, sect[j].offset);
                    } else if (strcmp(sect[j].sectname, SECT_STUBS) == 0) {
                        set_raw_zone(&ctx->stubs, "stubs", sect[j].size, sect[j].offset);
                    } else if (strcmp(sect[j].sectname, SECT_DATA) == 0) {
                        set_raw_zone(&ctx->data, "data", sect[j].size, sect[j].offset);
                    }
                }
                break;
            }
            case LC_SYMTAB: {
                ctx->symtab = *(struct symtab_command *)cmd_zone;
                break;
            }
            case LC_DYSYMTAB: {
                ctx->dysymtab = *(struct dysymtab_command *)cmd_zone;
                break;
            }
            case LC_MAIN: {
                ctx->entry_point = *(struct entry_point_command *)cmd_zone;
                break;
            }
            case LC_LOAD_DYLIB: {
                set_dylibs(ctx, cmd_zone);
                break;
            }
            case LC_ID_DYLIB: {
                set_dylibs(ctx, cmd_zone);
                break;
            }
            case LC_REEXPORT_DYLIB: {
                set_dylibs(ctx, cmd_zone);
                break;
            }
            default:
                break;
        }
        cmd_zone += load_cmd->cmdsize;
    }
    return ctx;
}
