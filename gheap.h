/* in GH argument 'h' has type like as:
 * struct {
 *     ... as GDA structure ...
 *     int (*cmp)(<ptr>, <ptr>);
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

#include <stddef.h>

#ifndef GUTL_ASSERT
#include <assert.h>
#define GUTL_ASSERT assert
#endif

#ifndef gutl_swapp
#include <string.h>
#define gutl_swapp(ap, bp) do { \
    char gutl_swapp[sizeof *(ap) == sizeof *(bp)  \
        ? sizeof *(ap) : (size_t)-1];             \
    memcpy(gutl_swapp, (ap), sizeof(gutl_swapp)); \
    memcpy((ap),       (bp), sizeof(gutl_swapp)); \
    memcpy((bp), gutl_swapp, sizeof(gutl_swapp)); \
} while (0)
#endif

#define gh_heapify(h, index) do { \
    size_t _gh_i = (index),  \
        _gh_l, _gh_r, _gh_b; \
    while (1) { \
        _gh_l = _gh_i * 2 + 1; \
        _gh_r = _gh_i * 2 + 2; \
        _gh_b = _gh_i        ; \
        if (_gh_l < (h)->count && (h)->cmp((h)->items + _gh_l, \
            (h)->items + _gh_b) > 0) _gh_b = _gh_l; \
        if (_gh_r < (h)->count && (h)->cmp((h)->items + _gh_r, \
            (h)->items + _gh_b) > 0) _gh_b = _gh_r; \
        if (_gh_b == _gh_i) break; \
        gutl_swapp((h)->items + _gh_i, (h)->items + _gh_b); \
        _gh_i = _gh_b; \
    } \
} while (0)

#define gh_init(h) do { \
    for (size_t _gh_ii = (h)->count / 2; _gh_ii --> 0;) \
        gh_heapify(a, _gh_ii); \
} while (0)

#define gh_push(h) do { \
    size_t _gh_i = (h)->count - 1;  \
    size_t _gh_p = (_gh_i - 1) / 2; \
    while (_gh_i > 0 && (h)->cmp(   \
        (h)->items + _gh_p, (h)->items + _gh_i) < 0) { \
        gutl_swapp((h)->items + _gh_i, (h)->items + _gh_p); \
        _gh_i = _gh_p; _gh_p = (_gh_i - 1) / 2; \
    } \
} while (0)

#define gh_pop(h) do { \
    GUTL_ASSERT((h)->count > 0); \
    gutl_swapp((h)->items, (h)->items + (h)->count - 1); \
    --(h)->count; gh_heapify(h, 0); \
} while (0)

#define gh_poped(h) ((h)->items[(h)->count])

#define gh_sort(h) do { \
    gh_init(h); \
    size_t _gh_count = (h)->count; \
    while ((h)->count) gh_pop(h);  \
    (h)->count = _gh_count;        \
} while (0)

#ifndef GH_NO_PRINT
#include <stdio.h>
#include <math.h>
/* Taken from: https://gist.github.com/ydm/4f0c948bc0d151631621 */
#define gh_print(h, max_width, type_spec) do { \
    size_t _gh_height = (size_t)floor(log2((h)->count)) + 1; \
    for (size_t _gh_h = 0; _gh_h < _gh_height; _gh_h++) {    \
        int _gh_field = (2 * (1 << (_gh_height - _gh_h - 1)) - 1) * (max_width); \
        for (size_t _gh_l = ((size_t)1 << _gh_h) - 1;                             \
            _gh_l < ((size_t)1 << (_gh_h + 1)) - 1 && _gh_l < (h)->count; _gh_l++) \
            printf("%*s%*"type_spec"%*s",                                          \
                (_gh_field - (max_width)) / 2, "", (max_width), (h)->items[_gh_l], \
                (_gh_field - (max_width)) / 2 + (max_width), ""); \
        putchar(10); \
    } \
} while (0)
#else
#define gh_print(h, max_width, type_spec)
#endif

#endif /* GENERIC_HEAP_H */