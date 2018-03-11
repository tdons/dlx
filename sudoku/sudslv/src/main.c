#include <stdio.h>

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

#define PUZZLE_HAS_UNIQUE_SOLUTION 0
#define PUZZLE_HAS_NO_SOLUTION -1
#define PUZZLE_HAS_MULTIPLE_SOLUTIONS -2

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
 * Initialize the matrix.
 */
void initialize_matrix(char *grid, struct dlx_matrix *m)
{
	dlx_matrix_reset(m);

	for (size_t r = 0; r < PUZZLE_SIZE; ++r) {
		for (size_t c = 0; c < PUZZLE_SIZE; ++c) {
			int given = grid[r * PUZZLE_SIZE + c] - '1';

			for (int v = 0; v < PUZZLE_SIZE; ++v) {
				/* If given == -1 then we need to add all 9
				 * rows, otherwise just the 1. */
				if (given != -1 && v != given) {
					continue;
				}

				size_t b = r / BOXSIZE * BOXSIZE + c / BOXSIZE;

				dlx_matrix_begin_new_row(m, (void *)PACK_RCV(r, c, v));
				dlx_matrix_current_row_set_one(m, r * PUZZLE_SIZE + v);
				dlx_matrix_current_row_set_one(m, c * PUZZLE_SIZE + v + PUZZLE_SIZE * PUZZLE_SIZE);
				dlx_matrix_current_row_set_one(m, r * PUZZLE_SIZE + c + PUZZLE_SIZE * PUZZLE_SIZE * 2);
				dlx_matrix_current_row_set_one(m, b * PUZZLE_SIZE + v + PUZZLE_SIZE * PUZZLE_SIZE * 3);
			}
		}
	}
}

int solve(char *grid, struct dlx_matrix *m)
{
	initialize_matrix(grid, m);

	struct dlx_solver *s = dlx_iterative_solver_create(m);

	/* Find a solution. */
	if (!dlx_iterative_solver_find_solution(s)) {
		dlx_iterative_solver_destroy(s);
		return PUZZLE_HAS_NO_SOLUTION;
	}

	/* Get the solution */
	size_t num_solution_rows = dlx_iterative_solver_num_solution_rows(s);
	for (size_t i = 0; i < num_solution_rows; ++i) {
		size_t ri = (size_t)dlx_iterative_solver_get_solution_row(s, i);
		grid[UNPACK_R(ri) * PUZZLE_SIZE + UNPACK_C(ri)] = (char)(UNPACK_V(ri) + '1');
	}

	/* Check for multiple solutions. */
	if (dlx_iterative_solver_find_solution(s)) {
		dlx_iterative_solver_destroy(s);
		return PUZZLE_HAS_MULTIPLE_SOLUTIONS;
	}

	/* Solution is unique. */
	dlx_iterative_solver_destroy(s);
	return PUZZLE_HAS_UNIQUE_SOLUTION;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
	struct dlx_matrix *mat = dlx_matrix_create(NUM_MATRIX_COLUMNS, 0, NUM_MATRIX_NODES);

	/* Puzzle plus a newline ('\n') and a string terminator ('\0') */
	char grid[NUM_CELLS + 2];

	for (size_t current_line = 1;; ++current_line) {

		grid[0] = '\0';

		/* Read a puzzle. */
		char *read_result = fgets((char *)&grid, NUM_CELLS + 2, stdin);
		if (feof(stdin)) {
			if (read_result == NULL) {
				/* No more puzzles to be read. */
				break;
			}
		}
		if (ferror(stdin)) {
			fprintf(stderr, "Input error at line %lu.\n", current_line);
			exit(1);
		}

		/* Validate input. */
		for (size_t i = 0; i < NUM_CELLS; ++i) {
			if (!(grid[i] >= '0' && grid[i] <= '9')) {
				fprintf(stderr, "Unexpected character or eof at line %lu, character %lu.\n",
					current_line, i + 1);
				exit(1);
			}
		}

		/* Solve it. */
		int found_solution = solve(grid, mat);
		if (found_solution == PUZZLE_HAS_NO_SOLUTION) {
			fprintf(stderr, "Puzzle %lu has no solution.\n", current_line);
			continue;
		} else if (found_solution == PUZZLE_HAS_MULTIPLE_SOLUTIONS) {
			fprintf(stderr, "Puzzle %lu has multiple solutions.\n", current_line);
			continue;
		}

		/* Print it. */
		grid[NUM_CELLS] = '\0';
		puts(grid);
	}

	dlx_matrix_destroy(mat);
	exit(0);
}
