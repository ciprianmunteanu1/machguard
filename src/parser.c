#include "parser.h"

void set_generic_section(struct generic_section *sect_out, void *in, bool is_64bit) {
    if (is_64bit) {
        struct section_64 *sect_in = (struct section_64 *)in;

        strcpy(sect_out->sectname, sect_in->sectname);
        strcpy(sect_out->segname, sect_in->segname);

        sect_out->addr = sect_in->addr;
        sect_out->size = sect_in->size;
        sect_out->offset = sect_in->offset;
        sect_out->align = sect_in->align;

        sect_out->reloff = sect_in->reloff;
        sect_out->nreloc = sect_in->nreloc;

        sect_out->flags = sect_in->flags;
    } else {
        struct section *sect_in = (struct section *)in;

        strcpy(sect_out->sectname, sect_in->sectname);
        strcpy(sect_out->segname, sect_in->segname);

        sect_out->addr = sect_in->addr;
        sect_out->size = sect_in->size;
        sect_out->offset = sect_in->offset;
        sect_out->align = sect_in->align;

        sect_out->reloff = sect_in->reloff;
        sect_out->nreloc = sect_in->nreloc;

        sect_out->flags = sect_in->flags;
    }
}

void set_dylibs(struct mach_o_ctx *ctx, uint8_t *cmd_zone) {
    if (ctx->dylib_count == 0) {
        ctx->dylibs = malloc(sizeof(struct dylib_command_wrapper));
        DIE(!ctx->dylibs, "[ERROR]: malloc() failed.");
    } else {
        ctx->dylibs = realloc(ctx->dylibs, (ctx->dylib_count + 1) * sizeof(struct dylib_command_wrapper));
        DIE(!ctx->dylibs, "[ERROR]: realloc() failed.");
    }
    ctx->dylibs[ctx->dylib_count].cmd_struct = cmd_zone;
    ctx->dylibs[ctx->dylib_count].dylib_cmd = *(struct dylib_command *)cmd_zone;
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
            case LC_SEGMENT: case LC_SEGMENT_64: {
                uint32_t nsects;
                uint8_t *sect_zone;

                if (ctx->is_64bit) {
                    nsects = ((struct segment_command_64 *)cmd_zone)->nsects;
                    sect_zone = cmd_zone + sizeof(struct segment_command_64);
                } else {
                    nsects = ((struct segment_command *)cmd_zone)->nsects;
                    sect_zone = cmd_zone + sizeof(struct segment_command);
                }

                for (uint32_t j = 0; j < nsects; j++) {
                    char sectname[17];
                    if (ctx->is_64bit) {
                        memcpy(sectname, ((struct section_64 *)sect_zone)->sectname, 16);
                    } else {
                        memcpy(sectname, ((struct section *)sect_zone)->sectname, 16);
                    }
                    sectname[16] = '\0';
                    
                    if (strcmp(sectname, SECT_TEXT) == 0) {
                        set_generic_section(&ctx->text, sect_zone, ctx->is_64bit);
                    } else if (strcmp(sectname, SECT_CSTRING) == 0) {
                        set_generic_section(&ctx->cstring, sect_zone, ctx->is_64bit);
                    } else if (strcmp(sectname, SECT_STUBS) == 0) {
                        set_generic_section(&ctx->stubs, sect_zone, ctx->is_64bit);
                    } else if (strcmp(sectname, SECT_DATA) == 0) {
                        set_generic_section(&ctx->data, sect_zone, ctx->is_64bit);
                    }

                    if (ctx->is_64bit) {
                        sect_zone += sizeof(struct section_64);
                    } else {
                        sect_zone += sizeof(struct section);
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
