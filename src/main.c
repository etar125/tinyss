#include "tinyss.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>

void tss_printerr(tss_exception *e) {
    if(e->code == 0) return;
    printf("%d:%d ", e->line, e->symbol);
    if(e->code == 1) {
        printf("unknown error\n");
    } else if(e->code == 2) {
        printf("too many args\n");
    }
}

bool tss_ie(char *data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        if(data[i] >= '!') return false;
    } return true;
}
bool tss_strcmp(char *data1, size_t size1, char *data2, size_t size2) {
    if(size1 != size2) return false;
    for(size_t i = 0; i < size1 && i < size2; i++) {
        if(data1[i] != data2[i]) return false;
    } return true;
}

tss_exception tss_docode(char *code, size_t size) {
    uint8_t argc = 1;
    size_t argpos[6], line = 0;
    tss_exception ret;
    ret.code = 0;
    argpos[0] = 0;
    for(size_t i = 0; i < size; i++) {
        if(code[i] == '\n') {
            argpos[argc] = i + 1;
            line++;
            if(tss_strcmp(&code[argpos[0]], argpos[1] - argpos[0] - 1,
                          "nop", 3)) { }

            argc=1;
            argpos[0] = i + 1;
        } else if(code[i] == ' ') {
            if(argc == 1 && argpos[0]-i==0) {
                argpos[0]=i;
            }
            if(argc < 5) {
                argpos[argc] = i + 1;
                argc++;
            } else {
                ret.line = line;
                ret.symbol = i;
                ret.code = 2;
                return ret;
            }
        }
    } return ret;
}
