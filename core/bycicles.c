#include "bycicles.h"
#include "alch_types.h"

#include <stddef.h>
#include <stdint.h>

typedef union {
    combination_t combo;
    uint32_t key;
} combination_record;

size_t bsearch_element_index(const combination_record *arr, size_t n, uint32_t key) {
    size_t lo = 0;
    size_t hi = n; /* [lo, hi) */

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid].key < key) lo = mid + 1;
        else hi = mid;
    }

    if (lo < n && arr[lo].key == key)
        return lo;

    return (size_t)-1;
}

/* returns pointer to element or NULL. */
combination_t *bsearch_element_ptr(combination_record *arr, size_t n, uint32_t key) {
    size_t idx = bsearch_element_index(arr, n, key);
    return (idx == (size_t)-1) ? NULL : &arr[idx].combo;
}