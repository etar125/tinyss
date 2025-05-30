#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tinyss.h"

void tss_gfunc(tss_varlist *list, tss_stack *stack, char *name) {
    
}

int main(void) {
    char code[] = {
    "define i 0\n\
    gpushb start\n\
    gcall printline\n\
    :loop\n\
        if i e 1000000\n\
            gpushb end\n\
            gcall printline\n\
            exit\n\
        end\n\
        op i + 1\n\
    goto loop"
    };
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
            if(f.code[i] < 22) { printf("%d ", (int)f.code[i]); }
            else { printf("%c ", f.code[i]); }
        } printf("\n");
    }
    else { printf("error %lu\n", f.csize); }
    tsf_free(&f);
    return 0;
}
