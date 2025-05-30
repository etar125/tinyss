#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "tinyss.h"

int main(void) {
    tsf_file f;
    f.tsize = 2;
    f.table = malloc(sizeof(tsf_func) * 2);
    char *one = malloc(3), *two = malloc(4), *three = malloc(11);
    strcpy(one, "ab\0");
    strcpy(two, "666\0");
    strcpy(three, "0123456789\0");
    f.table[0].nsize = 3;
    f.table[0].name = one;
    f.table[0].pos = 3;
    f.table[1].nsize = 4;
    f.table[1].name = two;
    f.table[1].pos = 661;
    f.csize = 11;
    f.code = three;
    tsf_write(&f, "tsf_test.bin");
    tsf_free(&f);
    f.tsize = 0;
    f.csize = 0;
    f = tsf_read("tsf_test.bin");
    printf("table size: %u\n", f.tsize);
    for(unsigned int i = 0; i < f.tsize; i++) {
        tsf_func *u = &f.table[i];
        printf("%u:\nsize: %hu\nname: %s\npos: %lu\n",
        i, u->nsize, u->name, u->pos);
    }
    printf("end of table\ncode size: %lu\ncode: %s\n",
    f.csize, f.code);
    tsf_free(&f);
    return 0;
}
