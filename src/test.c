#include <stdio.h>
#include "tinyss.h"
#include <string.h>
#include <malloc.h>

int main(void) {
    char code[] = "   nop\n\tnop\nqqq aaa\nnop\tnop\n   \nnop\n";
    tss_printerr(tss_docode(code, strlen(code)));
    return 0;
}
