#ifndef LIBDLX_H
#define LIBDLX_H

/**
 * @file
 *
 * Implementation of Knuth's ``Algorithm X'' using dancing links.
 * For more information see the paper:
 * www-cs-faculty.stanford.edu/~uno/papers/dancing-color.ps.gz
 */

/**
 * @mainpage libdlx
 *
 * libdlx is a C implementation of `dancing links', an efficient backtracking
 * algorithm tailored to solving Exact Cover.  For more information on the
 * Exact Cover problem see the included paper or wikipedia:
 * https://en.wikipedia.org/wiki/Exact_cover
 */

#include "dlx_iterative_solver.h"
#include "dlx_recursive_solver.h"
#include "matrix.h"

#endif /* #ifndef LIBDLX_H */
