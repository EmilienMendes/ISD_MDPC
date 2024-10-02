#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// Arbitrary values for rotation types
#define LEFT -1
#define RIGHT 1

/**
 * Represents a one bit unsigned int value
 */
typedef struct
{
    unsigned int value : 1; /** Unsigned int on one bit only*/
} bit;

// Creation and Destruction of binary Matrix

bit **init_matrix(unsigned int nb_rows, unsigned int nb_columns);
int **init_non_binary_matrix(unsigned int nb_rows, unsigned int nb_columns);
void free_matrix(bit **matrix, unsigned int nb_rows);
bit **create_identity_matrix(unsigned int nb_rows);

void print_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns);

// Operations on binary Matrix

bit **copy_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns);

void target_weight_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns, unsigned int weight);
void randomize_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns);

bit **transpose_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns);
bit **rotation_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns, unsigned int nb_rotation, int direction_rotation);
bit **concatenation_matrix(bit **matrix1, bit **matrix2, unsigned int nb_rows_matrix1, unsigned int nb_columns_matrix1, unsigned int nb_rows_matrix2, unsigned int nb_columns_matrix2);

bit **inversion_matrix(bit **matrix, unsigned int n, unsigned int m);
void add_matrix(bit **matrix1, bit **matrix2, unsigned int nb_rows, unsigned nb_columns, unsigned int start);
bit **multiply_matrix(bit **matrix1, bit **matrix2, unsigned int nb_rows_matrix1, unsigned int nb_columns_matrix1, unsigned int nb_rows_matrix2, unsigned int nb_columns_matrix2);
int **multiply_non_binary_matrix(bit **matrix1, bit **matrix2, unsigned int nb_rows_matrix1, unsigned int nb_columns_matrix1, unsigned int nb_rows_matrix2, unsigned int nb_columns_matrix2);

// Properties check
unsigned int index_max_value(int start, int stop, bit *line);
int matrix_is_upper(bit **matrix, unsigned int nb_rows, unsigned int nb_columns);
unsigned int hamming_weight(bit **matrix, unsigned int nb_rows, unsigned int nb_columns);
int is_matrix_null(bit **matrix, unsigned int nb_rows, unsigned int nb_columns);

// Operations on lines
void shift_line(bit *line_to_modify, bit *initial_line, unsigned int nb_columns, unsigned int shift_size);
void swap_lines(bit **matrix, unsigned int line1, unsigned int line2);
void add_line(bit *line1, bit *line2, unsigned int nb_columns);

#endif