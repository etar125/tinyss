#include <stdio.h>
#include "tinyss.h"
#include <string.h>
#include <malloc.h>

void tss_gfunc(tss_varlist *list, tss_stack *stack, char *name) {
    if(strcmp(name, "print") == 0) {
        printf("%s\n", tss_pop(stack));
    }
}

int main(void) {
    char code[] = "\t  nop\n  :\tx\ndefine var test\n  gpushb\t\"hello из \\\\\\\"tinyss\\\\\\''\n gcall\tprint\n";
    tss_varlist list;
    tss_vlinit(&list);
    tss_printerr(tss_docode(&list, code, strlen(code)));
    printf("%s\n", tss_getvar(&list, "var"));
    return 0;
}
