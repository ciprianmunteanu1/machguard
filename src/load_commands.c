#include "load_commands.h"

void match_cmd_name(uint32_t cmd_name) {
    switch (cmd_name) {
        case LC_SEGMENT:                 printf("LC_SEGMENT\n"); break;
        case LC_SYMTAB:                  printf("LC_SYMTAB\n"); break;
        case LC_SYMSEG:                  printf("LC_SYMSEG (obsolete)\n"); break;
        case LC_THREAD:                  printf("LC_THREAD\n"); break;
        case LC_UNIXTHREAD:              printf("LC_UNIXTHREAD\n"); break;
        case LC_LOADFVMLIB:              printf("LC_LOADFVMLIB\n"); break;
        case LC_IDFVMLIB:                printf("LC_IDFVMLIB\n"); break;
        case LC_IDENT:                   printf("LC_IDENT (obsolete)\n"); break;
        case LC_FVMFILE:                 printf("LC_FVMFILE (internal use)\n"); break;
        case LC_PREPAGE:                 printf("LC_PREPAGE (internal use)\n"); break;
        case LC_DYSYMTAB:                printf("LC_DYSYMTAB\n"); break;
        case LC_LOAD_DYLIB:              printf("LC_LOAD_DYLIB\n"); break;
        case LC_ID_DYLIB:                printf("LC_ID_DYLIB\n"); break;
        case LC_LOAD_DYLINKER:           printf("LC_LOAD_DYLINKER\n"); break;
        case LC_ID_DYLINKER:             printf("LC_ID_DYLINKER\n"); break;
        case LC_PREBOUND_DYLIB:          printf("LC_PREBOUND_DYLIB\n"); break;
        case LC_ROUTINES:                printf("LC_ROUTINES\n"); break;
        case LC_SUB_FRAMEWORK:           printf("LC_SUB_FRAMEWORK\n"); break;
        case LC_SUB_UMBRELLA:            printf("LC_SUB_UMBRELLA\n"); break;
        case LC_SUB_CLIENT:              printf("LC_SUB_CLIENT\n"); break;
        case LC_SUB_LIBRARY:             printf("LC_SUB_LIBRARY\n"); break;
        case LC_TWOLEVEL_HINTS:          printf("LC_TWOLEVEL_HINTS\n"); break;
        case LC_PREBIND_CKSUM:           printf("LC_PREBIND_CKSUM\n"); break;
        case LC_LOAD_WEAK_DYLIB:         printf("LC_LOAD_WEAK_DYLIB\n"); break;
        case LC_SEGMENT_64:              printf("LC_SEGMENT_64\n"); break;
        case LC_ROUTINES_64:             printf("LC_ROUTINES_64\n"); break;
        case LC_UUID:                    printf("LC_UUID\n"); break;
        case LC_RPATH:                   printf("LC_RPATH\n"); break;
        case LC_CODE_SIGNATURE:          printf("LC_CODE_SIGNATURE\n"); break;
        case LC_SEGMENT_SPLIT_INFO:      printf("LC_SEGMENT_SPLIT_INFO\n"); break;
        case LC_REEXPORT_DYLIB:          printf("LC_REEXPORT_DYLIB\n"); break;
        case LC_LAZY_LOAD_DYLIB:         printf("LC_LAZY_LOAD_DYLIB\n"); break;
        case LC_ENCRYPTION_INFO:         printf("LC_ENCRYPTION_INFO\n"); break;
        case LC_DYLD_INFO:               printf("LC_DYLD_INFO\n"); break;
        case LC_DYLD_INFO_ONLY:          printf("LC_DYLD_INFO_ONLY\n"); break;
        case LC_LOAD_UPWARD_DYLIB:       printf("LC_LOAD_UPWARD_DYLIB\n"); break;
        case LC_VERSION_MIN_MACOSX:      printf("LC_VERSION_MIN_MACOSX\n"); break;
        case LC_VERSION_MIN_IPHONEOS:    printf("LC_VERSION_MIN_IPHONEOS\n"); break;
        case LC_FUNCTION_STARTS:         printf("LC_FUNCTION_STARTS\n"); break;
        case LC_DYLD_ENVIRONMENT:        printf("LC_DYLD_ENVIRONMENT\n"); break;
        case LC_MAIN:                    printf("LC_MAIN\n"); break;
        case LC_DATA_IN_CODE:            printf("LC_DATA_IN_CODE\n"); break;
        case LC_SOURCE_VERSION:          printf("LC_SOURCE_VERSION\n"); break;
        case LC_DYLIB_CODE_SIGN_DRS:     printf("LC_DYLIB_CODE_SIGN_DRS\n"); break;
        case LC_ENCRYPTION_INFO_64:      printf("LC_ENCRYPTION_INFO_64\n"); break;
        case LC_LINKER_OPTION:           printf("LC_LINKER_OPTION\n"); break;
        case LC_LINKER_OPTIMIZATION_HINT:printf("LC_LINKER_OPTIMIZATION_HINT\n"); break;
        case LC_VERSION_MIN_TVOS:        printf("LC_VERSION_MIN_TVOS\n"); break;
        case LC_VERSION_MIN_WATCHOS:     printf("LC_VERSION_MIN_WATCHOS\n"); break;
        case LC_NOTE:                    printf("LC_NOTE\n"); break;
        case LC_BUILD_VERSION:           printf("LC_BUILD_VERSION\n"); break;
        case LC_DYLD_EXPORTS_TRIE:       printf("LC_DYLD_EXPORTS_TRIE\n"); break;
        case LC_DYLD_CHAINED_FIXUPS:     printf("LC_DYLD_CHAINED_FIXUPS\n"); break;
        case LC_FILESET_ENTRY:           printf("LC_FILESET_ENTRY\n"); break;

        default:
            printf("Unknown (0x%08x)\n", cmd_name);
            break;
    }
}

void display_load_cmds_summary(struct mach_o_ctx *ctx) {
    printf("[*] Load Commands Summary:\n\n");

    uint8_t *load_cmds_zone;
    if (ctx->is_64bit) {
        load_cmds_zone = ctx->base + sizeof(struct mach_header_64);
    } else {
        load_cmds_zone = ctx->base + sizeof(struct mach_header);
    }

    for (uint32_t i = 0; i < ctx->nmcds; i++) {
        struct load_command *load_cmd = (struct load_command *)load_cmds_zone;

        uint32_t cmd_name = load_cmd->cmd;
        uint32_t cmd_size = load_cmd->cmdsize;

        printf("\t-> Load Command number %d\n", i + 1);
        printf("\t\t- Load Command name: ");
        match_cmd_name(cmd_name);
        printf("\t\t- Total size of command in bytes: %d\n", cmd_size);

        load_cmds_zone += cmd_size;
    }
    printf("\n\n");
}

void match_vm_prot(vm_prot_t prot) {
    if (prot == VM_PROT_NONE) {
        printf("\t\t- none (no access)\n");
        return;
    }

    if (prot & VM_PROT_READ)    printf("\t\t- READ\n");
    if (prot & VM_PROT_WRITE)   printf("\t\t- WRITE\n");
    if (prot & VM_PROT_EXECUTE) printf("\t\t- EXECUTE\n");
}

void match_seg_flags(uint32_t flags) {
    if (flags & SG_HIGHVM)              printf("\t\t- high VM (SG_HIGHVM)\n");
    if (flags & SG_FVMLIB)              printf("\t\t- fixed VM library (SG_FVMLIB)\n");
    if (flags & SG_NORELOC)             printf("\t\t- no relocations (SG_NORELOC)\n");
    if (flags & SG_PROTECTED_VERSION_1) printf("\t\t- protected segment (SG_PROTECTED_VERSION_1)\n");
    if (flags & SG_READ_ONLY)           printf("\t\t- read-only after fixups (SG_READ_ONLY)\n");

    if (flags == 0)
        printf("\t\t- no flags set\n");
}

void match_sect_flags(uint32_t flags) {
	uint32_t type = flags & SECTION_TYPE;
	uint32_t attrs = flags & SECTION_ATTRIBUTES;

	switch (type) {
		case S_REGULAR:                          printf("\t\t- regular section (S_REGULAR)\n"); break;
		case S_ZEROFILL:                         printf("\t\t- zero-filled section (S_ZEROFILL)\n"); break;
		case S_CSTRING_LITERALS:                 printf("\t\t- only literal c strings (S_CSTRING_LITERALS)\n"); break;
		case S_4BYTE_LITERALS:                   printf("\t\t- only 4-byte literals (S_4BYTE_LITERALS)\n"); break;
		case S_8BYTE_LITERALS:                   printf("\t\t- only 8-byte literals (S_8BYTE_LITERALS)\n"); break;
		case S_LITERAL_POINTERS:                 printf("\t\t- pointers to literals (S_LITERAL_POINTERS)\n"); break;
		case S_NON_LAZY_SYMBOL_POINTERS:         printf("\t\t- non-lazy symbol pointers (S_NON_LAZY_SYMBOL_POINTERS)\n"); break;
		case S_LAZY_SYMBOL_POINTERS:             printf("\t\t- lazy symbol pointers (S_LAZY_SYMBOL_POINTERS)\n"); break;
		case S_SYMBOL_STUBS:                     printf("\t\t- symbol stubs, size in reserved2 (S_SYMBOL_STUBS)\n"); break;
		case S_MOD_INIT_FUNC_POINTERS:           printf("\t\t- init function pointers (S_MOD_INIT_FUNC_POINTERS)\n"); break;
		case S_MOD_TERM_FUNC_POINTERS:           printf("\t\t- termination function pointers (S_MOD_TERM_FUNC_POINTERS)\n"); break;
		case S_COALESCED:                        printf("\t\t- coalesced symbols section (S_COALESCED)\n"); break;
		case S_GB_ZEROFILL:                      printf("\t\t- large zero-filled section (S_GB_ZEROFILL)\n"); break;
		case S_INTERPOSING:                      printf("\t\t- interposing function pointer pairs (S_INTERPOSING)\n"); break;
		case S_16BYTE_LITERALS:                  printf("\t\t- only 16-byte literals (S_16BYTE_LITERALS)\n"); break;
		case S_DTRACE_DOF:                       printf("\t\t- dtrace object format (S_DTRACE_DOF)\n"); break;
		case S_LAZY_DYLIB_SYMBOL_POINTERS:       printf("\t\t- lazy dylib symbol pointers (S_LAZY_DYLIB_SYMBOL_POINTERS)\n"); break;
		case S_THREAD_LOCAL_REGULAR:             printf("\t\t- thread-local regular data (S_THREAD_LOCAL_REGULAR)\n"); break;
		case S_THREAD_LOCAL_ZEROFILL:            printf("\t\t- thread-local zero-filled data (S_THREAD_LOCAL_ZEROFILL)\n"); break;
		case S_THREAD_LOCAL_VARIABLES:           printf("\t\t- thread-local variable descriptors (S_THREAD_LOCAL_VARIABLES)\n"); break;
		case S_THREAD_LOCAL_VARIABLE_POINTERS:   printf("\t\t- pointers to tlv descriptors (S_THREAD_LOCAL_VARIABLE_POINTERS)\n"); break;
		case S_THREAD_LOCAL_INIT_FUNCTION_POINTERS:
												 printf("\t\t- init functions for tlv values (S_THREAD_LOCAL_INIT_FUNCTION_POINTERS)\n"); break;
		case S_INIT_FUNC_OFFSETS:                printf("\t\t- 32-bit offsets to initializers (S_INIT_FUNC_OFFSETS)\n"); break;
		default:                                 printf("\t\t- unknown section type (0x%x)\n", type); break;
	}

	if (attrs & S_ATTR_PURE_INSTRUCTIONS)
		printf("\t\t- only pure machine instructions (S_ATTR_PURE_INSTRUCTIONS)\n");
	if (attrs & S_ATTR_NO_TOC)
		printf("\t\t- not in table of contents (S_ATTR_NO_TOC)\n");
	if (attrs & S_ATTR_STRIP_STATIC_SYMS)
		printf("\t\t- strip static symbols allowed (S_ATTR_STRIP_STATIC_SYMS)\n");
	if (attrs & S_ATTR_NO_DEAD_STRIP)
		printf("\t\t- no dead stripping (S_ATTR_NO_DEAD_STRIP)\n");
	if (attrs & S_ATTR_LIVE_SUPPORT)
		printf("\t\t- live blocks if they reference other live blocks (S_ATTR_LIVE_SUPPORT)\n");
	if (attrs & S_ATTR_SELF_MODIFYING_CODE)
		printf("\t\t- self-modifying code (S_ATTR_SELF_MODIFYING_CODE)\n");
	if (attrs & S_ATTR_DEBUG)
		printf("\t\t- debug-only section (S_ATTR_DEBUG)\n");
	if (attrs & S_ATTR_SOME_INSTRUCTIONS)
		printf("\t\t- contains some instructions (S_ATTR_SOME_INSTRUCTIONS)\n");
	if (attrs & S_ATTR_EXT_RELOC)
		printf("\t\t- has external relocations (S_ATTR_EXT_RELOC)\n");
	if (attrs & S_ATTR_LOC_RELOC)
		printf("\t\t- has local relocations (S_ATTR_LOC_RELOC)\n");

	if (attrs == 0)
		printf("\t\t- no section attributes set\n");
}

void display_generic_section(struct mach_o_ctx *ctx, struct generic_section sect) {
    printf("\n[*] Section %s information:\n\n", sect.sectname);

    printf("\t-> Segment this section goes in: %s\n", sect.segname);

    if (ctx->is_64bit) {
        printf("\t-> Memory address of this section: 0x%llx\n", sect.addr);
        printf("\t-> Size in bytes of this section: %llu\n", sect.size);
    } else {
        printf("\t-> Memory address of this section: 0x%x\n", (uint32_t)sect.addr);
        printf("\t-> Size in bytes of this section: %u\n", (uint32_t)sect.size);
    }
    
    printf("\t-> File offset of this section: %d\n", sect.offset);
    printf("\t-> Section alignment (power of 2): %d\n", sect.align);
    printf("\t-> File offset of relocation entries: %d\n", sect.reloff);
    printf("\t-> Number of relocation entries: %d\n", sect.nreloc);

    printf("\t-> Flags (section type and attributes):\n");
    match_sect_flags(sect.flags);

    printf("\n");
}

void iterate_sects(struct mach_o_ctx *ctx, struct load_command *load_cmd) {
    if (ctx->is_64bit) {
        struct segment_command_64 *seg_cmd = (struct segment_command_64 *)load_cmd;
        struct section_64 *sect = (struct section_64 *)(seg_cmd + 1);

        for (uint32_t i = 0; i < seg_cmd->nsects; i++) {
            struct section_64 curr_sect = sect[i];

            if (strcmp(curr_sect.segname, ctx->text.segname) == 0 && 
                strcmp(curr_sect.sectname, ctx->text.sectname) == 0) {
                printf("\n[?] This segment contains the known section __text. -> Would you like to display details about it? (y/n): ");

                char check;
                while (scanf(" %c", &check)) {
                    if (check == 'y') {
                        display_generic_section(ctx, ctx->text);
                        break;
                    } else if (check == 'n') {
                        break;
                    } else {
                        printf("[*] Invalid input. Try again: ");
                    }
                }
        
            } else if (strcmp(curr_sect.segname, ctx->stubs.segname) == 0 && 
                strcmp(curr_sect.sectname, ctx->stubs.sectname) == 0) {
                printf("\n[?] This segment contains the known section __stubs. -> Would you like to display details about it? (y/n): ");

                char check;
                while (scanf(" %c", &check)) {
                    if (check == 'y') {
                        display_generic_section(ctx, ctx->stubs);
                        break;
                    } else if (check == 'n') {
                        break;
                    } else {
                        printf("[*] Invalid input. Try again: ");
                    }
                }

            } else if (strcmp(curr_sect.segname, ctx->cstring.segname) == 0 && 
                strcmp(curr_sect.sectname, ctx->cstring.sectname) == 0) {
                printf("\n[?] This segment contains the known section __cstring. -> Would you like to display details about it? (y/n): ");

                char check;
                while (scanf(" %c", &check)) {
                    if (check == 'y') {
                        display_generic_section(ctx, ctx->cstring);
                        break;
                    } else if (check == 'n') {
                        break;
                    } else {
                        printf("[*] Invalid input. Try again: ");
                    }
                }

            } else if (strcmp(curr_sect.segname, ctx->data.segname) == 0 && 
                strcmp(curr_sect.sectname, ctx->data.sectname) == 0) {
                printf("\n[?] This segment contains the known section __data. -> Would you like to display details about it? (y/n): ");

                char check;
                while (scanf(" %c", &check)) {
                    if (check == 'y') {
                        display_generic_section(ctx, ctx->data);
                        break;
                    } else if (check == 'n') {
                        break;
                    } else {
                        printf("[*] Invalid input. Try again: ");
                    }
                }

            }
        }
    } else {
        struct segment_command *seg_cmd = (struct segment_command *)load_cmd;
        struct section *sect = (struct section *)(seg_cmd + 1);

        for (uint32_t i = 0; i < seg_cmd->nsects; i++) {
            struct section curr_sect = sect[i];

            if (strcmp(curr_sect.segname, ctx->text.segname) == 0 && 
                strcmp(curr_sect.sectname, ctx->text.sectname) == 0) {
                printf("\n[?] This segment contains the known section __text. -> Would you like to display details about it? (y/n): ");

                char check;
                while (scanf(" %c", &check)) {
                    if (check == 'y') {
                        display_generic_section(ctx, ctx->text);
                        break;
                    } else if (check == 'n') {
                        break;
                    } else {
                        printf("[*] Invalid input. Try again: ");
                    }
                }

            } else if (strcmp(curr_sect.segname, ctx->stubs.segname) == 0 && 
                strcmp(curr_sect.sectname, ctx->stubs.sectname) == 0) {
                printf("\n[?] This segment contains the known section __stubs. -> Would you like to display details about it? (y/n): ");

                char check;
                while (scanf(" %c", &check)) {
                    if (check == 'y') {
                        display_generic_section(ctx, ctx->stubs);
                        break;
                    } else if (check == 'n') {
                        break;
                    } else {
                        printf("[*] Invalid input. Try again: ");
                    }
                }

            } else if (strcmp(curr_sect.segname, ctx->cstring.segname) == 0 && 
                strcmp(curr_sect.sectname, ctx->cstring.sectname) == 0) {
                printf("\n[?] This segment contains the known section __cstring. -> Would you like to display details about it? (y/n): ");

                char check;
                while (scanf(" %c", &check)) {
                    if (check == 'y') {
                        display_generic_section(ctx, ctx->cstring);
                        break;
                    } else if (check == 'n') {
                        break;
                    } else {
                        printf("[*] Invalid input. Try again.: ");
                    }
                }

            } else if (strcmp(curr_sect.segname, ctx->data.segname) == 0 && 
                strcmp(curr_sect.sectname, ctx->data.sectname) == 0) {
                printf("\n[?] This segment contains the known section __data. -> Would you like to display details about it? (y/n): ");

                char check;
                while (scanf(" %c", &check)) {
                    if (check == 'y') {
                        display_generic_section(ctx, ctx->data);
                        break;
                    } else if (check == 'n') {
                        break;
                    } else {
                        printf("[*] Invalid input. Try again: ");
                    }
                }
            }
        }
    }
}

void display_segment_info(struct mach_o_ctx *ctx, struct load_command *load_cmd) {
    printf("[*] Segment Load Command information:\n\n");
    printf("\t-> The segment load command indicates that a part of this file is to be mapped into the task's address space.\n");

    if (ctx->is_64bit) {
        struct segment_command_64 *seg_cmd = (struct segment_command_64 *)load_cmd;

        printf("\t-> Segment name: %s\n", seg_cmd->segname);

        printf("\t-> Memory address of this segment: 0x%llx\n", seg_cmd->vmaddr);
        printf("\t-> Memory size of this segment: %llu\n", seg_cmd->vmsize);
        printf("\t-> File offset of this segment: %llu\n", seg_cmd->fileoff);
        printf("\t-> Amount to map from this file: %llu\n", seg_cmd->filesize);

        printf("\t-> Maximum VM protection:\n");
        match_vm_prot(seg_cmd->maxprot);

        printf("\t-> Initial VM protection:\n");
        match_vm_prot(seg_cmd->initprot);

        printf("\t-> Number of sections in segment: %d\n", seg_cmd->nsects);

        printf("\t-> Flags:\n");
        match_seg_flags(seg_cmd->flags);

        printf("\n");
        iterate_sects(ctx, load_cmd);
    
    } else {
        struct segment_command *seg_cmd = (struct segment_command *)load_cmd;

        printf("\t-> Segment name: %s\n", seg_cmd->segname);

        printf("\t-> Memory address of this segment: 0x%x\n", seg_cmd->vmaddr);
        printf("\t-> Memory size of this segment: %d\n", seg_cmd->vmsize);
        printf("\t-> File offset of this segment: %d\n", seg_cmd->fileoff);
        printf("\t-> Amount to map from this file: %d\n", seg_cmd->filesize);

        printf("\t-> Maximum VM protection:\n");
        match_vm_prot(seg_cmd->maxprot);

        printf("\t-> Initial VM protection:\n");
        match_vm_prot(seg_cmd->initprot);

        printf("\t-> Number of sections in segment: %d\n", seg_cmd->nsects);

        printf("\t-> Flags:\n");
        match_seg_flags(seg_cmd->flags);
        
        printf("\n");
        iterate_sects(ctx, load_cmd);
    }
}

void choose_segment(struct mach_o_ctx *ctx) {
    uint8_t *load_cmds_zone;
    if (ctx->is_64bit) {
        load_cmds_zone = ctx->base + sizeof(struct mach_header_64);
    } else {
        load_cmds_zone = ctx->base + sizeof(struct mach_header);
    }

    int count = 0;
    struct load_command **cmds;
    for (uint32_t i = 0; i < ctx->nmcds; i++) {
        struct load_command *load_cmd = (struct load_command *)load_cmds_zone;

        uint32_t cmd_name = load_cmd->cmd;
        uint32_t cmd_size = load_cmd->cmdsize;
        
        if (cmd_name == LC_SEGMENT || cmd_name == LC_SEGMENT_64) {
            if (count == 0) {
                cmds = malloc(sizeof(struct load_command *));
                DIE(!cmds, "[ERROR]: malloc() failed.");
            } else {
                cmds = realloc(cmds, (count + 1) * sizeof(struct load_command *));
                DIE(!cmds, "[ERROR]: realloc() failed.");
            }
            cmds[count] = load_cmd;
            count++;
        }

        load_cmds_zone += cmd_size;
    }

    int seg_number = -1;
    while (1) {
        printf("------------------------------------------------------------------------------------------------------\n");
        printf("\n[*] Choose segment to display information (enter a number from 1 to %d or press 0 to escape): ", count);

        if (scanf("%d", &seg_number) != 1) {
            while (getchar() != '\n');
            printf("\n[*] Invalid input. Please enter a number.\n");
            printf("\n------------------------------------------------------------------------------------------------------\n");
            continue;
        }

        if (seg_number == 0)  {
            printf("\n------------------------------------------------------------------------------------------------------\n");
            break;
        }

        if (seg_number - 1 < 0 || seg_number - 1 >= count) {
            printf("\n[*] Invalid number. Try again.\n\n");
            printf("------------------------------------------------------------------------------------------------------\n");
            continue;
        }

        printf("\n");
        display_segment_info(ctx, cmds[seg_number - 1]);
        printf("------------------------------------------------------------------------------------------------------\n");
    }
} 
