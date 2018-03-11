#include "dlx_dance.h"

void dlx_dance_cover_column(struct header *col)
{
	struct node *header = (struct node *)col;

	header->right->left = header->left;
	header->left->right = header->right;

	for (struct node *v = header->down; v != header; v = v->down) {
		for (struct node *h = v->right; h != v; h = h->right) {
			h->down->up = h->up;
			h->up->down = h->down;
			--h->head->size;
		}
	}
}

/**
 * Note: v->head is not covered.  The algorithm calls
 *       dlx_dance_cover_column on v->head and this function
 *       on the row that is included in a solution.
 */
void dlx_dance_cover_all_columns_in_row(struct node *v)
{
	for (struct node *h = v->right; h != v; h = h->right) {
		dlx_dance_cover_column(h->head);
	}
}

/**
 * Note: this functions performs the reverse operation
 *       of dlx_dance_cover_column.
 */
void dlx_dance_uncover_column(struct header *col)
{
	struct node *header = (struct node *)col;

	for (struct node *v = header->up; v != header; v = v->up) {
		for (struct node *h = v->left; h != v; h = h->left) {
			++h->head->size;
			h->up->down = h;
			h->down->up = h;
		}
	}

	header->left->right = header;
	header->right->left = header;
}

/**
 * Note: v->head is not uncovered.  The algorithm calls
 *       this function on the row that is excluded from the
 *       solution and then dlx_dance_uncover_column on v->head.
 */
void dlx_dance_uncover_all_columns_in_row(struct node *v)
{
	for (struct node *h = v->left; h != v; h = h->left) {
		dlx_dance_uncover_column(h->head);
	}
}
