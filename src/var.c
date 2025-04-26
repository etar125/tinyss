#include "tinyss.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

void tss_vlinit(tss_varlist *list) {
    if(list == NULL) { return; }
    list->size = 0;
    list->list = NULL;
}

void tss_vlapp(tss_varlist *list) {
    if(list == NULL) { return; }
    if(list->list == NULL) {
        list->size = 1;
        list->list = malloc(sizeof(tss_var));
        list->list[0].name = NULL;
        return;
    }
    
    /* change size */
    if(list->size <= 4) {
        list->size *= 4;
    } else if(list->size <= 32) {
        list->size *= 2;
    } else {
        list->size += list->size / 2;
    }
    
    list->list = realloc(list->list, sizeof(tss_var) * list->size);
}

void tss_vlfree(tss_varlist *list) {
    if(list == NULL || list->list == NULL) { return; }
    for(size_t i = 0; i < list->size; i++) {
        if(list->list[i].name != NULL) {
            free(list->list[i].name);
            free(list->list[i].value);
        }
    } free(list->list);
}

size_t tss_findvar(tss_varlist *list, char *name) {
    for(size_t i = 0; i < list->size; i++) {
        if(list->list[i].name != NULL &&
           strcmp(list->list[i].name, name) == 0) { return i; }
    } return (size_t)-1;
}

void tss_setvar(tss_varlist *list, char *name, char *val) {
    size_t i = tss_findvar(list, name);
    if(i != (size_t)-1) {
        size_t len = strlen(val) + 1;
        free(list->list[i].value);
        list->list[i].value = malloc(sizeof(char) * len);
        memcpy(list->list[i].value, val, len);
        
    } else {
        while(1) {
            for(size_t i = 0; i < list->size; i++) {
                if(list->list[i].name == NULL) {
                    size_t nsize = strlen(name),
                           vsize = strlen(val);
                    list->list[i].name = malloc(nsize + 1);
                    strcpy(list->list[i].name, name);
                    list->list[i].name[nsize] = '\0';
                    list->list[i].value = malloc(vsize + 1);
                    strcpy(list->list[i].value, val);
                    list->list[i].value[vsize] = '\0';
                    return;
                }
            } tss_vlapp(list);
        }
    }
}
char* tss_getvar(tss_varlist *list, char *name) {
    size_t i = tss_findvar(list, name);
    if(i != (size_t)-1) {
        return list->list[i].value;
    } return NULL;
}
void tss_delvar(tss_varlist *list, char *name) {
    size_t i = tss_findvar(list, name);
    if(i != (size_t)-1) {
        free(list->list[i].value);
        free(list->list[i].name);
    }
}
