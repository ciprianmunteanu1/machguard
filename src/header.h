#pragma once

#include "parser.h"

void match_magic_number(uint32_t magic);
void match_cpu_type(cpu_type_t cputype);
void match_arm_cpu_subtype(cpu_subtype_t subtype);
void match_x86_cpu_subtype(cpu_subtype_t subtype);
void match_file_type(uint32_t filetype);
void match_flags(uint32_t flags);
void print_mach_header(struct mach_o_ctx *ctx);
