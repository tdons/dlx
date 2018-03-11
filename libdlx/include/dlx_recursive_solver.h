#ifndef LIBDLX_RECURSIVE_SOLVER_H
#define LIBDLX_RECURSIVE_SOLVER_H

#include "matrix.h"

/**
 * @file
 *
 * Recursive solver.
 *
 * When compared to the iterative solver the implementation
 * of the recursive solver is orders of magnitude more elegant,
 * it's disadvantage is that it only permits *counting* the solutions.
 */

/**
 * @param the matrix to count solutions for
 * @return the amount of solutions
 */
size_t dlx_recursive_solver_count_solutions(struct dlx_matrix *);

#endif /* #ifndef LIBDLX_RECURSIVE_SOLVER_H */
