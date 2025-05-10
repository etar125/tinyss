#ifndef _TINYSS_FILE_H_
#define _TINYSS_FILE_H_

typedef struct {
    char *name;
    int pos;
} tsf_func;

typedef struct {
    int tsize;
    tsf_func *table;
    unsigned long int csize;
    char *code;
} tsf_file;

void tsf_write(tsf_file *f, char *path);
tsf_file tsf_read(char *path);

#endif
