#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tinyss.h"

void tss_gfunc(tss_varlist *list, tss_stack *stack, char *name) {
    if(strcmp(name, "echo") == 0) {
        printf("%s", tss_pop(stack));
    }
}

int main(void) {
    char code[] = {
    "define from TinySS\n\
    gpushb '!\\n'\n\
    gpushb $from\n\
    gpushb 'Привет из '\n\
    gcall echo\ngcall echo\ngcall echo\n"
    };
    printf("compiling...\n");
    tsf_file f = tbc_compile(code, strlen(code));
    printf("functions count: %hu\n", f.tsize);
    for(uint16_t i = 0; i < f.tsize; i++) {
        if(f.table[i].name != NULL) {
            printf("  %s %u %lu\n", f.table[i].name, (uint32_t)f.table[i].nsize, f.table[i].pos);
        }
    }
    if(f.code != NULL) {
        printf("code size: %lu\ncode:\n", f.csize);
        for(size_t i = 0; i < f.csize; i++) {
            if(f.code[i] < 31) { printf("%u ", (uint8_t)f.code[i]); }
            else { printf("%c ", f.code[i]); }
        } printf("\n");
    }
    else { printf("error %lu\n", f.csize); }
    tss_varlist list;
    tss_vlinit(&list);
    printf("doing code...\n");
    tss_printerr(tss_docode(&list, &f));
    tsf_free(&f);
    tss_vlfree(&list);
    return 0;
}
