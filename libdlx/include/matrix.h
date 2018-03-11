#ifndef LIBDLX_MATRIX_H
#define LIBDLX_MATRIX_H

#include <stdlib.h>

/**
 * @file
 *
 * Contains functions related to creating a sparse boolean matrix represented
 * using circular doubly linked lists (a toroid).
 *
 * Consider a small example matrix:
 *
 * \code{.unparsed}
 * 0 1 0
 * 1 0 0
 * 0 1 1
 * \endcode
 *
 * This structure would be created as follows:
 *
 * \code{.c}
 * // Matrix consists of 3 columns and 4 nodes in total.
 * dlx_matrix * mat = dlx_matrix_create(3, 0, 4);
 *
 * // Add the first row
 * dlx_matrix_begin_new_row(mat, (void*) 1);
 * // Flip the 0 at index 1
 * dlx_matrix_current_row_set_one(mat, 1);
 *
 * // Add the second row
 * dlx_matrix_begin_new_row(mat, (void*) 2);
 * // Flip the 0 at index 0
 * dlx_matrix_current_row_set_one(mat, 0);
 *
 * // Add the last row (which has 2 ones).
 * dlx_matrix_begin_new_row(mat, (void*) 3);
 * // Flip the two zeroes
 * dlx_matrix_current_row_set_one(mat, 2);
 * dlx_matrix_current_row_set_one(mat, 3);
 *
 * // Clean up
 * dlx_matrix_destroy(mat);
 * \endcode
 *
 * The structure of nodes created by the above procedure would consist
 * of 4 circularly linked lists in the 'horizontal' direction (1 for
 * the column header, 3 for the rows) and 4 circularly linked lists in
 * the vertical direction (1 for the root and 3 for the columns) that
 * intersect one another in various places.  It looks like this in
 * memory:
 *
 * \code{.unparsed}
 * +-----------------+     +-----------------+
 * |                 |     |                 |
 * |  +-------+      |     |     +--------+  |
 * |  |       |      |     |     |        |  |
 * |  | +----------------------------+    |  |
 * |  | |     |      |     |     |   |    |  |
 * |  | |  +--v-+    v     v     v   |    |  |
 * |  | +-->root<--->c<--->c<--->c<--+    |  |
 * |  |    +--^-+    ^     ^     ^        |  |
 * |  |       |      |  +--|--+  |        |  |
 * |  |       |      |  |  |  |  |        |  |
 * |  +-------+      |  |  v  |  |        |  |
 * |                 |  +->1<-+  |        |  |
 * |                 |     ^     |        |  |
 * |              +--|--+  |     |        |  |
 * |              |  |  |  |     |        |  |
 * |              |  v  |  |     |        |  |
 * |              +->1<-+  |     |        |  |
 * |                 ^     |     |        |  |
 * |                 |     |     |        |  |
 * +-----------------+     v     v        |  |
 *                      +->1<--->1<-+     |  |
 *                      |  ^     ^  |     |  |
 *                      |  |     |  |     |  |
 *                      +-----------+     |  |
 *                         |     |        |  |
 *                         |     +--------+  |
 *                         |                 |
 *                         +-----------------+
 * \endcode
 *
 * Explanation:
 * - root is the root node
 * - the 'c' nodes are column headers
 * - the '1' nodes are the ones in the matrix
 * - every line represents /two/ pointers, one
 *   in each direction
 */
struct dlx_matrix;

/* Creates a struct dlx_matrix
 *
 * @param columns         Amount of primary columns in the matrix.
 * @param columns         Amount of secondary columns in the matrix.
 * @param node_pool_size  Amount of nodes, *note*: this is _not_ the
 * amount of rows.
 *
 * @return
 * A pointer to the newly created matrix or 0.
 * Reasons for returning zero are:
 * - \c columns or \c node_pool_size are 0
 * - sufficient memory could not be allocated
 * If a valid pointer (not null) is returned it *must* be freed using a
 * call to dlx_matrix_destroy to prevent memory leaks.
 *
 * This function creates a new matrix. The node_pool_size argument
 * specified the amount of nodes that will be available to use, *not* the
 * amount of rows. To calculate node_pool_size, multiply the amount of
 * rows you expect to add with the amount of nodes per row.
 */
struct dlx_matrix *dlx_matrix_create(size_t, size_t, size_t);

/* Clean up and destroy matrix.
 *
 * @param mat The matrix to be `destroyed'.
 *
 * Frees memory allocated by dlx_matrix_destroy.
 */
void dlx_matrix_destroy(struct dlx_matrix *);

/* Resets the matrix, effectively undoes all calls made to dlx_add_row
 * and dlx_set_one.
 *
 * @param mat Matrix to be reset
 */
void dlx_matrix_reset(struct dlx_matrix *);

/* Start a new row.
 *
 * @param mat The matrix to which to add the row.
 * @param user_data A user object (or an integer) that will be used to
 *        identify this row in a solution.
 * @return pointer to the first node of this row
 */
struct node *dlx_matrix_begin_new_row(struct dlx_matrix *, const void *);

/* Flip a zero to a one in the sparse matrix.
 *
 * @param matrix The matrix in which to set the one.
 * @param index An index corresponding to the column in which to set the
 * one.
 */
void dlx_matrix_current_row_set_one(struct dlx_matrix *, size_t);

void dlx_matrix_hide_row(struct node *);

void dlx_matrix_unhide_row(struct node *);

#endif /* #ifndef LIBDLX_MATRIX_H */
