#include <stdio.h>
#include <sys/time.h>

#include "fisher_yates.h"

#include "libdlx.h"

/**
 * Some useful constants.
 */

/* Empty cell in the grid */
#define EMPTY_CELL -1
/* A sudoku box is 3x3 .. */
#define BOXSIZE 3
/* .. therefore the dimensions of the puzzle are 9x9 .. */
#define PUZZLE_SIZE (BOXSIZE * BOXSIZE)
/* .. meaning there are 81 cells. */
#define NUM_CELLS (PUZZLE_SIZE * PUZZLE_SIZE)
/**
 * There are four constraints placed on every cell:
 * 1) values 1 through 9 in every row
 * 2) values 1 through 9 in every column
 * 3) values 1 through 9 in every box
 * 4) precisely one value per cell
 */
#define NUM_MATRIX_COLUMNS (4 * NUM_CELLS)
/* There are 9 numbers to place in a cell and 81 cells, so we have this many
 * rows. */
#define NUM_MATRIX_ROWS (PUZZLE_SIZE * NUM_CELLS)
#define NUM_MATRIX_NODES (4 * NUM_MATRIX_ROWS)

/**
 * Some useful macros.
 */

/**
 * Pack & unpack the combination of row, column, and value into
 * 3 bytes so that we can store it as the user_data of a row in
 * the dlx matrix
 */
#define PACK_RCV(r, c, v) ((r << 16) | (c << 8) | v)
#define UNPACK_R(packed) (packed >> 16)
#define UNPACK_C(packed) (packed >> 8 & 0xff)
#define UNPACK_V(packed) (packed & 0xff)

/**
 * The sudoku grid used throughout this program.
 */
static char grid[NUM_CELLS];

/**
 * Initialize the matrix.
 */
void initialize_matrix(struct dlx_matrix *m, struct node **row_ptrs)
{
	/* Shuffle the rows to ensure that the solver generates a random sudoku
	 * grid. */
	size_t *row_indices = malloc(sizeof(*row_indices) * NUM_MATRIX_ROWS);
	for (size_t i = 0; i < NUM_MATRIX_ROWS; ++i) {
		row_indices[i] = i;
	}
	fisher_yates(row_indices, NUM_MATRIX_ROWS);

	for (size_t i = 0; i < NUM_MATRIX_ROWS; ++i) {
		size_t row_index = (size_t)row_indices[i];
		size_t r = row_index / NUM_CELLS;
		size_t c = (row_index % NUM_CELLS) / PUZZLE_SIZE;
		size_t v = row_index % PUZZLE_SIZE;
		size_t b = r / BOXSIZE * BOXSIZE + c / BOXSIZE;

		row_ptrs[row_index] = dlx_matrix_begin_new_row(m, (void *)PACK_RCV(r, c, v));
		/* row/value */
		dlx_matrix_current_row_set_one(m, r * PUZZLE_SIZE + v); /* 0 - 80 */
		/* column/value */
		dlx_matrix_current_row_set_one(m, c * PUZZLE_SIZE + v + PUZZLE_SIZE * PUZZLE_SIZE); /* 81 - 161 */
		/* 1 number per cell */
		dlx_matrix_current_row_set_one(m, r * PUZZLE_SIZE + c + PUZZLE_SIZE * PUZZLE_SIZE * 2); /* 162 - 242 */
		/* box/value */
		dlx_matrix_current_row_set_one(m, b * PUZZLE_SIZE + v + PUZZLE_SIZE * PUZZLE_SIZE * 3); /* 243 - 323 */
	}

	free(row_indices);
}

void find_filled_sudoku_grid(struct dlx_solver *s)
{
	if (!dlx_iterative_solver_find_solution(s)) {
		fprintf(stderr, "Couldn't find initial solution, shouldn't happen.");
		exit(1);
	}

	/* num_solution_rows is always equal to NUM_CELLS */
	size_t num_solution_rows = dlx_iterative_solver_num_solution_rows(s);
	for (size_t i = 0; i < num_solution_rows; ++i) {
		size_t ri = (size_t)dlx_iterative_solver_get_solution_row(s, i);
		grid[UNPACK_R(ri) * PUZZLE_SIZE + UNPACK_C(ri)] = (char)UNPACK_V(ri);
	}

	dlx_iterative_solver_reset(s);
}

/**
 * The `gut' of the program.  When this method is called the state of affairs is
 * as follows:
 * - `grid' contains the completed sudoku grid (all 81 values are given)
 * - the dlx matrix contains all 729 rows
 *
 *
 * This method removes /every/ given value in random order, if the given value
 * can be removed while maintaining a unique solution: great, try the next one.
 * If the removal leads to more than 1 solution we will add the given back and
 * try to remove the next given.
 *
 * To 'remove' a given value we make sure that the matrix contains all 9 rows
 * for the cell in question.  This might be a little confusing: we /remove/ a
 * given value by /adding/ rows back to the matrix.  Basically we're enlarging
 * the search space for the algorithm.
 *
 * Analogously: if the 'removal' of a given lead to a non-unique solution we
 * fill
 * it back in by /removing/ all the 9 rows for the cell in question except the 1
 * representing the given.
 */
void reduce_grid(struct dlx_solver *s, struct node **row_ptrs)
{
	/**
	 * Let's try removing the givens in the cells 1 by 1, we will shuffle a
	 * list
	 * of all 81 positions and we will try to remove them one by one in that
	 * order.
	 * As long as there remains precisely 1 solution we're happy.
	 */
	size_t positions[NUM_CELLS];
	for (size_t i = 0; i < NUM_CELLS; ++i) {
		positions[i] = i;
	}
	fisher_yates(positions, NUM_CELLS);

	/**
	 * We begin by hiding all the rows in the matrix except for the 81 rows
	 * that
	 * represent the solution to `grid'.
	 */
	for (size_t i = 0; i < NUM_CELLS; ++i) {
		size_t to_remove = positions[i];
		size_t r = to_remove / PUZZLE_SIZE;
		size_t c = to_remove % PUZZLE_SIZE;

		/* We need to hide 8 rows */
		for (char v = 0; v < PUZZLE_SIZE; ++v) {
			if (v == grid[to_remove]) {
				continue;
			}
			size_t row_id = r * NUM_CELLS + c * PUZZLE_SIZE + v;
			dlx_matrix_hide_row(row_ptrs[row_id]);
		}
	}

	/* And add them back one by one (if possible) */
	for (size_t i = 0; i < NUM_CELLS; ++i) {
		size_t to_remove = positions[NUM_CELLS - 1 - i];
		size_t r = to_remove / PUZZLE_SIZE;
		size_t c = to_remove % PUZZLE_SIZE;

		/* We need to unhide 8 rows */
		for (char v = 0; v < PUZZLE_SIZE; ++v) {
			if (v == grid[to_remove]) {
				continue;
			}
			size_t row_id = r * NUM_CELLS + c * PUZZLE_SIZE + v;
			dlx_matrix_unhide_row(row_ptrs[row_id]);
		}

		/**
		 * Find the first solution.  This should always be possible
		 * because we have only enlarged
		 * the search space.
		 */
		if (!dlx_iterative_solver_find_solution(s)) {
			/* Famous last words :-) */
			fprintf(stderr, "No solution anymore, shouldn't happen.\n");
			exit(1);
		}

		/**
		 * If we can find a second solution: too bad.  The puzzle is not
		 * unique anymore,
		 * we re-add the rows and try the next cell.
		 */
		if (dlx_iterative_solver_find_solution(s)) {
			dlx_iterative_solver_reset(s);

			/* Make it unique again by ƒilling in the value. */
			for (char v = 0; v < PUZZLE_SIZE; ++v) {
				if (v == grid[to_remove]) {
					continue;
				}
				size_t row_id = r * NUM_CELLS + c * PUZZLE_SIZE + v;
				dlx_matrix_hide_row(row_ptrs[row_id]);
			}

			continue;
		}

		/* Remove the given value. */
		grid[to_remove] = EMPTY_CELL;
		dlx_iterative_solver_reset(s);
	}
}

/**
 * Generate a sudoku puzzle that adheres to these properties:
 * a) it has a unique solution
 * b) it is a minimal puzzle: this means that it is impossible to
 *    remove any given value without violating a)
 */
int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
	struct timeval seed;
	gettimeofday(&seed, NULL);

	srand((unsigned int)(seed.tv_sec ^ seed.tv_usec));

	/* Keep a list of pointers to the rows in the matrix */
	struct node **row_ptrs = malloc(sizeof(*row_ptrs) * NUM_MATRIX_ROWS);

	struct dlx_matrix *m = dlx_matrix_create(NUM_MATRIX_COLUMNS, 0, NUM_MATRIX_NODES);
	initialize_matrix(m, row_ptrs);
	struct dlx_solver *s = dlx_iterative_solver_create(m);
	find_filled_sudoku_grid(s);
	reduce_grid(s, row_ptrs);

	/* Print it. */
	for (size_t i = 0; i < NUM_CELLS; ++i) {
		putchar(grid[i] != EMPTY_CELL ? grid[i] + '1' : '0');
	}
	putchar('\n');

	free(row_ptrs);
	dlx_iterative_solver_destroy(s);
	dlx_matrix_destroy(m);
	exit(0);
}
