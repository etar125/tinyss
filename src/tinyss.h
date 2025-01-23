#ifndef _TINYSS_H_
#define _TINYSS_H_

#include <stdio.h>

typedef struct {
    int line;
    int symbol;
    short code;
} tss_exception;

typedef struct {
    char *name;
    char *value;
} tss_var;

typedef struct {
    tss_var **list;
    size_t size;
} tss_varlist;

void tss_vlapp(tss_varlist *list);

size_t tss_findvar(tss_varlist *list, char *name);
void tss_setvar(tss_varlist *list, char *name, char *val);
char* tss_getvar(tss_varlist *list, char *name);
void tss_delvar(tss_varlist *list, char *name);

void tss_gfunc(tss_varlist *list, char *name);
tss_exception tss_docode(char *code, size_t size);

void tss_printerr(tss_exception *e);

#endif
