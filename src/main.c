#include "tinyss.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>

#define _retset ret.line = line; ret.symbol = ri

#define checkargc(e) if(argc != e) { _retset; ret.code = argc < e ? 3 : 2; return ret; }

void tss_printerr(tss_exception e) {
    if(e.code == 0) return;
    printf("%d:%d [%d] ", e.line, e.symbol, e.code);
    if(e.code == 1) {
        printf("...\n");
    } else if(e.code == 2) {
        printf("too many args\n");
    } else if(e.code == 3) {
        printf("not enough args\n");
    } else if(e.code == 4) {
        printf("wrong args\n");
    } else if(e.code == 5) {
        printf("stack overflow\n");
    } else {
        printf("unknown error\n");
    }
}

bool tss_ie(char *data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        if(data[i] >= '!') return false;
    } return true;
}
bool tss_strcmp(char *data1, size_t size1, char *data2, size_t size2) {
    if(size1 != size2) return false;
    for(size_t i = 0; i < size1; i++) {
        if(data1[i] != data2[i]) return false;
    } return true;
}

char schar(char ch) {
    if(ch == 'n') {
        return '\n';
    } else if(ch == 't') {
        return '\t';
    } else if(ch == 'a') {
        return '\033';
    } else if(ch == '\\') {
        return '\\';
    } else if(ch == '\'') {
        return '\'';
    } else if(ch == '"') {
        return '"';
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

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while(start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
} char* ltoa(long int num, char* str, int base) {
    int i = 0;
    bool m = false;
    if(num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    if(num < 0 && base == 10) {
        m = true;
        num = -num;
    }
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    if(m) { str[i++] = '-'; }
    str[i] = '\0';
    reverse(str, i);
    return str;
}

tss_exception tss_docode(tss_varlist *list, char *code, size_t size) {
    uint8_t argc = 0;
    size_t i, ri = 0, line = 0, psize;
    char *arg, *tmp, *tmp2;
    tss_arg args[5];
    for(uint8_t i = 0; i < 5; i++) {
        tss_ainit(&args[i]);
    }
    bool com = false;
    tss_stack st;
    tss_sinit(&st);
    tss_exception ret;
    ret.code = 0;
    for(i = 0; i < size; i++, ri++) {
        if(code[i] == '\n') {
            line++;
            arg = tss_aget(&args[0]);
            psize = strlen(arg);
            if(tss_strcmp(arg, psize, "nop", 3)) { }
            else if(tss_strcmp(arg, psize, "gpushb", 6)) {
                checkargc(1);
                arg = tss_aget(&args[1]);
                psize = strlen(arg);
                if(psize == 0 || (psize == 1 && arg[0] == '$')) {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                tss_push(&st, arg[0] == '$' ? tss_getvar(list, ++arg) : arg);
            } else if(tss_strcmp(arg, psize, "gcall", 5)) {
                checkargc(1);
                arg = tss_aget(&args[1]);
                if(strlen(arg) == 0 || arg[0] == '$') {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                tss_gfunc(list, &st, arg);
            } else if(tss_strcmp(arg, psize, "define", 6)) {
                checkargc(2);
                arg = tss_aget(&args[1]);
                tmp = tss_aget(&args[2]);
                if(strlen(arg) == 0 || arg[0] == '$' || strlen(tmp) == 0) {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                tss_setvar(list, arg, tmp[0] == '$' ? tss_getvar(list, ++tmp) : tmp);
            } else if(tss_strcmp(arg, psize, "del", 3)) {
                checkargc(1);
                tss_delvar(list, tss_aget(&args[1]));
            } else if(tss_strcmp(arg, psize, "op", 2)) {
                checkargc(3);
                arg  = tss_aget(&args[1]);
                tmp2 = tss_aget(&args[2]);
                tmp  = tss_aget(&args[3]);
                tmp  = tmp[0] == '$' ? tss_getvar(list, ++tmp) : tmp;
                if(arg == NULL || tmp == NULL) {
                    _retset;
                    ret.code = 6;
                    return ret;
                } if(strlen(tmp2) != 1) {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                long int a = atol(tss_getvar(list, arg)), b = atol(tmp);
                char *buff = malloc(32);
                switch(tmp2[0]) {
                    case '+':
                        tss_setvar(list, arg, ltoa(a + b, buff, 10));
                        break;
                    case '-':
                        tss_setvar(list, arg, ltoa(a - b, buff, 10));
                        break;
                    case '*':
                        tss_setvar(list, arg, ltoa(a * b, buff, 10));
                        break;
                    case '/':
                        tss_setvar(list, arg, ltoa(a / b, buff, 10));
                        break;
                    case '%':
                        tss_setvar(list, arg, ltoa(a % b, buff, 10));
                        break;
                    case '^':
                        tss_setvar(list, arg, ltoa(a ^ b, buff, 10));
                        break;
                    default:
                        _retset;
                        ret.code = 4;
                        return ret;
                        break;
                } free(buff);
            }

            argc=0;
            for(uint8_t i = 0; i < 5; i++) {
                if(args[i].data != NULL) { free(args[i].data); }
                tss_ainit(&args[i]);
            } ri = 0;
        } else if((code[i] == '"' || code[i] == '\'')) {
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
            } else { tss_aadd(&args[argc], code[i]); }
        } else if(code[i] == ':' && argc == 0 && args[0].data == NULL) {
            while(code[i] != '\n') { i++; }
            continue;
        } else if(code[i] == '\\') { tss_aadd(&args[argc], schar(code[++i])); }
        else { tss_aadd(&args[argc], code[i]); }
    } return ret;
}
