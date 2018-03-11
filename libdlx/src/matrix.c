#include "matrix.h"
#include "matrix_struct.h"

static void init_header(struct header *);
static void init_node(struct node *, const void *);
static void add_node_to_row(struct node *, struct node *);
static void add_node_to_column(struct header *, struct node *);
static void remove_node_from_column(struct node *);

struct dlx_matrix *dlx_matrix_create(size_t primary_columns, size_t secondary_columns, size_t node_pool_size)
{
	const size_t col_bytes = sizeof(struct header) * (primary_columns + secondary_columns);
	const size_t node_bytes = sizeof(struct node) * node_pool_size;

	/** calloc zeroes out the memory (malloc doesn't) **/
	struct dlx_matrix *const mat = calloc(1, sizeof(*mat));

	if (mat == 0) {
		return 0;
	}

	mat->pool = malloc(col_bytes + node_bytes);

	if (mat->pool == 0) {
		free(mat);
		return 0;
	}

	mat->primary_cols = primary_columns;
	mat->secondary_cols = secondary_columns;
	mat->node_pool_size = node_pool_size;
	mat->column_list = mat->pool;
	mat->node_pool = (struct node *)((struct header *)mat->pool + mat->primary_cols + mat->secondary_cols);

	dlx_matrix_reset(mat);

	return mat;
}

void dlx_matrix_destroy(struct dlx_matrix *mat)
{
	free(mat->pool);
	free(mat);
}

void dlx_matrix_reset(struct dlx_matrix *mat)
{
	init_header(&mat->root);

	mat->node_pool_used = 0;
	mat->rows_in_mat = 0;

	/* First initialize all headers */
	for (size_t i = 0; i < mat->primary_cols + mat->secondary_cols; ++i) {
		init_header(&mat->column_list[i]);
	}

	/*
	 * Tie all the primary headers to the root node.
	 *
	 * After this loop finishes we will have a
	 * doubly linked circular linked list of length
	 * 1 + primary_cols (root + column headers).
	 */
	struct header *prev = &mat->root;
	for (size_t i = 0; i < mat->primary_cols; ++i) {
		struct header *cur = &mat->column_list[i];
		add_node_to_row((struct node *)prev, (struct node *)cur);
		prev = cur;
	}
}

struct node *dlx_matrix_begin_new_row(struct dlx_matrix *mat, const void *user_data)
{
	mat->start_new_row = true;
	init_node(&mat->node_pool[mat->node_pool_used], user_data);
	++mat->rows_in_mat;
	return &mat->node_pool[mat->node_pool_used];
}

void dlx_matrix_current_row_set_one(struct dlx_matrix *mat, size_t index)
{
	add_node_to_column(&mat->column_list[index], &mat->node_pool[mat->node_pool_used]);

	/* If this is not the first node of a new row,
	   tie it to its predecessor */
	if (!mat->start_new_row) {
		add_node_to_row(&mat->node_pool[mat->node_pool_used - 1], &mat->node_pool[mat->node_pool_used]);
		mat->node_pool[mat->node_pool_used].user_data = mat->node_pool[mat->node_pool_used - 1].user_data;
	}

	mat->node_pool_used++;
	mat->start_new_row = false;
}

void dlx_matrix_hide_row(struct node *first)
{
	struct node *n = first;
	do {
		remove_node_from_column(n);
		n = n->right;
	} while (n != first);
}

void dlx_matrix_unhide_row(struct node *first)
{
	struct node *n = first;
	do {
		add_node_to_column(n->head, n);
		n = n->left;
	} while (n != first);
}

/** Unexposed functions **/

static void init_header(struct header *header)
{
	init_node((struct node *)header, 0);
	header->size = 0;
	((struct node *)header)->head = header;
}

static void init_node(struct node *n, const void *user_data)
{
	n->up = n->down = n->left = n->right = n;
	n->head = 0;
	n->user_data = user_data;
}

static void add_node_to_row(struct node *this, struct node *that)
{
	this->right->left = that;
	that->right = this->right;
	that->left = this;
	this->right = that;
}

static void add_node_to_column(struct header *header, struct node *that)
{
	struct node *this = (struct node *)header;
	++header->size;
	that->head = header;

	this->up->down = that;
	that->up = this->up;
	that->down = this;
	this->up = that;
}

static void remove_node_from_column(struct node *node)
{
	node->up->down = node->down;
	node->down->up = node->up;
	--node->head->size;
}
