#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tinyss.h"

void tss_sinit(tss_stack *stack) {
    if(stack == NULL) { return; }
    for(uint8_t i = 0; i < 8; i++) {
        stack->data[i] = NULL;
    }
    stack->sp = 8;
}

char* tss_pop(tss_stack *stack) {
    if(stack == NULL) { return NULL; }
    if(stack->sp != 8) {
        return stack->data[stack->sp++];
    } return NULL;
}

void tss_push(tss_stack *stack, char *data) {
    if(stack == NULL || data == NULL) { return; }
    if(stack->sp != 0) {
        size_t sz = strlen(data);
        char *new = malloc(sz + 1);
        memcpy(new, data, sz);
        new[sz] = '\0';
        if(stack->data[--stack->sp] != NULL) { free(stack->data[stack->sp]); }
        stack->data[stack->sp] = new;
        return;
    }
}

void tss_free(tss_stack *stack) {
    if(stack == NULL) { return; }
    for(unsigned char i = 0; i < 8; i++) {
        if(stack->data[i] != NULL) { free(stack->data[i]); }
    } stack->sp = 8;
}
