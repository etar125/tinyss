#include <stdio.h>
#include "tinyss.h"
#include <string.h>
#include <malloc.h>

void tss_gfunc(tss_varlist *list, tss_stack *stack, char *name) {
    if(strcmp(name, "printline") == 0) {
        printf("%s\n", tss_pop(stack));
    } else if(strcmp(name, "print") == 0) {
        printf("%s", tss_pop(stack));
    }
}

int main(void) {
    char code[] =
    ":rem main\n\
    gpushb \"goto test\"\n\
    gcall printline\n\
    goto main\n\
    \n\
    gpushb \"не должно выводиться\"\n\
    gcall printline\n\
    \n\
    :main\n\
    \tdefine a 2\n\
        define\tb 512\n\
    op a ^ 10\n\
    op b / -256\n\
    op a / $b\n\
    gpushb \"2^10 / (512 / -256) = '\n\
    gcall print\n\
    gpushb $a\n\
    gcall printline";
    tss_varlist list;
    tss_vlinit(&list);
    tss_printerr(tss_docode(&list, code, strlen(code)));
    return 0;
}
