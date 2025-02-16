#include "tinyss.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>

#define _retset ret.line = line
#define freeargs() for(uint8_t i = 0; i < argc + 1; i++) {\
                if(args[i].data != NULL) { free(args[i].data); }\
            }
#define retret(a, b) _retset; ret.symbol = a; ret.code = b; freeargs(); return ret
#define checkargc(e) if(argc != e) { retret(args[0].cpos, argc < e ? 3 : 2); }

#define labelfind(e) \
bool started = false;\
int pos = 0;\
for(size_t a = 0; a <= size; a++) {\
    if(e[pos] == '\0') {\
        if(a == size - 1 || code[a] == '\n') {\
            while(code[a] != '\n' && a < size) {\
                a++;\
            } i = a;\
            break;\
        } while(code[a] != '\n' && a < size) {\
            a++;\
        } pos = 0;\
        started = false;\
    }\
    if(code[a] == '\n' || a == size) {\
        started = false;\
        pos = 0;\
        continue;\
    }\
    if(!started) {\
        if(code[a] == ' ' || code[a] == '\t') { continue; }\
        else if(code[a] != ':') {\
            while(code[a] != '\n' && a < size) { a++; } pos = 0;\
        } else { started = true; }\
        continue;\
    } if(code[a] == e[pos]) { pos++; }\
    else {\
        while(code[a] != '\n' && a < size) { a++; }\
        pos = 0, started = false;\
    }\
}\
if(pos == 0) {\
    retret(args[1].cpos, 6);\
}

bool startswith(char *str, size_t at, size_t size, char *with);

#define iffind() \
int lvl = 0;\
bool started = false;\
for(size_t a = i + 1; a <= size; a++) {\
    if(lvl != 0) {\
        if(startswith(code, a, size, "if")) { lvl++; }\
        else if(startswith(code, a, size, "end")) { lvl--; }\
        while(code[a] != '\n' && a < size) { a++; }\
        continue;\
    }\
    if(!started) {\
        if(code[a] == ' ' || code[a] == '\t') { continue; }\
        started = true;\
    } if(startswith(code, a, size, "if")) {\
        lvl++;\
        while(code[a] != '\n' && a < size) { a++; }\
        continue;\
    } else if(startswith(code, a, size, "elif") || startswith(code, a, size, "end")\
           || startswith(code, a, size, "else")) {\
        i = a - 1;\
        break;\
    } else {\
        started = false;\
        while(code[a] != '\n' && a < size) { a++; }\
    }\
}

#define iffindeo() \
int lvl = 0;\
bool started = false;\
for(size_t a = i + 1; a <= size; a++) {\
    if(lvl != 0) {\
        if(startswith(code, a, size, "if")) { lvl++; }\
        else if(startswith(code, a, size, "end")) { lvl--; }\
        while(code[a] != '\n' && a < size) { a++; }\
        continue;\
    }\
    if(!started) {\
        if(code[a] == ' ' || code[a] == '\t') { continue; }\
        started = true;\
    } if(startswith(code, a, size, "if")) {\
        lvl++;\
        while(code[a] != '\n' && a < size) { a++; }\
        continue;\
    } else if(startswith(code, a, size, "end")) {\
        i = a - 1;\
        break;\
    } else {\
        started = false;\
        while(code[a] != '\n' && a < size) { a++; }\
    }\
}

bool tss_ie(char *data, size_t size);
bool tss_strcmp(char *data1, size_t size1, char *data2, size_t size2);

char schar(char ch);

typedef struct {
    char *data;
    size_t size, pos, cpos;
} tss_arg;
void tss_ainit(tss_arg *a);
void tss_aadd(tss_arg *a, char ch);
char* tss_aget(tss_arg *a);

char* itoa(int num, char* str, int base);
int _pow(int i, int n);

tss_exception tss_docode(tss_varlist *list, char *code, size_t size) {
    uint8_t argc = 0, iflvl = 0;
    size_t i, ri = 0, line = 0, psize;
    char *arg0, *arg1, *arg2, *arg3, *arg4,
         *tmp1, *tmp2;
    tss_arg args[5];
    for(uint8_t i = 0; i < 5; i++) {
        args[i].data = NULL;
        tss_ainit(&args[i]);
    }
    bool com = false;
    tss_stack st, cst;
    tss_sinit(&st);
    tss_sinit(&cst);
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
                    retret(args[1].cpos, 4);
                } if(st.sp == 0) {
                    retret(args[0].cpos, 5);
                } tss_push(&st, arg1[0] == '$' ? tss_getvar(list, ++arg1) : arg1);
            } else if(tss_strcmp(arg0, psize, "gcall", 5)) {
                checkargc(1);
                arg1 = tss_aget(&args[1]);
                if(strlen(arg1) == 0 || arg1[0] == '$') {
                    retret(args[1].cpos, 4);
                }
                tss_gfunc(list, &st, arg1);
            } else if(tss_strcmp(arg0, psize, "define", 6)) {
                checkargc(2);
                arg1 = tss_aget(&args[1]);
                arg2 = tss_aget(&args[2]);
                if(strlen(arg1) == 0 || arg1[0] == '$') {
                    retret(args[1].cpos, 4);
                } if(strlen(arg2) == 0) {
                    retret(args[2].cpos, 4);
                } tss_setvar(list, arg1, arg2[0] == '$' ? tss_getvar(list, ++arg2) : arg2);
            } else if(tss_strcmp(arg0, psize, "del", 3)) {
                checkargc(1);
                tss_delvar(list, tss_aget(&args[1]));
            } else if(tss_strcmp(arg0, psize, "op", 2)) {
                checkargc(3);
                arg1  = tss_aget(&args[1]);
                arg2  = tss_aget(&args[2]);
                arg3  = tss_aget(&args[3]);
                arg3  = arg3[0] == '$' ? tss_getvar(list, ++arg3) : arg3;
                if(strlen(arg1) == 0) { retret(args[1].cpos, 4); }
                if(arg3 == NULL) { retret(args[3].cpos, 4); }
                if(strlen(arg2) != 1) { retret(args[2].cpos, 4); }
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
                        retret(args[2].cpos, 4);
                        break;
                } free(tmp1);
            } else if(tss_strcmp(arg0, psize, "goto", 4)) {
                checkargc(1);
                arg1 = tss_aget(&args[1]);
                if(arg1[0] == '$') { retret(args[1].cpos, 4); }
                labelfind(arg1);
            } else if(tss_strcmp(arg0, psize, "call", 4)) {
                checkargc(1);
                arg1 = tss_aget(&args[1]);
                if(arg1[0] == '$') { retret(args[1].cpos, 4); }
                if(cst.sp == 0) { retret(args[0].cpos, 7); }
                while(code[i] != '\n' && i < size) { i++; }
                tmp1 = malloc(13);
                tss_push(&cst, itoa(i, tmp1, 10));
                free(tmp1);
                labelfind(arg1);
            } else if(tss_strcmp(arg0, psize, "ret", 3)) {
                tmp1 = tss_pop(&cst);
                if(tmp1 != NULL) {
                    i = atoi(tmp1);
                    free(tmp1);
                }
            } else if(tss_strcmp(arg0, psize, "if", 2) || tss_strcmp(arg0, psize, "elif", 4)) {
                if(psize == 4 && iflvl) {
                    iflvl--;
                    iffindeo();
                } else {
                    checkargc(3);
                    arg1 = tss_aget(&args[1]);
                    arg2 = tss_aget(&args[2]);
                    arg3 = tss_aget(&args[3]);
                    arg1 = tss_getvar(list, arg1);
                    arg3 = arg3[0] == '$' ? tss_getvar(list, arg3) : arg3;
                    if(arg1 == NULL) { retret(args[1].cpos, 4); }
                    if(arg2[0] == '\0') { retret(args[2].cpos, 4); }
                    if(arg3 == NULL) { retret(args[3].cpos, 4);  }
                    bool not = false, equ = false, tru = false;
                    if(arg2[0] == 'n') {
                        arg2++;
                        not = true;
                    } if(arg2[1] == 'e') { equ = true; }
                    int a, b;
                    if(arg2[0] == '\0') { retret(args[2].cpos, 4); }
                    if(arg2[0] == 'e') { tru = !strcmp(arg1, arg3); }
                    else if(arg2[0] == 'l')  {
                        a = atoi(arg1);
                        b = atoi(arg3);
                        tru = equ ? a <= b : a < b;
                    } else if(arg2[0] == 'g')  {
                        a = atoi(arg1);
                        b = atoi(arg3);
                        tru = equ ? a >= b : a > b;
                    }
                    if(not) { tru = !(tru); }
                    if(tru) { iflvl++; }
                    else { iffind(); }
                }
            } else if(tss_strcmp(arg0, psize, "end", 3) || tss_strcmp(arg0, psize, "else", 4)) {
                if(iflvl) {
                    iflvl--;
                    iffindeo();
                }
            } else if(tss_strcmp(arg0, psize, "exit", 4)) {
                _retset;
                ret.symbol = args[0].cpos;
                if(argc > 1) { ret.code = 2; }
                if(argc == 1) {
                    arg1 = tss_aget(&args[1]);
                    ret.code = (short)atoi(arg1);
                } return ret;
            } else { retret(0, 8); }
            
            for(uint8_t i = 0; i < argc + 1; i++) {
                if(args[i].data != NULL) { free(args[i].data); }
                tss_ainit(&args[i]);
            } ri = 0, argc = 0;
        } else if((code[i] == '"' || code[i] == '\'')) {
            if(com) { com = false; }
            else { com = true; }
        } else if(code[i] == ' ' || code[i] == '\t') {
            if(argc == 0 && args[0].data == NULL) { args[0].cpos++; }
            else if(!com) {
                if(argc < 5) {
                    argc++;
                    args[argc].cpos = ri + 1;
                } else { retret(0, 2); }
            } else { tss_aadd(&args[argc], code[i]); }
        } else if((code[i] == ':' || code[i] == ';' || code[i] == '#') && argc == 0 && args[0].data == NULL) {
            while(i < size && code[i] != '\n') { i++; }
            continue;
        } else if(code[i] == '\\') { tss_aadd(&args[argc], schar(code[++i])); }
        else { tss_aadd(&args[argc], code[i]); }
    } return ret;
}

char *tss_code[] = {
    "no error", // 0
    "test error?", // 1
    "too many args", // 2
    "not enough args", // 3
    "wrong args", // 4
    "stack overflow", // 5
    "not found label", // 6
    "call stack overflow", // 7
    "wrong command" // 8
};

void tss_printerr(tss_exception e) {
    if(e.code == 0) return;
    printf("%d:%d [%d] ", e.line, e.symbol, e.code);
    if(e.code < 9) {
        printf("%s\n", tss_code[e.code]);
    } else {
        printf("unknown error\n");
    }
} void tss_printerrv(tss_exception e, char *code, long unsigned int size) {
    tss_printerr(e);
    if(e.code == 0) return;
    int line = e.line, symbol = e.symbol, ri = 0, cur = 1;
    bool printed = false;
    size_t prev = 0;
    for(size_t i = 0; i < size; i++, ri++) {
        if(cur != line) {
            while(code[i] != '\n' && i < size) { i++; }
            ri = 0, cur++;
            continue;
        } if(!printed) {
            prev = i, printed = true;
            while(code[i] != '\n' && i < size) {
                printf("%c", code[i++]);
            } printf("\n");
            i = prev - 1;
        } while(ri < symbol) {
            printf(" "); ri++;
        } printf("^\n"); break;
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

void tss_ainit(tss_arg *a) {
    if(a == NULL) { return; }
    a->data = NULL;
    a->pos = 0;
    a->cpos = 0;
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

bool startswith(char *str, size_t at, size_t size, char *with) {
    int pos = 0;
    for(; at <= size; at++) {
        if(with[pos] == '\0') { return true; }
        if(at == size || str[at] == '\n') { return false; }
        if(str[at] == with[pos]) { pos++; }
        else { return false; }
    } return false;
}
