#include "header.h"

void match_magic_number(uint32_t magic) {
    switch (magic) {
        case 0xfeedface: case 0xfeedfacf: 
            printf("big-endian binary\n");
            break;
        case 0xcefaedfe: case 0xcffaedfe:
            printf("little-endian binary\n");
            break;
        default:
            printf("unknown endianess\n");
            break;
    }
}

void match_cpu_type(cpu_type_t cputype) {
    switch (cputype) {
        case 0x00000001: case 0x01000001:
            printf("VAX\n");
            break;
        case 0x00000002: case 0x01000002:
            printf("ROMP\n");
            break;
        case 0x00000004: case 0x01000004:
            printf("NS32032\n");
            break;
        case 0x00000005: case 0x01000005:
            printf("NS32332\n");
            break;
        case 0x00000006: case 0x01000006:
            printf("MC680x0\n");
            break;
        case 0x00000007: case 0x01000007:
            printf("x86\n");
            break;
        case 0x00000008: case 0x01000008:
            printf("MIPS\n");
            break;
        case 0x00000009: case 0x01000009:
            printf("NS32352\n");
            break;
        case 0x0000000B: case 0x0100000B:
            printf("HP-PA\n");
            break;
        case 0x0000000C: case 0x0100000C:
            printf("ARM\n");
            break;
        case 0x0000000D: case 0x0100000D:
            printf("MC88000\n");
            break;
        case 0x0000000E: case 0x0100000E:
            printf("SPARC\n");
            break;
        case 0x0000000F: case 0x0100000F:
            printf("i860 (big-endian)\n");
            break;
        case 0x00000010: case 0x01000010:
            printf("i860 (little-endian) or DEC Alpha\n");
            break;
        case 0x00000011: case 0x01000011:
            printf("RS/6000\n");
            break;
        case 0x00000012: case 0x01000012:
            printf("PowerPC / MC98000\n");
            break;
        default:
            printf("unknown CPU type\n");
            break;
    }
}

void match_arm_cpu_subtype(cpu_subtype_t subtype) {
    switch (subtype) {
        case 0x00000000:
            printf("all ARM processors\n");
            break;
        case 0x00000001:
            printf("optimized for ARM-A500 ARCH or newer\n");
            break;
        case 0x00000002:
            printf("optimized for ARM-A500 or newer\n");
            break;
        case 0x00000003:
            printf("pptimized for ARM-A440 or newer\n");
            break;
        case 0x00000004:
            printf("optimized for ARM-M4 or newer\n");
            break;
        case 0x00000005:
            printf("optimized for ARM-V4T or newer\n");
            break;
        case 0x00000006:
            printf("optimized for ARM-V6 or newer\n");
            break;
        case 0x00000007:
            printf("optimized for ARM-V5TEJ or newer\n");
            break;
        case 0x00000008:
            printf("optimized for ARM-XSCALE or newer\n");
            break;
        case 0x00000009:
            printf("optimized for ARM-V7 or newer\n");
            break;
        case 0x0000000A:
            printf("optimized for ARM-V7F (Cortex A9) or newer\n");
            break;
        case 0x0000000B:
            printf("optimized for ARM-V7S (Swift) or newer\n");
            break;
        case 0x0000000C:
            printf("optimized for ARM-V7K (Kirkwood40) or newer\n");
            break;
        case 0x0000000D:
            printf("optimized for ARM-V8 or newer\n");
            break;
        case 0x0000000E:
            printf("optimized for ARM-V6M or newer\n");
            break;
        case 0x0000000F:
            printf("optimized for ARM-V7M or newer\n");
            break;
        case 0x00000010:
            printf("optimized for ARM-V7EM or newer\n");
            break;
        default:
            printf("unknown ARM CPU subtype\n");
            break;
    }
}

void match_x86_cpu_subtype(cpu_subtype_t subtype) {
    switch (subtype) {
        case 0x00000003:
            printf("all x86 processors\n");
            break;
        case 0x00000004:
            printf("optimized for 486 or newer\n");
            break;
        case 0x00000084:
            printf("optimized for 486SX or newer\n");
            break;
        case 0x00000056:
            printf("optimized for pentium m5 or newer\n");
            break;
        case 0x00000067:
            printf("optimized for celeron or newer\n");
            break;
        case 0x00000077:
            printf("optimized for celeron mobile\n");
            break;
        case 0x00000008:
            printf("optimized for pentium 3 or newer\n");
            break;
        case 0x00000018:
            printf("optimized for pentium 3-m or newer\n");
            break;
        case 0x00000028:
            printf("optimized for pentium 3-xeon or newer\n");
            break;
        case 0x0000000A:
            printf("optimized for pentium 4 or newer\n");
            break;
        case 0x0000000B:
            printf("optimized for itanium or newer\n");
            break;
        case 0x0000001B:
            printf("optimized for itanium 2 or newer\n");
            break;
        case 0x0000000C:
            printf("optimized for xeon or newer\n");
            break;
        case 0x0000001C:
            printf("optimized for xeon-mp or newer\n");
            break;
        default:
            printf("unknown x86 cpu subtype\n");
            break;
    }
}

void match_file_type(uint32_t filetype) {
    switch (filetype) {
        case 0x00000001:
            printf("relocatable object file\n");
            break;
        case 0x00000002:
            printf("demand paged executable file\n");
            break;
        case 0x00000003:
            printf("fixed vm shared library file\n");
            break;
        case 0x00000004:
            printf("core file\n");
            break;
        case 0x00000005:
            printf("preloaded executable file\n");
            break;
        case 0x00000006:
            printf("dynamically bound shared library file\n");
            break;
        case 0x00000007:
            printf("dynamic link editor\n");
            break;
        case 0x00000008:
            printf("dynamically bound bundle file\n");
            break;
        case 0x00000009:
            printf("shared library stub for static linking only\n");
            break;
        case 0x0000000A:
            printf("debug companion file with only debug sections\n");
            break;
        case 0x0000000B:
            printf("x86_64 kexts file\n");
            break;
        case 0x0000000C:
            printf("composite mach-o for userspace with shared linkedit\n");
            break;
        default:
            printf("unknown file type\n");
            break;
    }
}

void match_flags(uint32_t flags) {
    if (flags & MH_NOUNDEFS)               printf("\t\t\t- no undefined references (MH_NOUNDEFS)\n");
    if (flags & MH_INCRLINK)               printf("\t\t\t- incremental link output (MH_INCRLINK)\n");
    if (flags & MH_DYLDLINK)               printf("\t\t\t- input for dynamic linker (MH_DYLDLINK)\n");
    if (flags & MH_BINDATLOAD)             printf("\t\t\t- bind at load (MH_BINDATLOAD)\n");
    if (flags & MH_PREBOUND)               printf("\t\t\t- prebound file (MH_PREBOUND)\n");
    if (flags & MH_SPLIT_SEGS)             printf("\t\t\t- split read-only and read-write segments (MH_SPLIT_SEGS)\n");
    if (flags & MH_LAZY_INIT)              printf("\t\t\t- lazy shared library init (obsolete) (MH_LAZY_INIT)\n");
    if (flags & MH_TWOLEVEL)               printf("\t\t\t- two-level namespace bindings (MH_TWOLEVEL)\n");
    if (flags & MH_FORCE_FLAT)             printf("\t\t\t- force flat namespace bindings (MH_FORCE_FLAT)\n");
    if (flags & MH_NOMULTIDEFS)            printf("\t\t\t- no multiple definitions allowed in sub-images (MH_NOMULTIDEFS)\n");
    if (flags & MH_NOFIXPREBINDING)        printf("\t\t\t- no prebinding agent notification (MH_NOFIXPREBINDING)\n");
    if (flags & MH_PREBINDABLE)            printf("\t\t\t- prebinding can be redone (MH_PREBINDABLE)\n");
    if (flags & MH_ALLMODSBOUND)           printf("\t\t\t- binds to all modules of dependent libraries (MH_ALLMODSBOUND)\n");
    if (flags & MH_SUBSECTIONS_VIA_SYMBOLS)printf("\t\t\t- subsections via symbols enabled (MH_SUBSECTIONS_VIA_SYMBOLS)\n");
    if (flags & MH_CANONICAL)              printf("\t\t\t- canonicalized binary (MH_CANONICAL)\n");
    if (flags & MH_WEAK_DEFINES)           printf("\t\t\t- contains external weak symbols (MH_WEAK_DEFINES)\n");
    if (flags & MH_BINDS_TO_WEAK)          printf("\t\t\t- binds to weak symbols (MH_BINDS_TO_WEAK)\n");
    if (flags & MH_ALLOW_STACK_EXECUTION)  printf("\t\t\t- stack execution allowed (MH_ALLOW_STACK_EXECUTION)\n");
    if (flags & MH_ROOT_SAFE)              printf("\t\t\t- safe for root processes (MH_ROOT_SAFE)\n");
    if (flags & MH_SETUID_SAFE)            printf("\t\t\t- safe for setuid/setgid processes (MH_SETUID_SAFE)\n");
    if (flags & MH_NO_REEXPORTED_DYLIBS)   printf("\t\t\t- no re-exported dylibs (MH_NO_REEXPORTED_DYLIBS)\n");
    if (flags & MH_PIE)                    printf("\t\t\t- position independent executable (PIE) (MH_PIE)\n");
    if (flags & MH_DEAD_STRIPPABLE_DYLIB)  printf("\t\t\t- dead-strippable dylib (MH_DEAD_STRIPPABLE_DYLIB)\n");
    if (flags & MH_HAS_TLV_DESCRIPTORS)    printf("\t\t\t- has thread-local variable descriptors (MH_HAS_TLV_DESCRIPTORS)\n");
    if (flags & MH_NO_HEAP_EXECUTION)      printf("\t\t\t- no heap execution (MH_NO_HEAP_EXECUTION)\n");
    if (flags & MH_APP_EXTENSION_SAFE)     printf("\t\t\t- safe for app extensions (MH_APP_EXTENSION_SAFE)\n");
    if (flags & MH_NLIST_OUTOFSYNC_WITH_DYLDINFO)
                                           printf("\t\t\t- nlist out of sync with dyld info (MH_NLIST_OUTOFSYNC_WITH_DYLDINFO)\n");
    if (flags & MH_SIM_SUPPORT)            printf("\t\t\t- simulator platform support (MH_SIM_SUPPORT)\n");
    if (flags & MH_DYLIB_IN_CACHE)         printf("\t\t\t- dylib in dyld shared cache (MH_DYLIB_IN_CACHE)\n");

    if (flags == 0)
        printf("\t\t\t- no flags set\n");
}

void print_mach_header(struct mach_o_ctx *ctx) {
    printf("[*] Mach-O Header:\n");

    if (ctx->is_64bit) {
        struct mach_header_64 *hdr = (struct mach_header_64 *)ctx->base;

        printf("\t\tArchitecture: 64-bit\n");

        uint32_t magic = __builtin_bswap32(hdr->magic);
        printf("\t\tMagic Number: 0x%x - ", magic);
        match_magic_number(magic);

        cpu_type_t cputype = hdr->cputype;
        printf("\t\tCPU Type: 0x%x - ", cputype);
        match_cpu_type(cputype);

        cpu_subtype_t cpusubtype = hdr->cpusubtype;
        printf("\t\tCPU Subtype: 0x%x - ", cpusubtype);
        if (cputype == 0x0000000C || cputype == 0x0100000C) {
            match_arm_cpu_subtype(cpusubtype);
        } else if (cputype == 0x00000007 || cputype == 0x01000007) {
            match_x86_cpu_subtype(cpusubtype);
        }

        uint32_t filetype = hdr->filetype;
        printf("\t\tFile Type: 0x%x - ", filetype);
        match_file_type(filetype);

        printf("\t\tNumber of Load Commands: %d\n", hdr->ncmds);
        printf("\t\tSize of all the Load Commands: %d\n", hdr->sizeofcmds);

        uint32_t flags = hdr->flags;
        printf("\t\tFlags:\n");
        match_flags(flags);
    } else {
        struct mach_header *hdr = (struct mach_header *)ctx->base;

        printf("\t\tArchitecture: 32-bit\n");

        uint32_t magic = __builtin_bswap32(hdr->magic);
        printf("\t\tMagic Number: 0x%x - ", magic);
        match_magic_number(magic);

        cpu_type_t cputype = hdr->cputype;
        printf("\t\tCPU Type: 0x%x - ", cputype);
        match_cpu_type(cputype);

        cpu_subtype_t cpusubtype = hdr->cpusubtype;
        printf("\t\tCPU Subtype: 0x%x - ", cpusubtype);
        if (cputype == 0x0000000C || cputype == 0x0100000C) {
            match_arm_cpu_subtype(cpusubtype);
        } else if (cputype == 0x00000007 || cputype == 0x01000007) {
            match_x86_cpu_subtype(cpusubtype);
        }

        uint32_t filetype = hdr->filetype;
        printf("\t\tFile Type: 0x%x - ", filetype);
        match_file_type(filetype);

        printf("\t\tNumber of Load Commands: %d\n", hdr->ncmds);
        printf("\t\tSize of all the Load Commands: %d\n", hdr->sizeofcmds);

        uint32_t flags = hdr->flags;
        printf("\t\tFlags:\n");
        match_flags(flags);
    }
}
