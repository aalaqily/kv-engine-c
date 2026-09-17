# kv-engine-c

Key-Value engine written in C.

# Build

Run the build recipe with `just`:

```sh
just build
```

this will build the `kv_engine::lib`, `kv_engine::app`, as well as tests targets defined in `CMakeLists.txt`.

## App Usage Example

```sh
> kv-engine put --file foo.db key1 value1
> kv-engine put --file foo.db key2 value2
> kv-engine show --file foo.db
key2:	value2
key1:	value1
> kv-engine get --file foo.db key1
key1:	value1
> key-engine rm --file foo.db key2
> key-engine show --file foo.db
key1:	value1
> key-engine contains --file foo.db key1 # Exit code 0 
> key-engine contains --file foo.db key2 # Exit code 1
```

# Library Usage Example

```c
#include <stdio.h>
#include "kv_engine.h"

int main(void) {
    KVEHashMap *map = kve_map_create(16);
    kve_map_put(map, "First Name", "Gottfried");
    kve_map_put(map, "Middle Name", "Wilhelm");
    kve_map_put(map, "Last Name", "Leibniz");
    kve_map_put(map, "Birth Year", "1646");
    kve_map_put(map, "Death Year", "1716");
    kve_map_put(map, "Birth Place", "Leipzig");
    kve_map_put(map, "Death Place", "Hanover");

    KVEIterator *iter = kve_iter_create(map);
    while(kve_iter_next(iter))
        printf("%s:\t\t%s\n", kve_iter_key(iter), kve_iter_value(iter));

    kve_map_destroy(map);
    kve_iter_destroy(iter);

    return 0;
}
```

Output:

```
Death Year:		1716
Birth Place:		Leipzig
Death Place:		Hanover
Middle Name:		Wilhelm
Birth Year:		1646
Last Name:		Leibniz
First Name:		Gottfried
```
