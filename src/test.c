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
    "# main\n\
    gpushb \"goto test\"\n\
    gcall printline\n\
    goto main\n\
    \n\
    gpushb \"не должно выводиться\"\n\
    gcall printline\n\
    \n\
    :call\n\
    gpushb \"должно выводиться\"\n\
    gcall printline\n\
    ret\n\
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
    gcall printline\n\
    call call\n\
    op a * -1\n\
    if a g 512\n\
        gpushb \"512 больше 512\"\n\
        gcall printline\n\
    elif a e 512\n\
        gpushb '512 равно 512'\n\
        gcall printline\n\
        exit\n\
    else\n\
        gpushb '512 не больше 512'\n\
        gcall printline\n\
    end\n\
    \n\
    gpushb \"не должно выводиться\"\n\
    gcall printline";
    tss_varlist list;
    tss_vlinit(&list);
    tss_printerr(tss_docode(&list, code, strlen(code)));
    return 0;
}
