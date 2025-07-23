#pragma once

#include "parser.h"
#include <mach/vm_prot.h>

void match_cmd_name(uint32_t cmd_name);
void display_load_cmds_summary(struct mach_o_ctx *ctx);

void match_vm_prot(vm_prot_t prot);
void match_seg_flags(uint32_t flags);
void display_segment_info(struct mach_o_ctx *ctx, struct load_command *load_cmd);
void choose_segment(struct mach_o_ctx *ctx);
void match_sect_flags(uint32_t flags);
void display_generic_section(struct mach_o_ctx *ctx, struct generic_section sect);
void iterate_sects(struct mach_o_ctx *ctx, struct load_command *load_cmd);

void display_entry_point(struct mach_o_ctx *ctx);

void display_dylibs(struct mach_o_ctx *ctx);