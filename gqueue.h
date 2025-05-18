/* in GQU argument 'q' has type like as:
 * struct {
 *     <type>* items;
 *     size_t count, capacity;
 *     size_t read, write;
 *     ... and your stuffs ...
 * }
 */

#ifndef GENERIC_QUEUE_H
#define GENERIC_QUEUE_H

#include <string.h>

/* Macros definitions */

#ifndef GUTL_ASSERT
#include <assert.h>
#define GUTL_ASSERT assert
#endif

#ifndef GQU_INIT_CAP
#define GQU_INIT_CAP 32
#endif

#ifndef GQU_REALLOC
#include <stdlib.h>
#define GQU_REALLOC realloc
#endif

#define gqu_type(type) struct { \
    type* items; size_t count,  \
    capacity, read, write;      \
}

/* Memory managment */

#define gqu_reserve(q, expect) do { \
    if ((expect) > (q)->capacity) { \
        if ((q)->capacity == 0)     \
            (q)->capacity = GQU_INIT_CAP; \
        while ((expect) > (q)->capacity)  \
            (q)->capacity += ((q)->capacity + 1) / 2; \
        (q)->items = GQU_REALLOC((q)->items,          \
            (q)->capacity * sizeof(*(q)->items));     \
        GUTL_ASSERT((q)->items != NULL && "No memory"); \
    } \
} while (0)

#define gqu_can_reserve(q) ((q)->count == 0 || (q)->read < (q)->write)

/* Modification functions */

#define gqu_push(q, value) do { \
    gqu_reserve(q, (q)->count + 1); \
    if ((q)->read == (q)->write && (q)->count) { \
        memmove((q)->items + (q)->capacity + (q)->read - (q)->count, \
        (q)->items + (q)->read, ((q)->count - (q)->read) * sizeof(*(q)->items)); \
        (q)->read += (q)->capacity - (q)->count; \
    } \
    (q)->items[(q)->write++] = (value); \
    (q)->write %= (q)->capacity; \
    ++(q)->count; \
} while (0)

#define gqu_pop(q, ret) do { \
    GUTL_ASSERT((q)->count > 0); \
    (ret) = (q)->items[(q)->read++]; \
    (q)->read %= (q)->capacity; \
    --(q)->count; \
} while (0)

#endif /* GENERIC_QUEUE_H */