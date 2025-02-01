#include "tinyss.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>

#define _retset ret.line = line; ret.symbol = i

void tss_printerr(tss_exception e) {
    if(e.code == 0) return;
    printf("%d:%d ", e.line, e.symbol);
    if(e.code == 1) {
        printf("unknown error\n");
    } else if(e.code == 2) {
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

tss_exception tss_docode(tss_varlist *list, char *code, size_t size) {
    uint8_t argc = 1;
    size_t argpos[6], line = 0, psize;
    char *arg, sp = 0, *stack[8], *tmp;
    tss_exception ret;
    ret.code = 0;
    argpos[0] = 0;
    for(size_t i = 0; i < size; i++) {
        if(code[i] == '\n') {
            argpos[argc] = i + 1;
            line++;
            arg = &code[argpos[0]];
            psize = argpos[1] - argpos[0] - 1;
            if(tss_strcmp(arg, psize, "nop", 3)) { }
            else if(tss_strcmp(arg, psize, "gpushb", 6)) {
                if(argc != 2) {
                    _retset;
                    ret.code = argc < 2 ? 3 : 2;
                    return ret;
                }
                arg = &code[argpos[1]];
                psize = argpos[2] - argpos[1] - 1;
                if(psize == 0 || (psize == 1 && arg[0] == '$')) {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                if(arg[0] == '$') {
                    arg++;
                    tmp = malloc(psize + 1);
                    for(size_t i = 0; i < psize; i++) {
                        tmp[i] = arg[i];
                    } tmp[psize] = '\0';
                    if(sp != 8) {
                        tmp = tss_getvar(list, tmp);
                        stack[sp] = malloc(strlen(tmp));
                        strcpy(stack[sp], tmp);
                        tmp = NULL;
                        sp++;
                    } else {
                        free(tmp);
                        _retset;
                        ret.code = 5;
                    }
                } else {
                    tmp = malloc(psize + 1);
                    for(size_t i = 0; i < psize; i++) {
                        tmp[i] = arg[i];
                    } tmp[psize] = '\0';
                    if(sp != 8) {
                        stack[sp] = tmp;
                        tmp = NULL;
                        sp++;
                    } else {
                        free(tmp);
                        _retset;
                        ret.code = 5;
                    }
                }
            } else if(tss_strcmp(arg, psize, "gcall", 5)) {
                if(argc != 2) {
                    _retset;
                    ret.code = argc < 2 ? 3 : 2;
                    return ret;
                }
                arg = &code[argpos[1]];
                psize = argpos[2] - argpos[1] - 1;
                if(psize == 0 || arg[0] == '$') {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                tmp = malloc(psize + 1);
                for(size_t i = 0; i < psize; i++) {
                    tmp[i] = arg[i];
                } tmp[psize] = '\0';
                tss_gfunc(list, tmp, stack, sp);
                free(tmp);
            }

            argc=1;
            argpos[0] = i + 1;
        } else if(code[i] == ' ' || code[i] == '\t') {
            if(argc == 1 && (argpos[0] - i) == 0) {
                argpos[0] = i + 1;
            } else if(argc < 5) {
                argpos[argc] = i + 1;
                argc++;
            } else {
                _retset;
                ret.code = 2;
                return ret;
            }
        }
    } return ret;
}
