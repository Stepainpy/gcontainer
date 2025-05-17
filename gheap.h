/* in GHP argument 'h' has type like as:
 * struct {
 *     <type>* items;
 *     size_t count, capacity;
 *     int (*cmp)([const] <type>*, [const] <type>*);
 *     ... and your stuffs ...
 * }
 * 
 * compare function return:
 * = 0 if lhs = rhs
 * < 0 if lhs < rhs
 * > 0 if lhs > rhs
 */

#ifndef GENERIC_HEAP_H
#define GENERIC_HEAP_H

#include <string.h>

/* Macros definitions */

#ifndef GUTL_ASSERT
#include <assert.h>
#define GUTL_ASSERT assert
#endif

#ifndef GHP_INIT_CAP
#define GHP_INIT_CAP 32
#endif

#ifndef GHP_REALLOC
#include <stdlib.h>
#define GHP_REALLOC realloc
#endif

/* Memory managment */

#define ghp_reserve(h, expect) do { \
    if ((expect) > (h)->capacity) { \
        if ((h)->capacity == 0)     \
            (h)->capacity = GHP_INIT_CAP; \
        while ((expect) > (h)->capacity)  \
            (h)->capacity += ((h)->capacity + 1) / 2; \
        (h)->items = GHP_REALLOC((h)->items,          \
            (h)->capacity * sizeof(*(h)->items));     \
        GUTL_ASSERT((h)->items != NULL && "No memory"); \
    } \
} while (0)

/* Support functions */

#ifndef gutl_swapp
#define gutl_swapp(ap, bp) do { \
    char gutl_swapp[sizeof *(ap) == sizeof *(bp)  \
        ? sizeof *(ap) : (size_t)-1];             \
    memcpy(gutl_swapp, (ap), sizeof(gutl_swapp)); \
    memcpy((ap),       (bp), sizeof(gutl_swapp)); \
    memcpy((bp), gutl_swapp, sizeof(gutl_swapp)); \
} while (0)
#endif

#define ghp_heapify(h, index) do { \
    size_t _ghp_i = (index),     \
        _ghp_l, _ghp_r, _ghp_b;  \
    while (1) {                  \
        _ghp_l = _ghp_i * 2 + 1; \
        _ghp_r = _ghp_i * 2 + 2; \
        _ghp_b = _ghp_i        ; \
        if (_ghp_l < (h)->count && (h)->cmp((h)->items + _ghp_l, \
            (h)->items + _ghp_b) > 0) _ghp_b = _ghp_l;           \
        if (_ghp_r < (h)->count && (h)->cmp((h)->items + _ghp_r, \
            (h)->items + _ghp_b) > 0) _ghp_b = _ghp_r;           \
        if (_ghp_b == _ghp_i) break;                             \
        gutl_swapp((h)->items + _ghp_i, (h)->items + _ghp_b);    \
        _ghp_i = _ghp_b; \
    } \
} while (0)

/* Modification functions */

#define ghp_init(h) do { \
    for (size_t _ghp_ii = (h)->count / 2; _ghp_ii --> 0;) \
        ghp_heapify(a, _ghp_ii); \
} while (0)

#define ghp_push(h, value) do { \
    ghp_reserve(h, (h)->count + 1); \
    (h)->items[(h)->count++] = (value); \
    size_t _ghp_i = (h)->count - 1;   \
    size_t _ghp_p = (_ghp_i - 1) / 2; \
    while (_ghp_i > 0 && (h)->cmp(    \
        (h)->items + _ghp_p, (h)->items + _ghp_i) < 0) { \
        gutl_swapp((h)->items + _ghp_i, (h)->items + _ghp_p); \
        _ghp_i = _ghp_p; _ghp_p = (_ghp_i - 1) / 2; \
    } \
} while (0)

#define ghp_pop(h, out) do { \
    GUTL_ASSERT((h)->count > 0); \
    gutl_swapp((h)->items, (h)->items + (h)->count - 1); \
    (out) = *gda_last(h); --(h)->count; \
    ghp_heapify(h, 0); \
} while (0)

#define ghp_sort(h) do { \
    ghp_init(h); \
    size_t _ghp_count = (h)->count; \
    while ((h)->count) ghp_pop(h);  \
    (h)->count = _ghp_count;        \
} while (0)

#ifndef GH_NO_PRINT
#include <stdio.h>
#include <math.h>
/* Taken from: https://gist.github.com/ydm/4f0c948bc0d151631621 */
#define ghp_print(h, max_width, type_spec) do { \
    size_t _ghp_height = (size_t)floor(log2((h)->count)) + 1; \
    for (size_t _ghp_h = 0; _ghp_h < _ghp_height; _ghp_h++) { \
        int _ghp_field = (2 * (1 << (_ghp_height - _ghp_h - 1)) - 1) * (max_width);    \
        for (size_t _ghp_l = ((size_t)1 << _ghp_h) - 1;                                \
            _ghp_l < ((size_t)1 << (_ghp_h + 1)) - 1 && _ghp_l < (h)->count; _ghp_l++) \
            printf("%*s%*"type_spec"%*s",                                              \
                (_ghp_field - (max_width)) / 2, "", (max_width), (h)->items[_ghp_l],   \
                (_ghp_field - (max_width)) / 2 + (max_width), ""); \
        putchar(10); \
    } \
} while (0)
#else
#define ghp_print(h, max_width, type_spec)
#endif

#endif /* GENERIC_HEAP_H */