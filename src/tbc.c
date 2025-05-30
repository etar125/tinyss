#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include "tinyss.h"

inline char schar(char ch) {
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

/*char opcodes[21][7] = {
  "nul",
  "define",
  "delete",
  "op",
  "if",
  "elif",
  "else",
  "end",
  "call",
  "fcall",
  "gcall",
  "push",
  "pop",
  "goto",
  "ret",
  "exit",
  "stradd",
  "strins",
  "strlen",
  "substr",
  "nop"
};*/

tsf_file tbc_compile(char *code, unsigned long int size) {
    tsf_file ret;
    ret.tsize = 0;
    ret.table = NULL;
    ret.csize = 0;
    ret.code = NULL;
    if(code == NULL || size == 0) { return ret; }
    uint8_t argc = 0;
    uint8_t opcode = 0;
    tss_arg args[5];
    for(int8_t i = 0; i < 5; i++) {
        args[i].data = NULL;
        tss_ainit(&args[i]);
    }
    tsf_func *table = malloc(sizeof(tsf_func) * 1);
    ret.table = table;
    int16_t tsize = 1, tp = 0;
    bool com = false;
    char *bin = malloc(32), *new, *arg;
    size_t allocsize = 32, newsize = 0, bini = 0, psize = 0;

    void write(char *data, uint8_t size) {
        if(bini + size > allocsize) {
            newsize = allocsize + size;
            new = malloc(newsize);
            memcpy(new, bin, allocsize);
            allocsize = newsize;
            free(bin);
            bin = new;
            new = NULL, newsize = 0;
        }
        memcpy(bin + bini, data, size);
        bini += size;
    }
    
    if(bin == NULL) { ret.csize = 1; return ret; }
    for(size_t i = 0; i <= size; i++) {
        if(i == size || code[i] == '\0' || code[i] == '\n') {
            if(argc == 0 && args[0].data == NULL) { continue; }
            arg = tss_aget(&args[0]);
            psize = strlen(arg);
            opcode = 0;
                 if(tss_strcmp(arg, psize, "nul", 3)) { opcode = 0; }
            else if(tss_strcmp(arg, psize, "define", 6)) { opcode = 1; }
            else if(tss_strcmp(arg, psize, "delete", 6)) { opcode = 2; }
            else if(tss_strcmp(arg, psize, "op", 2)) { opcode = 3; }
            else if(tss_strcmp(arg, psize, "if", 2)) { opcode = 4; }
            else if(tss_strcmp(arg, psize, "elif", 4)) { opcode = 5; }
            else if(tss_strcmp(arg, psize, "else", 4)) { opcode = 6; }
            else if(tss_strcmp(arg, psize, "end", 3)) { opcode = 7; }
            else if(tss_strcmp(arg, psize, "call", 4)) { opcode = 8; }
            else if(tss_strcmp(arg, psize, "gcall", 5)) { opcode = 10; }
            else if(tss_strcmp(arg, psize, "gpushb", 6)) { opcode = 11; }
            else if(tss_strcmp(arg, psize, "goto", 4)) { opcode = 13; }
            else if(tss_strcmp(arg, psize, "ret", 3)) { opcode = 14; }
            else if(tss_strcmp(arg, psize, "exit", 4)) { opcode = 15; }
            else if(tss_strcmp(arg, psize, "stradd", 6)) { opcode = 16; }
            else if(tss_strcmp(arg, psize, "strins", 6)) { opcode = 17; }
            else if(tss_strcmp(arg, psize, "strlen", 6)) { opcode = 18; }
            else if(tss_strcmp(arg, psize, "substr", 6)) { opcode = 19; }
            else if(tss_strcmp(arg, psize, "nop", 3)) { opcode = 20; }
            /* данный код хуже того, что выше, ведь я не могу нормально
               сделать несколько вариантов написаний для одного и того же опкода
            for(char i = 0; i < 21; i++) {
                if(tss_strcmp(arg, psize, opcodes[i], strlen(opcodes[i]))) { opcode = i; break; }
            }*/
            write((char*)&opcode, 1);

            write((char*)&argc, 1);

            for(uint8_t i = 1; i < argc + 1; i++) {
                arg = tss_aget(&args[i]);
                psize = strlen(arg);
                if(psize > 255) { ret.csize = 5; return ret; }
                opcode = (uint8_t)psize;
                write((char*)&opcode, 1);
                write(arg, psize);
            }
            
            for(uint8_t i = 0; i < argc + 1; i++) {
                if(args[i].data != NULL) { free(args[i].data); }
                tss_ainit(&args[i]);
            } argc = 0;
            args[0].cpos = i + 1;
        } else if((code[i] == '"' || code[i] == '\'')) {
            if(com) { com = false; }
            else { com = true; }
        } else if(code[i] == ' ' || code[i] == '\t') {
            if(argc == 0 && args[0].data == NULL) { args[0].cpos++; }
            else if(!com) {
                if(argc < 5) {
                    argc++;
                    args[argc].cpos = i + 1;
                } else { ret.csize = 2; return ret;  }
            } else { tss_aadd(&args[argc], code[i]); }
        } else if((code[i] == ';' || code[i] == '#') && argc == 0 && args[0].data == NULL) {
            while(i < size && code[i] != '\0' && code[i] != '\n') { i++; } args[0].cpos = i + 1;
        } else if(code[i] == ':' && argc == 0 && args[0].data == NULL) {
            i++;
            psize = i;
            while(i < size && code[i] != '\0' && code[i] != '\n') { i++; } args[0].cpos = i + 1;
            psize = i - psize;
            if(psize > 255) { ret.csize = 3; return ret; }
            arg = malloc(psize + 1);
            if(arg == NULL) { ret.csize = 4; return ret; }
            memcpy(arg, &code[i - psize], psize);
            arg[psize] = '\0';
            if(tp == tsize) {
                tsf_func *newt = malloc(tsize + 1);
                memcpy(newt, table, tsize);
                free(table);
                table = newt;
                tsize += 1;
            }
            table[tp].name = arg;
            table[tp].nsize = (uint8_t)psize;
            table[tp].pos = bini;
            tp++;
        } else if(code[i] == '\\') { tss_aadd(&args[argc], schar(code[++i])); }
        else { tss_aadd(&args[argc], code[i]); }
    }
    ret.tsize = tsize;
    if(allocsize > bini) {
        new = malloc(bini);
        memcpy(new, bin, bini);
        allocsize = bini;
    } else { ret.code = bin; }
    ret.csize = allocsize;
    return ret;
}
