#include "parser.h"
#include "header.h"
#include "load_commands.h"

int main(int argc, char **argv) {
    DIE(argc == 1, "[ERROR]: Invalid number of arguments.");

    // Open binary file
    int fdin = open(argv[1], O_RDONLY);
    DIE(fdin < 0, "[ERROR]: open() failed.");

    // Get file size
    struct stat statbuf;
    int ret = fstat(fdin, &statbuf);
    DIE(ret < 0, "[ERROR]: fstat() failed.");

    // mmap input file
    uint8_t *base = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fdin, 0);
    DIE(base == MAP_FAILED, "[ERROR]: mmap() failed.");

    struct mach_o_ctx *ctx = parse_mach_file(argv[1], base, statbuf.st_size);
    DIE(!ctx, "[ERROR]: Failed to parse Mach-O file.");
        
    display_mach_header(ctx);
    display_load_cmds_summary(ctx);
    choose_segment(ctx);

    close(fdin);
    return 0;
}
