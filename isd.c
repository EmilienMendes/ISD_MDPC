#include "libs/matrix.h"
#include "libs_optimized/matrix_optimized.h"
#include "isd.h"

/**
 * Select a random sample of columns from a binary matrix.
 * Tailored for the Prage algorithm.
 *
 * @param matrix The matrix to select columns from
 * @param matrix_copy The matrix n-k x n-k
 * @param n
 * @param k
 */
void sample_random(bit **matrix, bit **matrix_copy, int n, int k)
{
    int *not_already_picked_index = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        not_already_picked_index[i] = i;

    unsigned int count = 0;
    unsigned int indice;
    while (count != n - k)
    {
        indice = rand() % n;
        if (not_already_picked_index[indice] == -1)
            continue;
        else
        {
            not_already_picked_index[indice] = -1;
            for (int i = 0; i < n - k; i++)
                matrix_copy[i][count].value = matrix[i][indice].value;
            count++;
        }
    }
    free(not_already_picked_index);
}

// Prange algorithm on a random binary matrix
void isd_matrix(int n, int k, int t)
{

    bit **H = init_matrix(n - k, n);
    randomize_matrix(H, n - k, n);
    // e de taille n
    bit **e = init_matrix(n, 1);
    target_weight_matrix(e, n, 1, t);

    bit **s = multiply_matrix(H, e, n - k, n, n, 1);

    bit **H_prime = init_matrix(n - k, n - k);
    bit **e_prime;
    bit **inversion_H_prime;
    int hamming_weight_matrix = -1;
    do
    {
        sample_random(H, H_prime, n, k);

        inversion_H_prime = inversion_matrix(H_prime, n - k, n - k);
        if (inversion_H_prime != NULL)
        {
            e_prime = multiply_matrix(inversion_H_prime, s, n - k, n - k, n - k, 1);
            hamming_weight_matrix = hamming_weight(e_prime, n - k, 1);
            if (hamming_weight_matrix < 80)
                printf("Weight of e_prime : %d \n", hamming_weight_matrix);
        }
        else
        {
            printf("Cannot invert \n");
        }
        free_matrix(inversion_H_prime, n - k);
        free_matrix(e_prime, n - k);

    } while (hamming_weight_matrix != t);
    printf(" Result : %d \n", hamming_weight_matrix);
}

// Prange algorithm on a random unsigned int matrix with bit to bit manipulation
void optimized_isd(int n, int k, int t)
{
    binary_matrix H = init_optimized_matrix(n - k, n);
    randomize_optimized_matrix(H);
    // e de taille n
    binary_matrix e = init_optimized_matrix(n, 1);
    target_weight_optimized_matrix(e, t);

    // transpose H to get correct dimensions
    binary_matrix s = multiply_optimized_matrix(transpose_optimized_matrix(H), e);

    binary_matrix H_prime = init_optimized_matrix(n - k, n - k);
    binary_matrix inversion_H_prime;
    binary_matrix e_prime;
    int hamming_weight_matrix = -1;
    int *inversion = malloc(sizeof(int));
    int boucle = 0;
    do
    {
        H_prime = optimized_sample_random(H, n - k);
        inversion_H_prime = inversion_optimized_matrix(H_prime, inversion);
        if (*(inversion))
        {
            e_prime = multiply_optimized_matrix(inversion_H_prime, s);
            hamming_weight_matrix = optimized_hamming_weight(e_prime);
            if (hamming_weight_matrix < 75)
                printf("Weight of e_prime : %d \n", hamming_weight_matrix);
            free_optimized_matrix(e_prime);
        }

        free_optimized_matrix(H_prime);
        free_optimized_matrix(inversion_H_prime);

    } while (hamming_weight_matrix != t);
    printf(" Result : %d \n", hamming_weight_matrix);
}

int main()
{
    srand(time(NULL));
    int n = 400;
    int k = 200;
    int t = 20;
    // isd_matrix(n, k, t);
    optimized_isd(n, k, t);
}