#include <stdio.h>
#include "kv_engine.h"

int main(void) {
    KVEngine *engine = kv_engine_create();
    kv_engine_put(engine, "First Name", "Gottfried");
    kv_engine_put(engine, "Middle Name", "Wilhelm");
    kv_engine_put(engine, "Last Name", "Leibniz");
    kv_engine_put(engine, "Birth Year", "1646");
    kv_engine_put(engine, "Death Year", "1716");
    kv_engine_put(engine, "Birth Place", "Leipzig");
    kv_engine_put(engine, "Death Place", "Hanover");

    KVIterator *iter = kv_iter_create(engine);
    while(kv_iter_next(iter))
        printf("%s:\t\t%s\n", kv_iter_current_key(iter), kv_iter_current_value(iter));

    kv_engine_destroy(engine);
    kv_iter_destroy(iter);

    return 0;
}
