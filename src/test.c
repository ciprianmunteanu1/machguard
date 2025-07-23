#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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


char str[6] = "system";

int main() {

    printf("Hello, world!");
    return 0;
}