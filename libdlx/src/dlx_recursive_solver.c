#include "dlx_recursive_solver.h"

#include "dlx_dance.h"
#include "matrix_query.h"

/**
 * The algorithm below maps very closely to the approach described
 * by Knuth in doc/Donald E. Knuth - Dancing Links.pdf
 */
size_t dlx_recursive_solver_count_solutions(struct dlx_matrix *matrix)
{
	if (dlx_matrix_is_empty(matrix)) {
		return 1;
	}

	size_t solutions_found = 0;
	struct header *column = dlx_matrix_query_smallest_column(matrix);
	dlx_dance_cover_column(column);
	for (struct node *vnode = ((struct node *)column)->down; vnode != (struct node *)column; vnode = vnode->down) {
		dlx_dance_cover_all_columns_in_row(vnode);
		solutions_found += dlx_recursive_solver_count_solutions(matrix);
		dlx_dance_uncover_all_columns_in_row(vnode);
	}
	dlx_dance_uncover_column(column);

	return solutions_found;
}
