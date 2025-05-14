/* GDA inspired by nob.h da by Tsoding */
/* in GDA argument 'a' has type like as:
 * struct {
 *     <type>* items;
 *     size_t count, capacity;
 *     ... and your stuffs ...
 * }
 */

#ifndef GENERIC_DYNAMIC_ARRAY_H
#define GENERIC_DYNAMIC_ARRAY_H

#include <string.h>

/* Macros definitions */

#ifndef GDA_INIT_CAP
#define GDA_INIT_CAP 32
#endif

#ifndef GDA_ASSERT
#include <assert.h>
#define GDA_ASSERT assert
#endif

#ifndef GDA_REALLOC
#include <stdlib.h>
#define GDA_REALLOC realloc
#endif

/* Memory managment */

#define gda_reserve(a, expect) do { \
    if ((expect) > (a)->capacity) { \
        if ((a)->capacity == 0)     \
            (a)->capacity = GDA_INIT_CAP; \
        while ((expect) > (a)->capacity)  \
            (a)->capacity += ((a)->capacity + 1) / 2;  \
        (a)->items = GDA_REALLOC((a)->items,           \
            (a)->capacity * sizeof(*(a)->items));      \
        GDA_ASSERT((a)->items != NULL && "No memory"); \
    } \
} while (0)

#define gda_shrink(a) do { \
    (a)->capacity = (a)->count; \
    (a)->items = GDA_REALLOC((a)->items, \
        (a)->capacity * sizeof(*(a)->items)); \
    GDA_ASSERT((a)->count != 0 &&             \
        (a)->items != NULL && "No memory");   \
} while (0)

/* Modification functions */

#define gda_push(a, value) do { \
    gda_reserve(a, (a)->count + 1); \
    (a)->items[(a)->count++] = (value); \
} while (0)

#define gda_push_many(a, array, length) do { \
    gda_reserve(a, (a)->count + (length));   \
    memcpy((a)->items + (a)->count, (array), \
        (length) * sizeof(*(a)->items));     \
    (a)->count += (length); \
} while (0)

#define gda_insert(a, index, value) do { \
    GDA_ASSERT((index) <= (a)->count);   \
    gda_reserve(a, (a)->count + 1);      \
    memmove((a)->items + (index) + 1,    \
        (a)->items + (index), ((a)->count \
        - (index)) * sizeof(*(a)->items)); \
    (a)->items[(index)] = (value); ++(a)->count; \
} while (0)

#define gda_insert_many(a, index, array, length) do { \
    GDA_ASSERT((index) <= (a)->count);       \
    gda_reserve(a, (a)->count + (length));   \
    memmove((a)->items + (index) + (length), \
        (a)->items + (index), ((a)->count -  \
        (index)) * sizeof(*(a)->items));     \
    memcpy((a)->items + (index), (array),    \
        (length) * sizeof(*(a)->items));     \
    (a)->count += (length); \
} while (0)

#define gda_pop(a) \
(GDA_ASSERT((a)->count > 0), (a)->items[--(a)->count])

#define gda_pop_many(a, trunc) do { \
    GDA_ASSERT((trunc) <= (a)->count); \
    (a)->count -= (trunc); \
} while (0)

#define gda_erase(a, index) do { \
    GDA_ASSERT((index) < (a)->count); \
    memmove((a)->items + (index), (a)->items + (index) + 1,  \
        ((a)->count-- - (index) - 1) * sizeof(*(a)->items)); \
} while (0)

#define gda_erase_many(a, index, trunc) do { \
    GDA_ASSERT((index) + (trunc) <= (a)->count); \
    memmove((a)->items + (index), (a)->items + (index) + (trunc), \
        ((a)->count - (index) - (trunc)) * sizeof(*(a)->items));  \
    (a)->count -= (trunc); \
} while (0)

#define gda_clear(a) do { (a)->count = 0; } while (0)

#define gda_resize(a, newsize) do { \
    if ((a)->count < (newsize)) {   \
        gda_reserve(a, newsize);    \
        memset((a)->items + (a)->count, 0, \
            ((newsize) - (a)->count) * \
            sizeof(*(a)->items)); \
    } \
    (a)->count = (newsize); \
} while (0)

/* Access functions */

#define gda_front(a) \
(GDA_ASSERT((a)->count > 0), (a)->items)

#define gda_last(a) \
(GDA_ASSERT((a)->count > 0), (a)->items + (a)->count - 1)

#define gda_foreach(type, var, a) \
for (type* var = (a)->items; var < (a)->items + (a)->count; ++var)

#ifndef GDA_NO_INFO
#include <stdio.h>
#define gda_info(a) do { \
    printf("[INFO]: " #a " = { .items = %p, " \
        ".count = %zu, .capacity = %zu }\n",  \
    (void*)(a)->items, (a)->count, (a)->capacity); \
} while (0)
#else
#define gda_info(a)
#endif

#endif /* GENERIC_DYNAMIC_ARRAY_H */