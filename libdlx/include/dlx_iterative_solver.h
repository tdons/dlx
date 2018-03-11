#ifndef LIBDLX_ITERATIVE_SOLVER_H
#define LIBDLX_ITERATIVE_SOLVER_H

#include "matrix.h"

#include <stdbool.h>

/**
 * @file
 *
 * Iterative solver.
 *
 * Advantages of this solver are the ability to inspect each solution,
 * the recursive solver doesn't have this option.
 */

struct dlx_solver;

/**
 * Create an iterative solver.
 *
 * Note: dlx_matrix* is not affected in any way.
 *
 * @param dlx_matrix* the matrix for which to construct the solver
 * @return pointer to the solver structure
 */
struct dlx_solver *dlx_iterative_solver_create(struct dlx_matrix *);

/**
 * Destroy a solver.
 *
 * Note: dlx_matrix* is not affected in any way.
 *
 * @param dlx_solver the solver structure to destroy
 */
void dlx_iterative_solver_destroy(struct dlx_solver *);

/**
 * Runs the solver.
 *
 * Call this function repeatedly until it returns false to enumerate
 * all the solutions.
 *
 * @param dlx_solver structure
 * @return true iff a solution was found
 */
bool dlx_iterative_solver_find_solution(struct dlx_solver *);

/**
 * Reset the solver.
 *
 * Effectively the same as a call to dlx_solver_destroy() followed
 * by a call to dlx_solver_create().
 */
void dlx_iterative_solver_reset(struct dlx_solver *);

/**
 * Find out how many rows of the matrix are included in the solution.
 *
 * @pre \c dlx_iterative_solver_find_solution() has been called and
 *      the function returned true
 * @return the number of rows in the current solution.
 */
size_t dlx_iterative_solver_num_solution_rows(struct dlx_solver *);

/**
 * Get user_data associated with a row in the current solution.
 *
 * @param index index of the row.
 * @return the \c void * user_data associated with a row of the current
 * solution. \c index should be \leq to num_rows.
 */
const void *dlx_iterative_solver_get_solution_row(struct dlx_solver *, size_t);

#endif /* #ifndef LIBDLX_ITERATIVE_SOLVER_H */
