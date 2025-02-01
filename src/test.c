#include <stdio.h>
#include "tinyss.h"
#include <string.h>
#include <malloc.h>

void tss_gfunc(tss_varlist *list, char *name, char *stack[], char sp) {
    if(strcmp(name, "print") == 0) {
        if(sp != 0) {
            printf("%s\n", stack[--sp]);
        }
    }
}

int main(void) {
    char code[] = "\t  nop\n  gpushb\thello_from_tinyss\n gcall print\n";
    tss_varlist list;
    tss_vlinit(&list);
    tss_printerr(tss_docode(&list, code, strlen(code)));
    return 0;
}
