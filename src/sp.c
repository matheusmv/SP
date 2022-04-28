#include "sp.h"

#include <ctype.h>
#include <string.h>

#define BLANK   ' '
#define NEWLINE '\n'
#define COMMENT '#'
#define MODULE  '['

static void
ltrim(char *src)
{
        char *dst;

        for (dst=src; *src == BLANK; src++) {;}

        if (dst==src) return;

        while ((*dst++ = *src++)) {;}
}

static int
extract_key_value(const char *src, const char *token, char *dest_key, char *dest_value)
{
        char *src_dup = strdup(src);

        if (src_dup != NULL) {
                char *rest = NULL;

                char *key = strtok_r(src_dup, token, &rest);
                char *value = strtok_r(NULL, token, &rest);

                if (key != NULL && value != NULL) {
                        memmove(dest_key, key, strlen(key));
                        memmove(dest_value, value, strlen(value) - 1);
                }

                free(src_dup);

                return SP_SUCCESS;
        }

        return SP_FAILURE;
}

bool
key_eq_value(const char *key, const char *value, size_t len_value)
{
        return strncmp(key, value, len_value) == 0;
}

int
SP_parse_config(FILE *conf_file, SP_Module modules[], size_t nmodules)
{
        void *object = NULL;
        properties_handler phandler = NULL;

        char conf_line[CONFLINE];
        while (fgets(conf_line, CONFLINE, conf_file) != NULL) {
                ltrim(conf_line);

                switch (conf_line[0]) {
                case NEWLINE: break;
                case COMMENT: break;
                case MODULE:

                        for (size_t mod = 0; mod < nmodules; ++mod) {
                                char *mod_name = modules[mod].name;
                                size_t mod_name_len = strlen(modules[mod].name);

                                if (key_eq_value(conf_line, mod_name, mod_name_len)) {
                                        object = modules[mod].object;
                                        phandler = modules[mod].phandler;
                                        break;
                                }
                        }

                        break;
                default:

                        char key[CONFLINE]   = { 0 },
                             value[CONFLINE] = { 0 };

                        int error = 0;
                        error = extract_key_value(conf_line, "=", key, value);
                        if (error) {
                                return SP_FAILURE;
                        }
                        
                        phandler(object, key, value);
                }
        }

        return SP_SUCCESS;
}
