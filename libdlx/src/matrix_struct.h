#ifndef LIBDLX_MATRIX_STRUCT_H
#define LIBDLX_MATRIX_STRUCT_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * @file
 * Matrix data structures.
 */

/**
 * A node in the matrix.
 *
 * A node represents a one in the matrix. Absence of a node indicates
 * a 0 at that position. The sparse matrix is represented as a doubly
 * linked circular list.
 *
 * All the nodes in the same row have identical \c user_data pointers.
 * The \c user_data member stores information that will help a user
 * identify which rows are present in a solution.
 */
struct node {
	struct node *up;     /**< Points to the node above this node. **/
	struct node *down;   /**< Points to the node below this node. **/
	struct node *left;   /**< Points to the left neighbour. **/
	struct node *right;  /**< Points to the right neighbour. **/
	struct header *head; /**< Points to the column header. **/

	/**
	 * points to a user provided data structure, alternatively
	 * a user can store an integer in here (8 bytes on a 64bit system)
	 */
	const void *user_data;
};

/**
 * A column header
 *
 * There are as many of these structures in the matrix as there are
 * columns. The size member is used to keep track of the amount of 1's
 * (nodes) in the corresponding column.
 */
struct header {
	/**
	 * This __must__ be the first member. Throughout the code (struct
	 * header*)'s are often cast to (struct node *)'s, this only works
	 * if node is the first member.
	 */
	struct node node;
	size_t size; /**< Keeps track of the number of nodes in this column. **/
};

/**
 * Contains the sparse matrix.
 *
 * This structure is to be manipulated only through the dlx_* functions.
 *
 * \code{.unparsed}
 * Memory layout:
 * pool          |---------------------------------|
 *               ^        ^
 * column_list---+        |
 * node_pool -------------+
 * \endcode
 *
 * column_list has length equal to `primary_cols' + `secondary_cols'
 * node_pool has length equal to `node_pool_size'
 */
struct dlx_matrix {
	struct header root; /**< Root node of the matrix. **/

	/**
	 * Memory pool holding all the column headers and nodes.
	 */
	void *pool;

	/**
	 * First column object in pool.  Always equal to `pool'.
	 */
	struct header *column_list;

	/**
	 * Indicates if a subsequent call to dlx_matrix_current_row_set_one
	 * starts a new row.  Set by dlx_matrix_begin_new_row
	 */
	bool start_new_row;

	/**
	 * Pointer to the first node object in pool.
	 */
	struct node *node_pool;
	size_t node_pool_size; /**< Amount of nodes in the pool. **/
	size_t node_pool_used; /**< Amount of nodes in use. **/

	size_t primary_cols;   /**< Amount of primary columns in the matrix. **/
	size_t secondary_cols; /**< Amount of secondary columns in the matrix. **/
	size_t rows_in_mat;    /**< Amount of rows in the matrix. **/
};

#endif /* #ifndef LIBDLX_MATRIX_STRUCT_H */
