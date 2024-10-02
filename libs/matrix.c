#include "matrix.h"

/**
 * Initialize a binary matrix.
 *
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 * @return binary matrix (nb_rows x nb_columns)
 */
bit **init_matrix(unsigned int nb_rows, unsigned int nb_columns)
{
    bit **matrix = (bit **)calloc(nb_rows, sizeof(bit *));

    for (unsigned int i = 0; i < nb_rows; i++)
        matrix[i] = calloc(nb_columns, sizeof(bit));
    return matrix;
}

/**
 * Initialize an integer matrix.
 *
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 * @return matrix (nb_rows x nb_columns)
 */
int **init_non_binary_matrix(unsigned int nb_rows, unsigned int nb_columns)
{
    int **matrix = (int **)calloc(nb_rows, sizeof(int *));
    for (unsigned int i = 0; i < nb_rows; i++)
        matrix[i] = (int *)calloc(nb_columns, sizeof(int));
    return matrix;
}

void free_matrix(bit **matrix, unsigned int nb_rows)
{
    for (unsigned int i = 0; i < nb_rows; i++)
        free(matrix[i]);
    free(matrix);
}

void free_non_binary_matrix(int **matrix, unsigned int nb_rows)
{
    for (unsigned int i = 0; i < nb_rows; i++)
        free(matrix[i]);
    free(matrix);
}
/**
 * Create the identity matrix.
 *
 * @param nb_rows number of rows
 * @return the identity matrix (nb_rows x nb_rows)
 */
bit **create_identity_matrix(unsigned int nb_rows)
{
    bit **identity = init_matrix(nb_rows, nb_rows);
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        for (unsigned int j = 0; j < nb_rows; j++)
        {
            if (i == j)
                identity[i][j].value = 1;
            else
                identity[i][j].value = 0;
        }
    }
    return identity;
}

void print_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns)
{
    printf("----------\n");
    for (int i = 0; i < nb_rows; i++)
    {
        printf("[");
        for (int j = 0; j < nb_columns; j++)
        {
            if (j != nb_columns - 1)
                printf("%d, ", matrix[i][j].value);
            else
                printf("%d", matrix[i][j].value);
        }
        printf("]\n");
    }
}

/**
 * Copy a binary matrix.
 *
 * @param matrix matrix to get to the hamming weight
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 * @return copy of the matrix
 */
bit **copy_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns)
{
    bit **matrix_copy = init_matrix(nb_rows, nb_columns);
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        for (unsigned int j = 0; j < nb_columns; j++)
        {
            matrix_copy[i][j].value = matrix[i][j].value;
        }
    }
    return matrix_copy;
}

/**
 * Randomize the matrix to a given hamming weight.
 * Mainly used for the error init.
 *
 * @param matrix matrix to get to the hamming weight
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 * @param weight hamming weight
 */
void target_weight_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns, unsigned int weight)
{
    unsigned int weight_matrix = 0;
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        for (unsigned int j = 0; j < nb_columns; j++)
        {
            unsigned int r = rand() % 2;
            if (r == 1 && weight_matrix < weight)
            {
                matrix[i][j].value = r;
                weight_matrix++;
            }
            else
            {
                matrix[i][j].value = 0;
            }
        }
    }
}

/**
 * Randomize the bits of an binary matrix.
 *
 * @param matrix matrix to randomize the bits of
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 */
void randomize_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns)
{
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        for (unsigned int j = 0; j < nb_columns; j++)
        {
            matrix[i][j].value = rand() % 2;
        }
    }
}

/**
 * Swap two lines of an binary matrix.
 *
 * @param matrix the matrix to print
 * @param line1 first line to swap
 * @param line2 second line to swap
 */
void swap_lines(bit **matrix, unsigned int line1, unsigned int line2)
{
    bit *temp_line = matrix[line1];
    matrix[line1] = matrix[line2];
    matrix[line2] = temp_line;
}

/**
 * Add two binary line mod 2.
 *
 * @param line1 first line
 * @param line2 second line
 * @param nb_columns size of the lines
 */
void add_line(bit *line1, bit *line2, unsigned int nb_columns)
{
    for (int i = 0; i < nb_columns; i++)
        line1[i].value = (line1[i].value ^ line2[i].value);
}

/**
 * Transposition of a matrix.
 *
 * @param matrix matrix to transpose
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 * @return the transposed matrix (keeping the source)
 */
bit **transpose_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns)
{
    bit **transposed_matrix = init_matrix(nb_columns, nb_rows);
    for (int i = 0; i < nb_columns; i++)
    {
        for (int j = 0; j < nb_rows; j++)
            transposed_matrix[i][j].value = matrix[j][i].value;
    }
    return transposed_matrix;
}

/**
 * Rotation of a matrix
 * @param matrix matrix to rotate
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 * @return the rotated matrix (keeping the source)
 *
 */
bit **rotation_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns, unsigned int nb_rotation, int direction_rotation)
{
    bit **rotated_matrix = init_matrix(nb_columns, nb_rows);
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_columns; j++)
        {
            rotated_matrix[i][j] = matrix[i][(j + direction_rotation * nb_rotation) % nb_columns];
        }
    }
    return rotated_matrix;
}

/**
 * Concatenation of two matrix
 * @param matrix1 first matrix to concatenate
 * @param matrix2 first matrix to concatenate
 * @param nb_rows_matrix1 number of rows of first matrix
 * @param nb_columns_matrix1 number of columns of first matrix
 * @param nb_rows_matrix2 number of rows of second matrix
 * @param nb_columns_matrix2 number of columns of second matrix
 * @return the concatenated matrix (keeping the source)
 *
 */
bit **concatenation_matrix(bit **matrix1, bit **matrix2, unsigned int nb_rows_matrix1, unsigned int nb_columns_matrix1, unsigned int nb_rows_matrix2, unsigned int nb_columns_matrix2)
{
    assert(nb_rows_matrix1 == nb_rows_matrix2);
    bit **concatenated_matrix = init_matrix(nb_rows_matrix1, nb_columns_matrix1 + nb_columns_matrix2);
    for (int i = 0; i < nb_rows_matrix1; i++)
    {
        for (int j = 0; j < nb_rows_matrix1; j++)
        {
            concatenated_matrix[i][j] = matrix1[i][j];
        }
        for (int k = 0; k < nb_columns_matrix2; k++)
        {
            concatenated_matrix[i][k + nb_columns_matrix1] = matrix2[i][k];
        }
    }
    return concatenated_matrix;
}

/**
 * Matrix inversion using Gauss - Jordan.
 * Heavily inspired by https://dde.binghamton.edu/filler/mct/hw/1/assignment.pdf
 *
 * @param m matrix to invert
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 * @return the inverted matrix (keeping the source)
 */
bit **inversion_matrix(bit **m, unsigned int nb_rows, unsigned int nb_columns)
{
    bit **matrix = copy_matrix(m, nb_rows, nb_columns);
    bit **inverted_matrix = create_identity_matrix(nb_rows);

    for (int i = 0; i < nb_columns; i++)
    {
        int maxi = i;

        for (int k = i; k < nb_rows; k++)
        {
            if (matrix[k][i].value == 1)
            {
                maxi = k;
            }
        }

        if (matrix[maxi][i].value == 1)
        {
            swap_lines(matrix, i, maxi);
            swap_lines(inverted_matrix, i, maxi);

            for (int u = i + 1; u < nb_columns; u++)
            {
                if (matrix[u][i].value != 0)
                {
                    add_line(matrix[u], matrix[i], nb_columns);
                    add_line(inverted_matrix[u], inverted_matrix[i], nb_columns);
                }
            }
        }
    }
    // Matrix is upper so we need to reduce it to have identity matrix
    int is_inversible;
    if (matrix_is_upper(matrix, nb_rows, nb_columns))
    {
        is_inversible = 1;
        // Start from the last column and add every time there is a 1 not in the [i][i]columns
        int current_column = nb_columns - 1;
        while (current_column != 0)
        {
            for (int i = nb_rows - 1; i >= 0; i--)
            {
                if (matrix[i][current_column].value == 1 && i != current_column)
                {
                    add_line(matrix[i], matrix[current_column], nb_columns);
                    add_line(inverted_matrix[i], inverted_matrix[current_column], nb_columns);
                }
            }
            current_column--;
        }
    }
    free_matrix(matrix, nb_rows);

    if (is_inversible)
        return inverted_matrix;
    else
        return NULL;
}

/**
 * Add two binary matrix.
 *
 * @param matrix1 original matrix
 * @param matrix2 added matrix
 * @param nb_rows number of rows of both
 * @param nb_columns number of columns of both
 * @param start The starting position for matrix2 e.g start = n, matrix2 = matrix2[n:]
 * @return the addition of both (binary matrix)
 */
void add_matrix(bit **matrix1, bit **matrix2, unsigned int nb_rows, unsigned nb_columns, unsigned int start)
{
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_columns; j++)
            matrix1[i][j].value ^= matrix2[i][start + j].value;
    }
}

/**
 * Multiplication of two binary matrix.
 * nb_columns_matrix1 must be equals to nb_rows_matrix2
 * Inspiration from https://www.geeksforgeeks.org/c-matrix-multiplication/
 *
 * @param matrix1 matrix to multiply with
 * @param matrix2 matrix to get multiplied
 * @param nb_rows_matrix1 number of columns
 * @param nb_columns_matrix1 number of columns
 * @param nb_rows_matrix2 number of columns
 * @param nb_columns_matrix2 number of columns
 *
 * @return the multiplication of both (nb_rows_matrix1 x nb_columns_matrix2)
 */
bit **multiply_matrix(bit **matrix1, bit **matrix2, unsigned int nb_rows_matrix1, unsigned int nb_columns_matrix1, unsigned int nb_rows_matrix2, unsigned int nb_columns_matrix2)
{
    bit **result = init_matrix(nb_rows_matrix1, nb_columns_matrix2);
    for (int i = 0; i < nb_rows_matrix1; i++)
    {
        for (int j = 0; j < nb_columns_matrix2; j++)
        {
            result[i][j].value = 0;

            for (int k = 0; k < nb_rows_matrix2; k++)
            {
                result[i][j].value += matrix1[i][k].value * matrix2[k][j].value;
            }
        }
    }
    return result;
}

/**
 * Multiplication of two binary matrix to get an integer matrix !.
 * nb_columns_matrix1 must be equals to nb_rows_matrix2
 * Inspiration from https://www.geeksforgeeks.org/c-matrix-multiplication/
 *
 * @param matrix1 matrix to multiply with
 * @param matrix2 matrix to get multiplied
 * @param nb_rows_matrix1 number of columns
 * @param nb_columns_matrix1 number of columns
 * @param nb_rows_matrix2 number of columns
 * @param nb_columns_matrix2 number of columns
 *
 * @return the integer matrix representing the multiplication of both (nb_rows_matrix1 x nb_columns_matrix2)
 */
int **multiply_non_binary_matrix(bit **matrix1, bit **matrix2, unsigned int nb_rows_matrix1, unsigned int nb_columns_matrix1, unsigned int nb_rows_matrix2, unsigned int nb_columns_matrix2)
{
    int **result = init_non_binary_matrix(nb_rows_matrix1, nb_columns_matrix2);
    for (int i = 0; i < nb_rows_matrix1; i++)
    {
        for (int j = 0; j < nb_columns_matrix2; j++)
        {
            for (int k = 0; k < nb_rows_matrix2; k++)
                result[i][j] += matrix1[i][k].value * matrix2[k][j].value;
        }
    }
    return result;
}

/**
 * Verify if a matrix is in upper triangular form.
 * Helpful for the inversion with Gauss - Jordan.
 *
 * @param matrix first line
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 * @return 1 if upper triangular, 0 if not
 */
int matrix_is_upper(bit **matrix, unsigned int nb_rows, unsigned int nb_columns)
{
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (matrix[i][j].value == 1)
                return 0;
        }
    }
    return 1;
}

/**
 * Hamming weight of an binary matrix.
 *
 * @param matrix matrix to count the weight of
 * @param nb_rows number of rows
 * @param nb_columns number of columns
 * @return the hamming weight of the matrix
 */
unsigned int hamming_weight(bit **matrix, unsigned int nb_rows, unsigned int nb_columns)
{
    unsigned int nb_bit_one = 0;
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_columns; j++)
        {
            if (matrix[i][j].value == 1)
                nb_bit_one++;
        }
    }
    return nb_bit_one;
}

int is_matrix_null(bit **matrix, unsigned int nb_rows, unsigned int nb_columns)
{
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_columns; j++)
        {
            if (matrix[i][j].value == 1)
                return 0;
        }
    }
    return 1;
}

/**
 * We will use the shift with a circular matrix, so we just have one line in memory
 *
 * @param line_to_modify result line
 * @param initial_line line that we will shift
 * @param nb_columns size of the line
 * @param shift_size number of positions of the shift
 */
void shift_line(bit *line_to_modify, bit *initial_line, unsigned int nb_columns, unsigned int shift_size)
{
    for (int i = 0; i < nb_columns; i++)
    {
        line_to_modify[i].value = initial_line[(i + shift_size) % nb_columns].value;
    }
}
