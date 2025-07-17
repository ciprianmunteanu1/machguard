#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include "../util/utils.h"

#define MAX_PATH_LEN 1024

typedef struct mach_ctx {
    char *file_path;

    uint32_t magic;
    bool is_64bit;
    void* header; // mach-o header

    uint32_t nmcds;
    struct load_command *cmds_zone; // array of load cmds structs

    uint32_t nsects;
    void *segment_zone; // pointer to segments zone
} MachOContext;

MachOContext *parse_mach_file(FILE *file) {
    MachOContext *ctx = (MachOContext *)malloc(sizeof(MachOContext));
    DIE(!ctx, "[Error]: malloc() error.");

    // Identify file as Mach-O
    uint32_t magic_le = 0;

    int ret = fread(&magic_le, sizeof(uint32_t), 1, file);
    DIE(ret < 1, "[Error] fread() error.");

    uint32_t magic_be = __builtin_bswap32(magic_le); // convert to big endian
    ctx->magic = magic_be;

    // Check architecture
    if (magic_be == MH_MAGIC || magic_be == MH_CIGAM) {
        ctx->is_64bit = false;
    } else if (magic_be == MH_MAGIC_64 || magic_be == MH_CIGAM_64) {
        ctx->is_64bit = true;
    } else {
        free(ctx);
        return NULL;
    }

    if (ctx->is_64bit) {
        // Extract header
        ctx->header = malloc(sizeof(struct mach_header_64));
        DIE(!ctx->header, "[Error]: malloc() error.");

        ret = fread(ctx->header, sizeof(struct mach_header_64), 1, file);
        DIE(ret < 1, "[Error] fread() error.");

        // Extract load commands zone
        ctx->nmcds = ((struct mach_header_64 *)ctx->header)->ncmds;
        ctx->cmds_zone = malloc(ctx->nmcds * sizeof(struct load_command));
        DIE(!ctx->cmds_zone, "[Error]: malloc() error.");

        ret = fread(ctx->cmds_zone, ((struct mach_header_64 *)ctx->header)->sizeofcmds, 1, file);
        DIE(ret < 1, "[Error] fread() error.");

        // Extract segments zone
        struct load_command *cmd_zone_ptr = ctx->cmds_zone;
        for (int i = 0; i < ctx->nmcds; i++) {
            switch (cmd_zone_ptr->cmd) {
                case LC_SEGMENT:
                    ret = fread(cmd_zone_ptr, sizeof(struct segment_command), 1, file);
                    DIE(ret < 1, "[Error] fread() error.");

                    cmd_zone_ptr += sizeof(struct segment_command_64);
                    break;

                case LC_SEGMENT_64:
                    ret = fread(cmd_zone_ptr, sizeof(struct segment_command_64), 1, file);
                    DIE(ret < 1, "[Error] fread() error.");

                    
                    cmd_zone_ptr += sizeof(struct segment_command_64);
                    break;
            }

            
        }
    }




    switch (magic_be) {
        case MH_MAGIC:
            struct mac_header *hdr = parse_mach_32be(file);
            break;

        case MH_CIGAM:
            printf("Little endian, 32-bit Mach-O binary file.\n");
            break;
        case MH_MAGIC_64:
            printf("Big endian, 64-bit Mach-O binary file.\n");
            break;
        case MH_CIGAM_64:
            printf("Little endian, 64-bit Mach-O binary file.\n");
            break;
        case FAT_MAGIC:
            printf("Big endian, 32-bit FAT binary file.\n");
            break;
        case FAT_CIGAM:
            printf("Little endian, 32-bit FAT binary file.\n");
            break;
        case FAT_MAGIC_64:
            printf("Big endian, 64-bit FAT binary file.\n");
            break;
        case FAT_CIGAM_64:
            printf("Little endian, 64-bit FAT binary file.\n");
            break;
        default:
            printf("Invalid file format.\n");
    }
    
}


int main(int argc, char **argv) {
    // Get file path
    printf("Enter binary file path: ");
    char *file_path = malloc(MAX_PATH_LEN);
    DIE(!file_path, "[Error]: malloc() error.");
    scanf("%s", file_path);

    // Open binary file
    FILE *file = fopen(file_path, "rb");
    DIE(!file, "[Error]: fopen() error.");

    

    
    return 0;
}