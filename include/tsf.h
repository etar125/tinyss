#ifndef _TINYSS_FILE_H_
#define _TINYSS_FILE_H_

typedef struct {
    unsigned short nsize;
    char *name;
    unsigned long int pos;
} tsf_func;

typedef struct {
    unsigned int tsize;
    tsf_func *table;
    unsigned long int csize;
    char *code;
} tsf_file;

void tsf_write(tsf_file *f, char *path);
tsf_file tsf_read(char *path);
void tsf_free(tsf_file *f);

#endif
