#ifndef POLYNOME_H
#define POLYNOME_H

#include "matrix.h"
#include <stdio.h>

/**
 * Represents a polynomial equation in 2.
 */
typedef struct
{
    int *liste_indice; /**< exponent of the polynom which are not null */
    int size;          /**< size of the array liste_indice */
} polynome;

/**
 * Represents a polynomial equation in Z.
 */
typedef struct
{
    int *coeff;        /**< coefficient of the corresponding exponent */
    int *liste_indice; /**< exponent of the polynom which are not null */
    int size;          /**< size of the array liste_indice */
} polynome_Z;

// Creation and Destruction of polynomial Matrix

polynome *init_polynomial_matrix(bit **matrix, unsigned int nb_rows, unsigned int nb_columns);
void free_polynomial_matrix(polynome *polynomial_matrix, unsigned int nb_rows);

void print_polynomial_matrix(polynome *polynomial_matrix, unsigned int nb_rows);
void print_polynomial_matrix_Z(polynome_Z *polynomial_Z_matrix, unsigned int nb_rows);

// Operations on polynomial Matrix

polynome *add_polymonial_matrix(polynome *polynome1, polynome *polynome2, unsigned int nb_rows_polynome1, unsigned int nb_columns_polynome1, unsigned int start);
polynome *inversion_polynomial_matrix(polynome *polynomial_matrix, unsigned int nb_rows, unsigned int nb_columns);
polynome *multiplication_polynomial_matrix(polynome *polynomial_matrix1, polynome *polynomial_matrix2, unsigned int nb_rows_polynome1, unsigned int nb_columns_polynome2);
polynome_Z *multiplication_polynomial_matrix_Z(polynome *polynomial_matrix1, polynome *polynomial_matrix2, unsigned int nb_rows_polynome1, unsigned int nb_columns_polynome2);
bit **conversion_polynome_to_binary(polynome *matrix, unsigned int nb_rows, unsigned int nb_columns);

// Multiplication utilities

int compare_int(const void *a, const void *b);
int search(void *index_to_search, int *array, int nb_element);

#endif
