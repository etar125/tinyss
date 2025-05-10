#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "tsf.h"

void tsf_write(tsf_file *f, char *path) {
    FILE* file = fopen(path, "wb");
    if(!file) {
        perror("Failed to open file for writing");
        return;
    }
    fwrite(&f->tsize, sizeof(unsigned int), 1, file);
    for (size_t i = 0; i < f->tsize; i++) {
        tsf_func *func = &f->table[i];
        fwrite(&func->nsize, sizeof(unsigned short), 1, file);
        fwrite(func->name, sizeof(char), func->nsize, file);
        fwrite(&func->pos, sizeof(size_t), 1, file);
    }
    fwrite(&f->csize, sizeof(size_t), 1, file);
    fwrite(f->code, sizeof(char), f->csize, file);
    fclose(file);
}

tsf_file tsf_read(char *path) {
    FILE* file = fopen(path, "rb");
    if(!file) {
        perror("Failed to open file for reading");
        exit(EXIT_FAILURE);
    }

    tsf_file f;

    fread(&f.tsize, sizeof(unsigned int), 1, file);
    f.table = malloc(f.tsize * sizeof(tsf_func));
    for(size_t i = 0; i < f.tsize; i++) {
        tsf_func *func = &f.table[i];
        fread(&func->nsize, sizeof(unsigned short), 1, file);
        func->name = malloc(func->nsize + 1);
        fread(func->name, sizeof(char), func->nsize, file);
        func->name[func->nsize] = '\0';
        fread(&func->pos, sizeof(size_t), 1, file);
    }
    fread(&f.csize, sizeof(size_t), 1, file);
    f.code = malloc(f.csize + 1);
    fread(f.code, sizeof(char), f.csize, file);
    f.code[f.csize] = '\0';

    fclose(file);
    return f;
}

void tsf_free(tsf_file *f) {
    free(f->code);
    for(size_t i = 0; i < f->tsize; i++) {
        free(f->table[i].name);
    }
    free(f->table);
}
