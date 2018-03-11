#ifndef LIBDLX_DANCE_H
#define LIBDLX_DANCE_H

#include "matrix_struct.h"

/**
 * @file
 *
 * Contains functions to make the links ``dance''.
 */

void dlx_dance_cover_column(struct header *);
void dlx_dance_cover_all_columns_in_row(struct node *);

void dlx_dance_uncover_column(struct header *);
void dlx_dance_uncover_all_columns_in_row(struct node *);

#endif /* #ifndef LIBDLX_DANCE_H */
