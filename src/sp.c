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

        for (dst = src; *src == BLANK; ++src);

        if (dst == src) {
                return;
        }

        while ((*dst++ = *src++));
}

static char *
trim(char *str)
{
        if (str == NULL) {
                return "";
        }

        char *end;

        while(isspace((unsigned char)*str)) str++;

        if(*str == 0) {
                return "";
        }

        end = str + strlen(str) - 1;
        while(end > str && isspace((unsigned char)*end)) end--;

        end[1] = '\0';

        return str;
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

                        sscanf(conf_line, "%[^=]=%[^\n]", key, value);

                        phandler(object, trim(key), trim(value));
                }
        }

        return SP_SUCCESS;
}
