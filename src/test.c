#include <stdio.h>
#include "tinyss.h"
#include <string.h>
#include <malloc.h>

int main(void) {
    char code[] = "nop\nnop\nqqq aaa\nnop nop\n   \nnop\n";
    tss_exception e = tss_docode(code, strlen(code));
    tss_printerr(&e);
    return 0;
}
