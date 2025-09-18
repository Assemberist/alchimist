#include "bycicles.h"

#include <stdint.h>
#include <stddef.h>

struct element {
    uint32_t id;
    /* ... */
};

size_t bsearch_element_index(const struct element *arr, size_t n, uint32_t key) {
    size_t lo = 0;
    size_t hi = n; /* [lo, hi) */

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid].id < key) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    if (lo < n && arr[lo].id == key)
        return lo;
    return (size_t)-1;
}

/* returns pointer to element or NULL. */
struct element *bsearch_element_ptr(struct element *arr, size_t n, uint32_t key) {
    size_t idx = bsearch_element_index(arr, n, key);
    return (idx == (size_t)-1) ? NULL : &arr[idx];
}