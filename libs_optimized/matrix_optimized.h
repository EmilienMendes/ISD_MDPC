#ifndef MATRIX_OPTIMIZED_H
#define MATRIX_OPTIMIZED_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "math.h"

#define INT_SIZE (sizeof(unsigned int) * 8)
#define INT_MAX 4294967295
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

/**
 * Matrix representation using unsigned int and binary operations
 */
typedef struct
{
   unsigned int **array;     /** 2d array */
   unsigned int line_size;   /** Number of rows*/
   unsigned int column_size; /** Number of columns, not of the array per say, but in bit !*/
} binary_matrix;

// Creation and Destruction of binary Matrix

binary_matrix init_optimized_matrix(unsigned int nb_rows, unsigned int nb_columns);
void free_optimized_matrix(binary_matrix matrix);
binary_matrix create_optimized_identity_matrix(unsigned int nb_rows);

void optimized_print_matrix(binary_matrix matrix);

// Operations on binary Matrix

binary_matrix copy_optimized_matrix(binary_matrix matrix);
void target_weight_optimized_matrix(binary_matrix matrix, unsigned int weight);
void randomize_optimized_matrix(binary_matrix matrix);
binary_matrix optimized_sample_random(binary_matrix matrix, int sample_size);

void add_optimized_matrix(binary_matrix matrix1, binary_matrix matrix2, unsigned int start);
binary_matrix transpose_optimized_matrix(binary_matrix matrix);
binary_matrix rotation_optimized_matrix(binary_matrix matrix);
binary_matrix concatenation_optimized_matrix(binary_matrix matrix1, binary_matrix matrix2);

binary_matrix inversion_optimized_matrix(binary_matrix matrix, int *result);
binary_matrix multiply_optimized_matrix(binary_matrix matrix1, binary_matrix matrix2);

// Properties check

int optimized_matrix_is_upper(binary_matrix matrix);
unsigned int optimized_hamming_weight(binary_matrix matrix);
int optimized_is_matrix_null(binary_matrix matrix);

// Operations on lines

// unsigned int *optimized_shift_line(unsigned int *initial_line, unsigned int nb_columns, unsigned int shift_size);

void optimized_swap_lines(binary_matrix matrix, unsigned int line1, unsigned int line2);
void optimized_add_line(unsigned int *line1, unsigned int *line2, unsigned int nb_columns);
void print_line(unsigned int *line, unsigned int nb_columns);

// Test
void test_optimized_print_matrix();
void test_hamming_weight();
void test_concatenation_matrix();
void test_shift_line();
void test_identity_matrix();
void test_transpose_matrix();
void test_multiplication_matrix();
void test_inversion_matrix();
void test_sample_random();

#endif