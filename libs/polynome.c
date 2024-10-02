#include "polynome.h"

/**
 * Initialize a polynomial matrix from a binary matrix.
 *
 * @param matrix binary matrix
 * @param nb_rows number of rows of both
 * @param nb_columns number of columns of the binary matrix
 * @return polynomial matrix
 */
polynome *init_polynomial_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns)
{
    polynome *polynomial_matrix = (polynome *)malloc(sizeof(polynome) * nb_rows);
    for (int i = 0; i < nb_rows; i++)
    {
        // Get size of each polynomial rows
        int size = 0;
        for (int j = 0; j < nb_columns; j++)
        {
            if (matrix[i][j].value == 1)
                size++;
        }
        polynomial_matrix[i].liste_indice = (int *)malloc(sizeof(int) * size);
        polynomial_matrix[i].size = size;

        // Add the index of element in the liste of index
        int index = 0;
        for (int j = 0; j < nb_columns; j++)
        {
            if (matrix[i][j].value == 1)
            {
                polynomial_matrix[i].liste_indice[index] = j;
                index++;
            }
        }
    }
    return polynomial_matrix;
}

void free_polynomial_matrix(polynome *polynomial_matrix, unsigned int nb_rows)
{
    for (int i = 0; i < nb_rows; i++)
        free(polynomial_matrix[i].liste_indice);
    free(polynomial_matrix);
}

void free_polynomial_matrix_Z(polynome_Z *polynomial_matrix_Z, unsigned int nb_rows)
{
    for (int i = 0; i < nb_rows; i++)
        free(polynomial_matrix_Z[i].liste_indice);
    free(polynomial_matrix_Z);
}

void print_polynomial_matrix(polynome *polynomial_matrix, unsigned int nb_rows)
{
    for (int i = 0; i < nb_rows; i++)
    {
        printf("[");
        for (int j = 0; j < polynomial_matrix[i].size; j++)
        {
            if (j != polynomial_matrix[i].size - 1)
                printf("%d, ", polynomial_matrix[i].liste_indice[j]);
            else
                printf("%d", polynomial_matrix[i].liste_indice[j]);
        }
        printf("] size : %d\n", polynomial_matrix[i].size);
    }
}

void print_polynomial_matrix_Z(polynome_Z *polynomial_matrix_Z, unsigned int nb_rows)
{
    for (int i = 0; i < nb_rows; i++)
    {
        printf("[");
        for (int j = 0; j < polynomial_matrix_Z[i].size; j++)
        {
            if (j != polynomial_matrix_Z[i].size - 1)
                printf("%d, ", polynomial_matrix_Z[i].liste_indice[j]);
            else
                printf("%d", polynomial_matrix_Z[i].liste_indice[j]);
        }
        printf("] coeff : ");

        printf("{");
        for (int k = 0; k < polynomial_matrix_Z[i].size; k++)
        {
            if (k != polynomial_matrix_Z[i].size - 1)
                printf("%d, ", polynomial_matrix_Z[i].coeff[k]);
            else
                printf("%d", polynomial_matrix_Z[i].coeff[k]);
        }
        printf("}\n");
    }
}

// Not usable, Euclide but we didn't get a good enough grasp on how to do it
/*polynome *inversion_polynomial_matrix(polynome *polynomial_matrix, unsigned int nb_rows, unsigned int nb_columns)
{
}*/

/**
 * Define the behavior when comparing two int for the bsearch algorithm in <stdlib.h>.
 * https://koor.fr/C/cstdlib/bsearch.wp
 *
 * @param a
 * @param b
 * @return if a is greater than b
 */
int compare_int(const void *a, const void *b)
{
    return (*(const int *)a) - (*(const int *)b);
}

/**
 * Searching if data found in array using the bsearch algorithm in <stdlib.h>.
 *
 * @param index_to_search the adress of the element to search for
 * @param array the array to search in
 * @param nb_element number of element inside the array
 * @return 1 if we found the element, 0 else
 */
int search(void *index_to_search, int *array, int nb_element)
{
    // int *a = (int *)index_to_search;
    return (bsearch(index_to_search, array, nb_element, sizeof(int), compare_int)) != NULL;
    if ((bsearch(index_to_search, array, nb_element, sizeof(int), compare_int)) != NULL)
    {
        return 1;
    }
    return 0;
}

/**
 * Multiplication of two polynomial matrix.
 * nb_columns_matrix1 must be equals to nb_rows_matrix2
 * Inspiration from https://www.geeksforgeeks.org/c-matrix-multiplication/
 *
 * @param matrix1 matrix to multiply with
 * @param matrix2 matrix to get multiplied
 * @param nb_rows_matrix1 number of columns
 * @param nb_columns_matrix2 number of columns
 *
 * @return the multiplication of both (a polynomial matrix)
 */
polynome *multiplication_polynomial_matrix(polynome *polynomial_matrix1, polynome *polynomial_matrix2, unsigned int nb_rows_polynome1, unsigned int nb_columns_polynome2)
{
    polynome *result = (polynome *)malloc(sizeof(polynome) * nb_rows_polynome1);

    int *tmp_line = (int *)malloc(nb_columns_polynome2 * sizeof(int));
    int value, current_index;

    for (int i = 0; i < nb_rows_polynome1; i++)
    {
        current_index = 0;

        for (int j = 0; j < nb_columns_polynome2; j++)
        {
            value = 0;
            for (int k = 0; k < polynomial_matrix1[i].size; k++)
            {
                int index = polynomial_matrix1[i].liste_indice[k];
                value = (value + search(&j, polynomial_matrix2[index].liste_indice, polynomial_matrix2[index].size)) % 2;
            }
            if (value != 0)
            {
                tmp_line[current_index] = j;
                current_index++;
            }
        }

        result[i].size = current_index;
        result[i].liste_indice = (int *)malloc(sizeof(int) * current_index);
        current_index = 0;

        for (int l = 0; l < result[i].size; l++)
        {
            result[i].liste_indice[l] = tmp_line[l];
        }
    }
    free(tmp_line);
    return result;
}

/**
 * Multiplication of two polynomial matrix, but result in Z, not in 2.
 * nb_columns_matrix1 must be equals to nb_rows_matrix2
 * Inspiration from https://www.geeksforgeeks.org/c-matrix-multiplication/
 *
 * @param matrix1 matrix to multiply with
 * @param matrix2 matrix to get multiplied
 * @param nb_rows_matrix1 number of columns
 * @param nb_columns_matrix2 number of columns
 *
 * @return the multiplication of both (a polynomial matrix)
 */
polynome_Z *multiplication_polynomial_matrix_Z(polynome *polynomial_matrix1, polynome *polynomial_matrix2, unsigned int nb_rows_polynome1, unsigned int nb_columns_polynome2)
{
    polynome_Z *result = (polynome_Z *)malloc(sizeof(polynome_Z) * nb_rows_polynome1);

    int *tmp_line_index = (int *)malloc(nb_columns_polynome2 * sizeof(int));
    int *tmp_line_coeff = (int *)malloc(nb_columns_polynome2 * sizeof(int));

    int value, current_index, index;

    for (int i = 0; i < nb_rows_polynome1; i++)
    {
        current_index = 0;

        for (int j = 0; j < nb_columns_polynome2; j++)
        {
            value = 0;
            for (int k = 0; k < polynomial_matrix1[i].size; k++)
            {
                index = polynomial_matrix1[i].liste_indice[k];
                value += search(&j, polynomial_matrix2[index].liste_indice, polynomial_matrix2[index].size);
            }
            if (value != 0)
            {
                tmp_line_index[current_index] = j;
                tmp_line_coeff[current_index] = value;
                current_index++;
            }
        }

        result[i].size = current_index;
        result[i].liste_indice = (int *)malloc(sizeof(int) * current_index);
        result[i].coeff = (int *)malloc(sizeof(int) * current_index);
        current_index = 0;

        for (int l = 0; l < result[i].size; l++)
        {
            result[i].liste_indice[l] = tmp_line_index[l];
            result[i].coeff[l] = tmp_line_coeff[l];
        }
    }
    free(tmp_line_index);
    free(tmp_line_coeff);
    return result;
}

/**
 * add two polynomial matrix.
 * matrix must be the same dimensions
 *
 * @param matrix1 matrix
 * @param matrix2 matrix
 * @param nb_rows_matrix1 number of columns of both
 * @param nb_columns_matrix2 number of columns of both
 *
 * @return the addition of both (a polynomial matrix)
 */
polynome *add_polymonial_matrix(polynome *polynome1, polynome *polynome2, unsigned int nb_rows_polynome, unsigned int nb_columns_polynome, unsigned int start)
{
    polynome *result = (polynome *)malloc(sizeof(polynome) * nb_rows_polynome);
    int current_index, index;
    int *tmp_line = (int *)malloc(sizeof(int) * nb_columns_polynome);

    for (int i = 0; i < nb_rows_polynome; i++)
    {
        // Each line we check both polynomes
        current_index = 0;
        for (int j = 0; j < polynome1[i].size; j++)
        {
            index = polynome1[i].liste_indice[j];
            // If the index is only on polynome 1 we store in the result polynome
            if (!search(&index, polynome2[i].liste_indice, polynome2[i].size))
            {
                tmp_line[current_index] = index;
                current_index++;
            }
        }
        for (int k = 0; k < polynome2[i].size; k++)
        {
            index = polynome2[i].liste_indice[k];
            /// If the index is only on polynome 2 we store in the result polynome
            if (!search(&index, polynome1[i].liste_indice, polynome1[i].size))
            {
                tmp_line[current_index] = index;
                current_index++;
            }
        }
        result[i].size = current_index;
        result[i].liste_indice = (int *)malloc(sizeof(int) * result[i].size);

        qsort(tmp_line, current_index, sizeof(int), compare_int);

        for (int l = 0; l < current_index; l++)
        {
            result[i].liste_indice[l] = tmp_line[l];
        }
    }
    free(tmp_line);
    return result;
}

/**
 * Convert a polynomial matrix to its binary form.
 *
 * @param matrix matrix to be converted
 * @param nb_rows number of columns
 * @param nb_columns number of columns (we should still have it in the code since a polynomial always comes from a binary)
 *
 * @return the binary matrix representing the polynomial matrix
 */
bit **conversion_polynome_to_binary(polynome *matrix, unsigned int nb_rows, unsigned int nb_columns)
{
    bit **binary_matrix = init_matrix(nb_rows, nb_columns);

    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < matrix[i].size; j++)
            binary_matrix[i][matrix[i].liste_indice[j]].value = 1;
    }
    return binary_matrix;
}