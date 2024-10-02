#include "mdpc.h"

/**
 * Display of the hash in hexadecilal
 * @param buff hash that will be display
 */

void print_hash(uint8 *buff)
{
    for (int i = 0; i < MD5_HASH_BYTES; i++)
    {
        printf("%02x", buff[i]);
    }
    printf("\n");
}

/**
 * Generation of the hash
 *
 * @param matrix matrix used for the hash
 * @param nb_rows rows of the matrix
 * @return hash of the matrix
 */
void hash(bit **matrix, unsigned int nb_rows)
{
    char *my_hash = (char *)malloc(sizeof(char) * nb_rows);
    for (int i = 0; i < nb_rows; i++)
    {
        my_hash[i] = matrix[i][0].value;
    }
    uint8 buff[MD5_HASH_BYTES];
    calcul_md5(my_hash, MD5_HASH_BYTES, buff);
    memcpy(my_hash, buff, MD5_HASH_BYTES);
    print_hash(buff);
    free(my_hash);
}

/**
 * Generation of the private key
 * @param h0 first part of the private key
 * @param h1 second part of the private key
 * @param nb_rows number of rows of the private key
 * @param nb_columns number of columns of the private key
 *
 */
void privkey_generation(bit **h0, bit **h1, unsigned int nb_rows, unsigned int nb_columns, unsigned int weight, clock_t *start, clock_t *end)
{
    *start = clock();
    bit **first_line_h0 = init_matrix(1, nb_columns);
    bit **first_line_h1 = init_matrix(1, nb_columns);
    // Create the first line of each matrix that will be used for the permutation
    target_weight_matrix(first_line_h0, 1, nb_columns, weight);
    target_weight_matrix(first_line_h1, 1, nb_columns, weight);

    unsigned int random_shift = rand() % nb_columns;
    // Permutation of each line using the initialisation
    for (int i = 0; i < nb_rows; i++)
    {
        shift_line(h0[i], first_line_h0[0], nb_columns, (i + 1) * random_shift);
        shift_line(h1[i], first_line_h1[0], nb_columns, (i + 1) * random_shift);
    }

    *end = clock();
    free_matrix(first_line_h0, 1);
    free_matrix(first_line_h1, 1);
}

/**
 * Generate public key from the private key
 *
 * @param h0 the first part of the private key
 * @param h1 the second part of the private key
 * @param nb_rows number of rows of the private key
 * @param nb_columns number of columns of the private key
 * @return the multiplication of both (a polynomial matrix)
 */
polynome *pubkey_generation(bit **h0, bit **h1, unsigned int nb_rows, unsigned int nb_columns, clock_t *start, clock_t *end)
{
    *start = clock();
    bit **inversion_h0 = inversion_matrix(h0, nb_rows, nb_columns);
    // Switching h0^-1 and h1 to polynome for the multiplication
    polynome *polynome_matrix_A = init_polynomial_matrix(inversion_h0, nb_rows, nb_columns);
    polynome *polynome_matrix_B = init_polynomial_matrix(h1, nb_rows, nb_columns);
    // Private key
    polynome *h = multiplication_polynomial_matrix(polynome_matrix_A, polynome_matrix_B, nb_rows, nb_columns);

    *end = clock();
    free_matrix(inversion_h0, nb_rows);
    free_polynomial_matrix(polynome_matrix_A, nb_rows);
    free_polynomial_matrix(polynome_matrix_B, nb_rows);

    return h;
}

// https://eprint.iacr.org/2019/1423.pdf
int bitflip(int n, bit **e0, bit **e1, bit **h0, bit **h1, bit **c, int T, int t, bit **e0_output, bit **e1_output)
{
    // Initialisation of all parameters needed
    bit **s = multiply_matrix(h0, c, n, n, n, 1);

    bit **u = init_matrix(1, n);
    bit **v = init_matrix(1, n);

    bit **transpose_h0 = transpose_matrix(h0, n, n);
    bit **transpose_h1 = transpose_matrix(h1, n, n);
    bit **H = concatenation_matrix(rotation_matrix(transpose_h0, n, n, 1, RIGHT), rotation_matrix(transpose_h1, n, n, 1, RIGHT), n, n, n, n);
    bit **syndrome = copy_matrix(s, n, 1);
    bit **flipped_positions;
    int **sum;

    while ((hamming_weight(u, 1, n) != t || hamming_weight(v, 1, n) != t) && (hamming_weight(syndrome, n, 1) != 0))
    {
        sum = multiply_non_binary_matrix(transpose_matrix(syndrome, n, 1), H, 1, n, n, 2 * n);

        flipped_positions = init_matrix(1, 2 * n);
        for (int j = 0; j < 2 * n; j++)
        {
            if (sum[0][j] >= T)
            {
                flipped_positions[0][j].value = 1;
            }
        }
        printf("|u|| : %d ||v|| : %d  ||syndrome|| : %d  \n", hamming_weight(u, 1, n), hamming_weight(v, 1, n), hamming_weight(syndrome, n, 1));
        // XOR between flipped positions and <u,v>
        add_matrix(u, flipped_positions, 1, n, 0);
        add_matrix(v, flipped_positions, 1, n, n);
        // New value of syndrome based on flipped position
        add_matrix(syndrome, multiply_matrix(H, transpose_matrix(flipped_positions, 1, 2 * n), n, 2 * n, 2 * n, 1), n, 1, 0);
    }
    // Verification that result is correct
    bit **matrix_to_sub = multiply_matrix(H, transpose_matrix(concatenation_matrix(u, v, 1, n, 1, n), 1, 2 * n), n, 2 * n, 2 * n, 1);
    add_matrix(matrix_to_sub, s, n, 1, 0);

    int matrix_null = !is_matrix_null(matrix_to_sub, n, 1);
    // Store the result in e0_output and e1_output if result is correct
    if (matrix_null)
    {
        e0_output = NULL;
        e1_output = NULL;
    }
    else
    {
        e0_output = transpose_matrix(u, 1, n);
        e1_output = transpose_matrix(v, 1, n);
    }

    free_matrix(transpose_h0, n);
    free_matrix(transpose_h1, n);
    free_matrix(syndrome, n);
    free_matrix(H, n);
    free_matrix(u, n);
    free_matrix(v, n);
    return matrix_null;
}

/**
 * Cypher the error generated by the public key
 * @param e0 first error generated
 * @param e1 first error generated
 * @param h0 first part of private key
 * @param h1 first part of second key
 * @param pubkey public key
 * @param n size of matrix
 * @param e weight of the error
 *
 * @return the cypher matrix
 */

bit **cypher(bit **e0, bit **e1, bit **h0, bit **h1, polynome *pubkey, int n, int e, clock_t *start, clock_t *end)
{
    *start = clock();
    // Creating the two errors of weight w = e/2
    target_weight_matrix(e0, n, 1, e / 2);
    target_weight_matrix(e1, n, 1, e / 2);

    // Changing matrix to polynome
    polynome *polynome_e0 = init_polynomial_matrix(e0, n, 1);
    polynome *polynome_e1 = init_polynomial_matrix(e1, n, 1);
    polynome *polynome_h0 = init_polynomial_matrix(h0, n, n);
    polynome *polynome_h1 = init_polynomial_matrix(h1, n, n);

    // Creating the cypher matrix
    polynome *tmp = multiplication_polynomial_matrix(pubkey, polynome_e1, n, 1);
    polynome *c1 = add_polymonial_matrix(polynome_e0, tmp, n, 1, 0);
    bit **c = conversion_polynome_to_binary(c1, n, 1);
    *end = clock();

    hash(e0, n);
    hash(e1, n);

    free_polynomial_matrix(polynome_e0, 1);
    free_polynomial_matrix(polynome_e1, 1);
    free_polynomial_matrix(polynome_h0, n);
    free_polynomial_matrix(polynome_h1, n);
    return c;
}

/**
 * MDPC Algorithm
 * @param w weight of each matrix
 * @param n size of matrix
 * @param e total weight of the error
 * @param T treshold for flipped bits
 * 
*/

void mdpc(unsigned int w, unsigned int n, unsigned int e, unsigned int T)
{
    clock_t start, end;
    // Alice
    bit **h0 = init_matrix(n, n);
    bit **h1 = init_matrix(n, n);

    // Generation of keys
    privkey_generation(h0, h1, n, n, w, &start, &end);
    printf("Time for generating private key : %ld ms \n", (end - start) / 1000);
    polynome *pubkey = pubkey_generation(h0, h1, n, n, &start, &end);
    printf("Time for generating public key : %ld ms \n", (end - start) / 1000);

    bit **e0 = init_matrix(n, 1);
    bit **e1 = init_matrix(n, 1);
    // Bob
    bit **c = cypher(e0, e1, h0, h1, pubkey, n, e, &start, &end);
    printf("Time for cypher  : %ld ms \n", (end - start) / 1000);

    bit **e0_alice;
    bit **e1_alice;

    if (bitflip(n, e0, e1, h0, h1, c, T, w, e0_alice, e1_alice))
    {
        printf("Alice a reussi a decode e0 et e1\n");
    }

    free_matrix(h0, n);
    free_matrix(h1, n);
    free_matrix(e0, 1);
    free_matrix(e1, 1);

    free_polynomial_matrix(pubkey, n);
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    int w = 39;
    int n = 4813;
    int e = 78;
    int T = 26;
    printf("Launch of MPDC for w = %d, n = %d, e = %d, T = %d\n", w, n, e, T);
    mdpc(w, n, e, T);
}
