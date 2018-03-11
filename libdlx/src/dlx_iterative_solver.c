#include "dlx_iterative_solver.h"

#include "dlx_dance.h"
#include "matrix_query.h"

static bool dlx_iterative_solver_has_found_solution(const struct dlx_solver *);

struct dlx_solver {
	/**
	 * The matrix for which solutions will be computed.
	 */
	struct dlx_matrix *matrix;

	/**
	 * This is a list of pointers to nodes, each item in the list
	 * points to a node of a different row in the matrix.
	 * Combining these rows yields a solution to the problem.
	 */
	struct node **stack;
	size_t max_node_stack_size;

	/**
	 * At what level the search is.
	 *
	 * When the algorithm finds a solution, depth is equal to the
	 * number of rows that make up the found solution.
	 */
	int depth;

	/**
	 * If set to true the algorithm will stop, this is used
	 * to 'unwind' the matrix back to its initial state (before any
	 * call to dlx_iterative_solve_next).
	 */
	bool abort_search;
};

struct dlx_solver *dlx_iterative_solver_create(struct dlx_matrix *mat)
{
	struct dlx_solver *const solver = calloc(1, sizeof(*solver));

	if (solver == 0) {
		return 0;
	}

	solver->matrix = mat;

	/*
	 * Compute the amount of space to allocate to accommodate a maximal
	 * solution (consisting of the highest number of rows).  This is:
	 * min(num_columns, num_rows)
	 */
	const size_t max_rows_in_solution = mat->rows_in_mat < mat->primary_cols ? mat->rows_in_mat : mat->primary_cols;

	/* We add one because the algorithm initializes the next element
	 * of the stack to zero after including a row in the solution,
	 * since we include max_rows_in_solution at most we need a
	 * dummy entry.
	 */
	solver->max_node_stack_size = max_rows_in_solution + 1;
	solver->stack = calloc(solver->max_node_stack_size, sizeof(*solver->stack));

	if (solver->stack == 0) {
		free(solver);
		return 0;
	}

	solver->depth = 0;
	dlx_iterative_solver_reset(solver);

	return solver;
}

void dlx_iterative_solver_reset(struct dlx_solver *solver)
{
	if (solver->depth > 0) {
		solver->abort_search = true;
		dlx_iterative_solver_find_solution(solver);
	}
	solver->depth = 0;
	for (size_t i = 0; i < solver->max_node_stack_size; ++i) {
		solver->stack[i] = 0;
	}
	solver->abort_search = false;
}

/**
 * Clean up after running the algorithm.
 *
 * Cleans up the data structures that were initialized by solve_init after
 * making sure the algorithm has terminated so that the matrix is in its
 * original state.
 */
void dlx_iterative_solver_destroy(struct dlx_solver *solver)
{
	/* Put the matrix in its initial state. */
	dlx_iterative_solver_reset(solver);

	free(solver->stack);
	free(solver);
}

bool dlx_iterative_solver_find_solution(struct dlx_solver *solver)
{
	/* If this is not the first solution */
	if (solver->depth > 0) {
		--solver->depth;
	}

	while (solver->depth >= 0) {
		if (solver->stack[solver->depth] == 0 && dlx_iterative_solver_has_found_solution(solver)) {
			/* We have found a solution. */
			return true;
		} else if (solver->stack[solver->depth] == 0) {
			/* No solution yet.  Cover a column. */
			solver->stack[solver->depth] = (struct node *)dlx_matrix_query_smallest_column(solver->matrix);
			dlx_dance_cover_column((struct header *)solver->stack[solver->depth]);
		} else if (solver->stack[solver->depth]->down != (struct node *)solver->stack[solver->depth]->head) {
			/**
			 * We have covered a column but have not yet explored
			 * the entire solution space. Do this now by walking
			 * down the matrix until we find a row with a 1 and
			 * include this row in the solution.
			 *
			 * Keep doing this until we have explored all the rows
			 * with 1's in in for the covered column.
			 */

			/* First uncover the current row if we need to .. */
			if (solver->stack[solver->depth] != (struct node *)solver->stack[solver->depth]->head) {
				dlx_dance_uncover_all_columns_in_row(solver->stack[solver->depth]);
			}

			/* .. and then cover the next row down */
			solver->stack[solver->depth] = solver->stack[solver->depth]->down;
			dlx_dance_cover_all_columns_in_row(solver->stack[solver->depth]);

			if (solver->abort_search) {
				continue;
			}

			/* Go down the rabbit hole. */
			++solver->depth;

			solver->stack[solver->depth] = 0;
		} else {
			/* Backtrack. */
			if (solver->stack[solver->depth] != (struct node *)solver->stack[solver->depth]->head) {
				dlx_dance_uncover_all_columns_in_row(solver->stack[solver->depth]);
			}
			dlx_dance_uncover_column(solver->stack[solver->depth]->head);
			--solver->depth;
		}
	}

	return false;
}

size_t dlx_iterative_solver_num_solution_rows(struct dlx_solver *solver) { return (size_t)solver->depth; }

const void *dlx_iterative_solver_get_solution_row(struct dlx_solver *solver, size_t index)
{
	return solver->stack[index]->user_data;
}

static bool dlx_iterative_solver_has_found_solution(const struct dlx_solver *solver)
{
	return dlx_matrix_is_empty(solver->matrix);
}
