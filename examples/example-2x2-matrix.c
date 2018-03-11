#include "libdlx.h"
#include <stdio.h>

/**
 * Simple program to demonstrate usage of the dlx_* functions
 *
 * This should be the output of this program:
 *
 * found solution consisting of 2 rows
 * row 0 is part of the solution
 * row 1 is part of the solution
 */
int main(int argc, char *argv[])
{
	struct dlx_matrix *m = dlx_matrix_create(2, 0, 2);

	/**
	 * Create this matrix:
	 *   ab
	 * 0 10
	 * 1 01
	 */
	/* Start row 0 */
	dlx_matrix_begin_new_row(m, (void *)0);
	/* Set row 0 column a to 1 */
	dlx_matrix_current_row_set_one(m, 0);
	/* Start row 1 */
	dlx_matrix_begin_new_row(m, (void *)1);
	/* Set row 1 column b to 1 */
	dlx_matrix_current_row_set_one(m, 1);

	struct dlx_solver *s = dlx_iterative_solver_create(m);
	while (dlx_iterative_solver_find_solution(s)) {
		size_t rows = dlx_iterative_solver_num_solution_rows(s);
		fprintf(stdout, "found solution consisting of %zu rows\n", rows);
		for (size_t i = 0; i < rows; ++i)
			fprintf(stdout, "row %d is part of the solution\n",
				(int)dlx_iterative_solver_get_solution_row(s, i));
	}

	dlx_iterative_solver_destroy(s);
	dlx_matrix_destroy(m);
	exit(0);
}
