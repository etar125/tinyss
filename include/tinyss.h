#ifndef _TINYSS_H_
#define _TINYSS_H_
#include <stdbool.h>

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
    unsigned long int size, pos;
} tss_arg;

typedef struct {
    unsigned char nsize;
    char *name;
    unsigned long int pos;
} tsf_func;

typedef struct {
    unsigned short tsize;
    tsf_func *table;
    unsigned long int csize;
    char *code;
} tsf_file;

typedef enum {
    NUL   = 0,
    DEF   = 1,
    DEL   = 2,
    OP    = 3,
    IF    = 4,
    ELIF  = 5,
    ELSE  = 6,
    END   = 7,
    CALL  = 8,
    FCALL = 9,
    GCALL = 10,
    PUSH  = 11,
    POP   = 12,
    GOTO  = 13,
    RET   = 14,
    EXIT  = 15,
    ADD   = 16,
    INS   = 17,
    LEN   = 18,
    SUB   = 19,
    NOP   = 20
} tbc_opcode;

void tsf_write(tsf_file *f, char *path);
tsf_file tsf_read(char *path);
void tsf_free(tsf_file *f);

tsf_file tbc_compile(char *code, unsigned long int size);
tsf_file tbc_compileFile(char *path);

void tss_ainit(tss_arg *a);
void tss_aadd(tss_arg *a, char ch);
char* tss_aget(tss_arg *a);

bool tss_strcmp(char *data1, unsigned long int size1, char *data2, unsigned long int size2);

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
tss_exception tss_docode(tss_varlist *list, tsf_file *file);

void tss_printerr(tss_exception e);
void tss_printerrv(tss_exception e, tsf_file *f);

#endif
