#include "tinyss.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>

#define freeargs() for(uint8_t i = 0; i < argc; i++) { if(args[i].data != NULL) { free(args[i].data); } }
#define retret(a, b) ret.symbol = a; ret.code = b; tss_free(&st); tss_free(&cst); freeargs(); return ret
#define checkargc(e) if(argc != e) { retret(args[0].pos, argc < e ? 3 : 2); }

static inline char tss_schar(char ch)  {
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
} static inline void tss_reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while(start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
} static inline char* tss_itoa(int num, char* str, int base) {
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
    tss_reverse(str, i);
    return str;
} static inline int tss_pow(int i, int n) {
    int res = 1;
    for(;;) {
        if (n & 1)
            res *= i;
        n >>= 1;
        if(!n) { break; }
        i *= i;
    } return res;
}

#define check() if(i > f->csize) { retret(as, 9); }
#define checksize(x) if(i + x > f->csize) { retret(cur->pos, 10); }

tss_exception tss_docode(tss_varlist *list, tsf_file *f) {
    tss_stack st, cst;
    tss_sinit(&st);
    tss_sinit(&cst);
    tss_exception ret;
    ret.code = 0;
    size_t as = 0;
    uint8_t t8 = 0;
    char *tmp;
    tbc_opcode op = 0;
    uint8_t argc = 0;
    tss_arg args[4];
    tss_arg *cur;
    if(f->code == NULL || f->csize == 0) { retret(0, 9); }
    for(size_t i = 0; i < f->csize;) {
        as = i;
        op = (tbc_opcode)f->code[i];
        i++; check();
        argc = f->code[i];
        if(argc != 0) { i++; check(); }
        for(t8 = 0; t8 < argc; t8++) {
            cur = &args[t8];
            cur->pos = i + 1;
            cur->size = (size_t)f->code[i];
            i++; check();
            if(cur->size != 0) {
                checksize(cur->size);
                cur->data = &f->code[i];
                i += cur->size;
            } else { i++; }
            if(t8 != argc - 1) { check(); }
        }
        for(t8 = 0; t8 < argc; t8++) {
            tmp = malloc(args[t8].size + 1);
            memcpy(tmp, args[t8].data, args[t8].size);
            tmp[args[t8].size] = '\0';
            args[t8].data = tmp;
        }
        
        if(op == NUL || op == NOP) { }
        else if(op == PUSH) {
            checkargc(1);
            if(args[0].size == 0 || (args[0].size == 1 && args[0].data[0] == '$')) {
                retret(args[1].pos, 4);
            } if(st.sp == 0) {
                retret(args[0].pos, 5);
            } tss_push(&st, args[0].data[0] == '$' ? tss_getvar(list, args[0].data + 1) : args[0].data);
        } else if(op == GCALL) {
            checkargc(1);
            if(args[0].size == 0 || (args[0].size == 1 && args[0].data[0] == '$')) {
                retret(args[0].pos, 4);
            } tss_gfunc(list, &st, args[0].data[0] == '$' ? tss_getvar(list, args[0].data + 1) : args[0].data);
        } else if(op == DEF) {
            checkargc(2);
            if(args[0].size == 0 || (args[0].size == 1 && args[0].data[0] == '$')) {
                retret(args[0].pos, 4);
            } if(args[1].size == 0 || (args[1].size == 1 && args[1].data[0] == '$')) {
                retret(args[1].pos, 4);
            }
            tss_setvar(list, args[0].data[0] == '$' ? tss_getvar(list, args[0].data + 1) : args[0].data,
                             args[1].data[0] == '$' ? tss_getvar(list, args[1].data + 1) : args[1].data);
        }

        else { retret(0, 8); }
        freeargs();
    }
    tss_free(&st);
    tss_free(&cst);
    return ret;
}
