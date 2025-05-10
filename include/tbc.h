#ifndef _TINYSS_BYTECODE_H_
#define _TINYSS_BYTECODE_H_
#include "tsf.h"

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
    SUB   = 19
} tbc_opcode;

tsf_file tbc_compile(char *code, unsigned long int size);
tsf_file tbc_compileFile(char *path);

#endif
