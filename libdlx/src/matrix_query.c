#include "matrix_query.h"

#include "matrix_struct.h"

struct header *dlx_matrix_query_smallest_column(const struct dlx_matrix *mat)
{
	const struct node *header = ((struct node *)&mat->root)->right;
	const struct node *smallest = header;

	do {
		if (((struct header *)header)->size < ((struct header *)smallest)->size) {
			smallest = header;
		}

		/* Bail early if we find a column with just 1 node, they don't
		   come any smaller. */
		if (((struct header *)smallest)->size == 1) {
			break;
		}

		header = ((struct node *)header)->right;

	} while (header != (struct node *)&mat->root);

	return (struct header *)smallest;
}

bool dlx_matrix_is_empty(const struct dlx_matrix *matrix)
{
	const struct node *root = (const struct node *)&matrix->root;

	return root == root->right;
}
