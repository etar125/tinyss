#ifndef _TINYSS_H_
#define _TINYSS_H_

typedef struct {
    unsigned long int symbol;
    short code;
} tss_exception;

typedef struct {
    char *name;
    char *value;
} tss_var;

typedef struct {
    tss_var *list;
    long unsigned int size;
} tss_varlist;

typedef struct {
    char *data[8];
    unsigned char sp;
} tss_stack;

typedef struct {
    char *data;
    unsigned long int size, pos, cpos;
} tss_arg;
void tss_ainit(tss_arg *a);
void tss_aadd(tss_arg *a, char ch);
char* tss_aget(tss_arg *a);

void tss_sinit(tss_stack *stack);
char* tss_pop(tss_stack *stack);
void tss_push(tss_stack *stack, char *data);
void tss_free(tss_stack *stack);

void tss_vlinit(tss_varlist *list);
void tss_vlapp(tss_varlist *list);
void tss_vlfree(tss_varlist *list);

long unsigned int tss_findvar(tss_varlist *list, char *name);
void tss_setvar(tss_varlist *list, char *name, char *val);
char* tss_getvar(tss_varlist *list, char *name);
void tss_delvar(tss_varlist *list, char *name);

void tss_gfunc(tss_varlist *list, tss_stack *stack, char *name);
tss_exception tss_docode(tss_varlist *list, char *code, long unsigned int size);

void tss_printerr(tss_exception e);
void tss_printerrv(tss_exception e, char *code, long unsigned int size);

#endif
