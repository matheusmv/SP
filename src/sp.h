#ifndef SP_H
#define SP_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SP_SUCCESS 0
#define SP_FAILURE 1
#define CONFLINE   64

typedef void (*properties_handler)(void *object, const char *key, char *value);

typedef struct SP_Module {
        void               *object;
        char               *name;
        properties_handler phandler;
} SP_Module;


int SP_parse_config(FILE *conf_file, SP_Module modules[], size_t nmodules);
bool key_eq_value(const char *key, const char *value, size_t len_value);

#endif /* SP_H */
