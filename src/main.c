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

char schar(char ch) {
    return ch; 
    if(ch == 'n') {
        return '\n';
    } else if(ch == 't') {
        return '\t';
    } else if(ch == 'a') {
        return '\033';
    } return ch;
}

typedef struct {
    char *data;
    size_t size, pos;
} tss_arg;
void tss_ainit(tss_arg *a) {
    if(a == NULL) { return; }
    a->data = NULL;
    a->size = 0;
    a->pos = 0;
} void tss_aadd(tss_arg *a, char ch) {
    if(a == NULL) { return; }
    if(a->data == NULL) {
        a->size = 8;
        a->data = malloc(8);
        a->data[a->pos++] = ch;
        return;
    }
    if(a->pos == a->size) {
        char *old = a->data;
        size_t prev = a->size;
        a->size += prev / 4;
        a->data = malloc(a->size);
        memcpy(a->data, old, prev);
        free(old);
    } a->data[a->pos++] = ch;
} char* tss_aget(tss_arg *a) {
    if(a == NULL) { return NULL; }
    if(a->pos == a->size) {
        char *old = a->data;
        size_t prev = a->size;
        a->size += 1;
        a->data = malloc(a->size);
        memcpy(a->data, old, prev);
        free(old);
    } a->data[a->pos++] = '\0';
    return a->data;
}

tss_exception tss_docode(tss_varlist *list, char *code, size_t size) {
    uint8_t argc = 0;
    size_t line = 0, psize;
    char *arg, *tmp;
    tss_arg args[5];
    for(uint8_t i = 0; i < 5; i++) {
        tss_ainit(&args[i]);
    }
    bool com = false;
    tss_stack st;
    tss_sinit(&st);
    tss_exception ret;
    ret.code = 0;
    for(size_t i = 0; i < size; i++) {
        if(code[i] == '\n') {
            // argpos[argc] = i;
            line++;
            arg = tss_aget(&args[0]);
            psize = strlen(arg);
            if(tss_strcmp(arg, psize, "nop", 3)) { }
            else if(tss_strcmp(arg, psize, "gpushb", 6)) {
                if(argc != 1) {
                    _retset;
                    ret.code = argc < 2 ? 3 : 2;
                    return ret;
                }
                arg = tss_aget(&args[1]);
                psize = strlen(arg);
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
                    tss_push(&st, tss_getvar(list, tmp));
                    free(tmp);
                } else {
                    tmp = malloc(psize + 1);
                    for(size_t i = 0; i < psize; i++) {
                        tmp[i] = arg[i];
                    } tmp[psize] = '\0';
                    tss_push(&st, tmp);
                    free(tmp);
                }
            } else if(tss_strcmp(arg, psize, "gcall", 5)) {
                if(argc != 1) {
                    _retset;
                    ret.code = argc < 2 ? 3 : 2;
                    return ret;
                }
                arg = tss_aget(&args[1]);
                psize = args[1].size;
                if(psize == 0 || arg[0] == '$') {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                tmp = malloc(psize + 1);
                for(size_t i = 0; i < psize; i++) {
                    tmp[i] = arg[i];
                } tmp[psize] = '\0';
                tss_gfunc(list, &st, tmp);
                free(tmp);
            }

            argc=0;
            for(uint8_t i = 0; i < 5; i++) {
                if(args[i].data != NULL) { free(args[i].data); }
                tss_ainit(&args[i]);
            }
        } else if((code[i] == '"' || code[i] == '\'') && (i != 0 && code[i - 1] != '\\')) {
            if(com) { com = false; }
            else { com = true; }
        } else if(code[i] == ' ' || code[i] == '\t') {
            if(argc == 0 && args[0].data == NULL) {
                
            } else if(!com) {
                if(argc < 5) {
                    argc++;
                } else {
                    _retset;
                    ret.code = 2;
                    return ret;
                }
            } else { tss_aadd(&args[argc], schar(code[i])); }
        } else if(code[i] == ':' && argc == 0 && args[0].data == NULL) {
            while(code[i] != '\n') { i++; }
            continue;
        } else { tss_aadd(&args[argc], schar(code[i])); }
    } return ret;
}
