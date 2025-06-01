#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

char *tss_codes[] = {
    "no error",              // 0
    "unknown error",         // 1
    "too many args",         // 2
    "not enough args",       // 3
    "wrong args",            // 4
    "stack overflow",        // 5
    "not found label",       // 6
    "call stack overflow",   // 7
    "wrong command",         // 8
    "compile error"          // 9
};

/* void tss_printerr(tss_exception e) {
    if(e.code == 0) return;
    printf("%lu [%d] ", e.symbol, e.code);
    if(e.code < 10) {
        printf("%s\n", tss_code[e.code]);
    } else {
        printf("unknown error\n");
    }
} void tss_printerrv(tss_exception e, char *code, long unsigned int size) {
    if(e.code == 0) return;
    tss_printerr(e);
    size_t symbol = e.symbol;
    size_t prev = 0;
    size_t i = 0;
    while(i < symbol) { i++; }
    while(i != 0 && code[i] != '\n') { i--; }
    if(code[i] == '\n') { i++; }
    prev = i;
    while(i < size && code[i] != '\n') { printf("%c", code[i]); i++; }
    printf("\n");
    i = prev;
    while(i < symbol) { printf(" "); i++; }
    printf("^\n");
}*/
bool tss_strcmp(char *data1, size_t size1, char *data2, size_t size2) {
    if(size1 != size2) { return false; }
    for(size_t i = 0; i < size1; i++) {
        if(data1[i] != data2[i]) return false;
    } return true;
}

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
    } if(a->pos == a->size) {
        a->size += a->size / 2;
        a->data = realloc(a->data, a->size);
    } a->data[a->pos++] = ch;
} char* tss_aget(tss_arg *a) {
    if(a == NULL || a->data == NULL) { return NULL; }
    if(a->pos == a->size) {
        a->size += 1;
        a->data = realloc(a->data, a->size);
    } a->data[a->pos++] = '\0';
    return a->data;
}
