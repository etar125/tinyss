#include <stdio.h>
#include "tinyss.h"
#include <string.h>
#include <malloc.h>

#define _run(e) tss_printerr(tss_docode(&list, e, strlen(e)));

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

    char error1[] = {
        "gpushb a\n\
        gpushb a\n\
        gpushb a\n\
        gpushb a\n\
        gpushb a\n\
        gpushb a\n\
        gpushb a\n\
        gpushb a\n\
        gpushb a\n\
        exit"
    }, error2[] = {
        "exit 3 2 1\n\
        exit"
    }, error3[] = {
        "exit 1"
    };
    tss_varlist list;
    tss_vlinit(&list);
    printf("default test:\n\n");
    _run(code);
    printf("\nerror tests:\n\n");
    _run(error1);
    _run(error2);
    _run(error3);
    return 0;
}
