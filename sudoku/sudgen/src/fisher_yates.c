#include <stdlib.h>

#include "fisher_yates.h"

/**
 * Returns a random integer i such that 0 <= i < n
 */
static size_t r(size_t n) { return rand() / (RAND_MAX / n + 1); }

void fisher_yates(size_t *arr, size_t sz)
{
	for (size_t i = sz - 1; i > 0; --i) {
		size_t j = r(i + 1);
		size_t t = arr[j];
		arr[j] = arr[i];
		arr[i] = t;
	}
}
