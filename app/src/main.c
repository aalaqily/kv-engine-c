#include <stdio.h>
#include <string.h>
#include <argparse.h>
#include "kv_engine.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

struct cmd_struct {
    const char *cmd;
    int (*fn) (int, const char **);
};

static const char *const cmd_put_usages[] = {
    "kv-engine put --file | -f <file> <key> <value>",
    NULL,
};

int cmd_put(int argc, const char **argv) {
    const char *file = NULL;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING('f', "file", &file, "file to load/save", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, cmd_put_usages, 0);
    argparse_describe(&argparse, "\nPut key-value pair into the database", NULL);
    argc = argparse_parse(&argparse, argc, argv);

    KVEngine *engine = kv_engine_create();
    kv_engine_load(engine, file);

    const char *key = argv[0];
    const char *value = argv[1];

    if(!kv_engine_put(engine, key, value))
        return 1;
    if(!kv_engine_save(engine, file))
        return 1;
    
    kv_engine_destroy(engine);
    
    return 0;
}

static const char *const cmd_get_usages[] = {
    "kv-engine get --file | -f <file> <key>",
    NULL,
};

int cmd_get(int argc, const char **argv) {
    const char *file = NULL;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING('f', "file", &file, "file to load/save", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, cmd_get_usages, 0);
    argparse_describe(&argparse, "\nGet value of key from the database", NULL);
    argc = argparse_parse(&argparse, argc, argv);

    KVEngine *engine = kv_engine_create();
    if(!kv_engine_load(engine, file))
        return 1;

    const char *key = argv[0];
    const char *value = kv_engine_get(engine, key);

    printf("%s:\t%s\n", key, value);

    kv_engine_destroy(engine);

    return 0;
}

static const char *const cmd_rm_usages[] = {
    "kv-engine rm --file | -f <file> <key>",
    NULL,
};

int cmd_rm(int argc, const char **argv) {
    const char *file = NULL;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING('f', "file", &file, "file to load/save", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, cmd_rm_usages, 0);
    argparse_describe(&argparse, "\nRemove key-value pair from the database", NULL);
    argc = argparse_parse(&argparse, argc, argv);

    KVEngine *engine = kv_engine_create();
    if(!kv_engine_load(engine, file))
        return 1;

    const char *key = argv[0];

    if(!kv_engine_remove(engine, key))
        return 1;
    if(!kv_engine_save(engine, file))
        return 1;
    
    kv_engine_destroy(engine);

    return 0;
}

static const char *const cmd_show_usages[] = {
    "kv-engine show --file | -f <file>",
    NULL,
};

int cmd_show(int argc, const char **argv) {
    const char *file = NULL;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING('f', "file", &file, "file to load/save", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, cmd_show_usages, 0);
    argparse_describe(&argparse, "\nShow all key-value pairs from the database", NULL);
    argc = argparse_parse(&argparse, argc, argv);

    KVEngine *engine = kv_engine_create();
    if(!kv_engine_load(engine, file))
        return 1;

    KVIterator *iter = kv_iter_create(engine);
    if(!iter) {
        kv_engine_destroy(engine);
        return 1;
    }
    
    while(kv_iter_next(iter))
        printf("%s:\t%s\n", kv_iter_current_key(iter), kv_iter_current_value(iter));

    kv_engine_destroy(engine);
    kv_iter_destroy(iter);

    return 0;
}

static const char *const cmd_contains_usages[] = {
    "kv-engine contains --file | -f <file> <key>",
    NULL,
};

int cmd_contains(int argc, const char **argv) {
    const char *file = NULL;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING('f', "file", &file, "file to load/save", NULL, 0, 0),
        OPT_END(),
    };
    struct argparse argparse;
    argparse_init(&argparse, options, cmd_contains_usages, 0);
    argparse_describe(&argparse, "\nCheck if key is in the database", NULL);
    argc = argparse_parse(&argparse, argc, argv);

    KVEngine *engine = kv_engine_create();
    if(!kv_engine_load(engine, file))
        return 1;

    const char *key = argv[0];

    int result = kv_engine_contains(engine, key) ? 0 : 1;

    kv_engine_destroy(engine);

    return result;
}

static struct cmd_struct commands[] = {
    {"put",  cmd_put},
    {"get",  cmd_get},
    {"rm",   cmd_rm},
    {"show", cmd_show},
    {"contains", cmd_contains},
};

static const char *const usages[] = {
    "kv-engine [options] [cmd] [args]\n"
    "Available subcommands:\n"
    "\n"
    "\tput\n"
    "\tget\n"
    "\trm\n"
    "\tshow\n"
    "\tcontains\n"
    "\n"
    "Available options:",
    NULL,
};

int main(int argc, const char **argv) {
    struct argparse argparse;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_END(),
    };
    argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
    argc = argparse_parse(&argparse, argc, argv);
    if (argc < 1) {
        argparse_usage(&argparse);
        return -1;
    }

    struct cmd_struct *cmd = NULL;
    for (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
        if (!strcmp(commands[i].cmd, argv[0])) {
            cmd = &commands[i];
        }
    }
    if (cmd) {
        return cmd->fn(argc, argv);
    }

    return 0;
}
