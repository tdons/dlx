#ifndef LIBDLX_MATRIX_QUERY_H
#define LIBDLX_MATRIX_QUERY_H

#include "matrix.h"

#include <stdbool.h>

/**
 * @file
 *
 * Contains functions related to querying the matrix.
 */

/**
 * Find the column with the least amount of 1's
 *
 * Performs a linear search through the column headers to find the
 * one with minimum size.
 *
 * @pre \c dlx_matrix_is_empty returns \c false
 * @param the matrix
 * @return header of the smallest column
 */
struct header *dlx_matrix_query_smallest_column(const struct dlx_matrix *);

/**
 * Is the matrix empty (zero by zero).
 *
 * @param the matrix
 * @return whether the matrix is empty
 */
bool dlx_matrix_is_empty(const struct dlx_matrix *);

#endif /* #ifndef LIBDLX_MATRIX_QUERY_H */
