#include <stdio.h>
#include "tinyss.h"
#include <string.h>
#include <malloc.h>

int main(void) {
    char code[] = "nop\nnop\nqqq aaa\nnop nop\n   \nnop\n";
    tss_printerr(tss_docode(code, strlen(code)));
    return 0;
}
