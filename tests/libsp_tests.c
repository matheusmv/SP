#include "minunit.h"
#include "../src/sp.h"

#include <string.h>
#include <errno.h>

#define ARRLEN(arr) (sizeof(arr) / sizeof(arr)[0])
#define BUFFER 64

typedef struct User {
        char  username[BUFFER];
        char  password[BUFFER];
        char  email[BUFFER];
        bool  authenticated;
        float points;
} User;

typedef struct Network {
        char interface[BUFFER];
        char inet[BUFFER];
        char brd[BUFFER];
} Network;

static void
user_phandler(void *object, const char *key, char *value)
{
        struct User *user = object;

        if (key_eq_value(key, "username", strlen("username"))) {
                memmove(user->username, value, strlen(value));
        }

        if (key_eq_value(key, "password", strlen("password"))) {
                memmove(user->password, value, strlen(value));
        }

        if (key_eq_value(key, "email", strlen("email"))) {
                memmove(user->email, value, strlen(value));
        }

        if (key_eq_value(key, "authenticated", strlen("authenticated"))) {
                if (key_eq_value(value, "true", strlen("true"))) {
                        user->authenticated = true;
                } else {
                        user->authenticated = false;
                }
        }

        if (key_eq_value(key, "points", strlen("points"))) {
                user->points = (float) atof(value);
        }
}

static void
network_phandler(void *object, const char *key, char *value)
{
        struct Network *network = object;

        if (key_eq_value(key, "interface", strlen("interface"))) {
                memmove(network->interface, value, strlen(value));
        }

        if (key_eq_value(key, "inet", strlen("inet"))) {
                memmove(network->inet, value, strlen(value));
        }

        if (key_eq_value(key, "brd", strlen("brd"))) {
                memmove(network->brd, value, strlen(value));
        }
}

static char *
test_parse_user_and_network(void)
{
        User newUser      = { 0 },
             expectedUser = {
                     "First Name Last Name",
                     "12346",
                     "user@email.com",
                     true,
                     365.25
             };

        Network newNetwork = { 0 },
                expectedNetwork = {
                        "enp25s0",
                        "192.168.1.25",
                        "192.168.1.255"
                };

        SP_Module modules[] = {
                { &newUser, "[User]", user_phandler },
                { &newNetwork, "[Network]", network_phandler }
        };

        FILE *conf_file = NULL;
        conf_file = fopen("tests/test.conf", "r");
        if (conf_file == NULL) {
                return strerror(errno);
        }

        SP_parse_config(conf_file, modules, ARRLEN(modules));

        fclose(conf_file);

        mu_assert(strcmp(newUser.username, expectedUser.username) == 0, "username not match.");
        mu_assert(strcmp(newUser.password, expectedUser.password) == 0, "password not match.");
        mu_assert(strcmp(newUser.email, expectedUser.email) == 0, "email not match.");
        mu_assert(newUser.authenticated == expectedUser.authenticated, "authenticated not match.");
        mu_assert(newUser.points == expectedUser.points, "points not match.");

        mu_assert(strcmp(newNetwork.interface, expectedNetwork.interface) == 0, "interface not match.");
        mu_assert(strcmp(newNetwork.inet, expectedNetwork.inet) == 0, "inet not match.");
        mu_assert(strcmp(newNetwork.brd, expectedNetwork.brd) == 0, "brd not match.");

        return NULL;
}

static char *
all_tests(void)
{
        mu_suite_start();

        mu_run_test(test_parse_user_and_network);

        return NULL;
}

RUN_TESTS(all_tests)
