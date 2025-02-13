#include "tinyss.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>

#define _retset ret.line = line; ret.symbol = ri

#define checkargc(e) if(argc != e) { _retset; ret.code = argc < e ? 3 : 2; return ret; }

char *tss_code[] = {
    "no error",
    "test error?",
    "too many args",
    "not enough args",
    "wrong args",
    "stack overflow",
    "not found label"
};

void tss_printerr(tss_exception e) {
    if(e.code == 0) return;
    printf("%d:%d [%d] ", e.line, e.symbol, e.code);
    if(e.code < 7) {
        printf("%s\n", tss_code[e.code]);
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
        a->size += prev / 2;
        a->data = malloc(a->size);
        memcpy(a->data, old, prev);
        free(old);
    } a->data[a->pos++] = ch;
} char* tss_aget(tss_arg *a) {
    if(a == NULL || a->data == NULL) { return NULL; }
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
} char* itoa(int num, char* str, int base) {
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
} int _pow(int i, int n) {
    int res = 1;
    for(;;) {
        if (n & 1)
            res *= i;
        n >>= 1;
        if(!n) { break; }
        i *= i;
    } return res;
}

tss_exception tss_docode(tss_varlist *list, char *code, size_t size) {
    uint8_t argc = 0;
    size_t i, ri = 0, line = 0, psize;
    char *arg0, *arg1, *arg2, *arg3, *arg4,
         *tmp1, *tmp2;
    tss_arg args[5];
    for(uint8_t i = 0; i < 5; i++) {
        args[i].data = NULL;
        tss_ainit(&args[i]);
    }
    bool com = false;
    tss_stack st;
    tss_sinit(&st);
    tss_exception ret;
    ret.code = 0;
    
    for(i = 0; i <= size; i++, ri++) {
        if(i == size || code[i] == '\n') {
            if(argc == 0 && args[0].data == NULL) { continue; }
            line++;
            arg0 = tss_aget(&args[0]);
            psize = strlen(arg0);
            if(tss_strcmp(arg0, psize, "nop", 3)) { }
            else if(tss_strcmp(arg0, psize, "gpushb", 6)) {
                checkargc(1);
                arg1 = tss_aget(&args[1]);
                psize = strlen(arg1);
                if(psize == 0 || (psize == 1 && arg1[0] == '$')) {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                tss_push(&st, arg1[0] == '$' ? tss_getvar(list, ++arg1) : arg1);
            } else if(tss_strcmp(arg0, psize, "gcall", 5)) {
                checkargc(1);
                arg1 = tss_aget(&args[1]);
                if(strlen(arg1) == 0 || arg1[0] == '$') {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                tss_gfunc(list, &st, arg1);
            } else if(tss_strcmp(arg0, psize, "define", 6)) {
                checkargc(2);
                arg1 = tss_aget(&args[1]);
                arg2 = tss_aget(&args[2]);
                if(strlen(arg1) == 0 || arg1[0] == '$' || strlen(arg2) == 0) {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                tss_setvar(list, arg1, arg2[0] == '$' ? tss_getvar(list, ++arg2) : arg2);
            } else if(tss_strcmp(arg0, psize, "del", 3)) {
                checkargc(1);
                tss_delvar(list, tss_aget(&args[1]));
            } else if(tss_strcmp(arg0, psize, "op", 2)) {
                checkargc(3);
                arg1  = tss_aget(&args[1]);
                arg2  = tss_aget(&args[2]);
                arg3  = tss_aget(&args[3]);
                arg3  = arg3[0] == '$' ? tss_getvar(list, ++arg3) : arg3;
                if(arg1 == NULL || arg3 == NULL) {
                    _retset;
                    ret.code = 6;
                    return ret;
                } if(strlen(arg2) != 1) {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                int a = atoi(tss_getvar(list, arg1)), b = atoi(arg3);
                tmp1 = malloc(13);
                switch(arg2[0]) {
                    case '+':
                        tss_setvar(list, arg1, itoa(a + b, tmp1, 10));
                        break;
                    case '-':
                        tss_setvar(list, arg1, itoa(a - b, tmp1, 10));
                        break;
                    case '*':
                        tss_setvar(list, arg1, itoa(a * b, tmp1, 10));
                        break;
                    case '/':
                        tss_setvar(list, arg1, itoa(a / b, tmp1, 10));
                        break;
                    case '%':
                        tss_setvar(list, arg1, itoa(a % b, tmp1, 10));
                        break;
                    case '^':
                        tss_setvar(list, arg1, itoa(_pow(a, b), tmp1, 10));
                        break;
                    default:
                        _retset;
                        ret.code = 4;
                        return ret;
                        break;
                } free(tmp1);
            } else if(tss_strcmp(arg0, psize, "goto", 4)) {
                checkargc(1);
                arg1 = tss_aget(&args[1]);
                if(arg1[0] == '$') {
                    _retset;
                    ret.code = 4;
                    return ret;
                }
                
                bool started = false;
                int pos = 0;
                for(size_t a = 0; a <= size; a++) {
                    if(arg1[pos] == '\0') {
                        if(a == size - 1 || code[a] == '\n') {
                            while(code[a] != '\n' && a < size) {
                                a++;
                            } i = a;
                            break;
                        } while(code[a] != '\n' && a < size) {
                            a++;
                        } pos = 0;
                        started = false;
                    }
                    if(code[a] == '\n' || a == size) {
                        started = false;
                        pos = 0;
                        continue;
                    }
                    if(!started) {
                        if(code[a] == ' ' || code[a] == '\t') { continue; }
                        else if(code[a] != ':') {
                            while(code[a] != '\n' && a < size) { a++; } pos = 0;
                        } else { started = true; }
                        continue;
                    } if(code[a] == arg1[pos]) { pos++; }
                    else {
                        while(code[a] != '\n' && a < size) { a++; }
                        pos = 0, started = false;
                    }
                }
                if(pos == 0) {
                    _retset;
                    ret.code = 6;
                    return ret;
                }
            }
            
            for(uint8_t i = 0; i < argc + 1; i++) {
                if(args[i].data != NULL) { free(args[i].data); }
                tss_ainit(&args[i]);
            } ri = 0, argc = 0;
        } else if((code[i] == '"' || code[i] == '\'')) {
            if(com) { com = false; }
            else { com = true; }
        } else if(code[i] == ' ' || code[i] == '\t') {
            if(argc == 0 && args[0].data == NULL) { }
            else if(!com) {
                if(argc < 5) {
                    argc++;
                } else {
                    _retset;
                    ret.code = 2;
                    return ret;
                }
            } else { tss_aadd(&args[argc], code[i]); }
        } else if(code[i] == ':' && argc == 0 && args[0].data == NULL) {
            while(i < size && code[i] != '\n') { i++; }
            continue;
        } else if(code[i] == '\\') { tss_aadd(&args[argc], schar(code[++i])); }
        else { tss_aadd(&args[argc], code[i]); }
    } return ret;
}
