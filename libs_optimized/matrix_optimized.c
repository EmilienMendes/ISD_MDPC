#include "matrix_optimized.h"

/**
 * All the functions are serving the same purpose as in matrix.c
 * The twist is that we are using binary operation on unsigned int to have a binary matrix.
 * It helps speeding up the algorithm.
 */

binary_matrix init_optimized_matrix(unsigned int nb_rows, unsigned int nb_columns)
{
    binary_matrix matrix;
    matrix.line_size = nb_rows;
    matrix.column_size = nb_columns;

    matrix.array = (unsigned int **)calloc(sizeof(unsigned int *), matrix.line_size);

    for (unsigned int i = 0; i < nb_rows; i++)
    {
        matrix.array[i] = (unsigned int *)calloc(sizeof(unsigned int), matrix.column_size);
    }
    return matrix;
}

void free_optimized_matrix(binary_matrix matrix)
{
    unsigned int nb_rows = matrix.line_size;
    for (int i = 0; i < nb_rows; i++)
    {
        free(matrix.array[i]);
    }
    free(matrix.array);
}

binary_matrix create_optimized_identity_matrix(unsigned int nb_rows)
{
    binary_matrix matrix = init_optimized_matrix(nb_rows, nb_rows);
    unsigned int nb_memory_columns = ceil((float)matrix.column_size / INT_SIZE);
    int column_index;
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        column_index = i / INT_SIZE;
        matrix.array[i][column_index] = matrix.array[i][column_index] ^ (1 << (min(INT_SIZE, nb_rows - column_index * INT_SIZE) - (i % INT_SIZE) - 1));
    }
    return matrix;
}

void optimized_print_matrix(binary_matrix matrix)
{
    unsigned int nb_rows = matrix.line_size;
    unsigned int nb_columns = matrix.column_size;
    unsigned int nb_memory_columns = ceil((float)matrix.column_size / INT_SIZE);
    printf("----------Printing binary matrix----------\n");
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        if (i < 10)
            printf(" A[%d] :", i);
        else
            printf("A[%d] :", i);
        for (unsigned int j = 0; j < nb_memory_columns; j++)
        {
            printf("[");
            for (int count = min(INT_SIZE, nb_columns - j * INT_SIZE) - 1; count >= 0; count--)
            {
                unsigned int value = (matrix.array[i][j] >> count) & 1;
                if (count != 0)
                    printf("%d, ", value);
                else
                    printf("%d] ", value);
            }
        }
        printf("\n");
    }
    printf("----------End of matrix----------\n");
}

binary_matrix copy_optimized_matrix(binary_matrix matrix)
{
    unsigned int nb_rows = matrix.line_size;
    unsigned int nb_columns = matrix.column_size;
    binary_matrix cp_matrix = init_optimized_matrix(nb_rows, nb_columns);

    for (unsigned int i = 0; i < nb_rows; i++)
    {
        for (unsigned int j = 0; j < nb_columns; j++)
        {
            cp_matrix.array[i][j] = matrix.array[i][j];
        }
    }
    return cp_matrix;
}

void target_weight_optimized_matrix(binary_matrix matrix, unsigned int weight)
{
    unsigned int nb_rows = matrix.line_size;
    unsigned int nb_columns = matrix.column_size;
    unsigned int index = 0;
    unsigned int i, j;
    assert(weight < nb_rows * nb_columns);
    while (index < weight)
    {
        i = rand() % nb_rows;
        j = rand() % nb_columns;
        if (((matrix.array[i][j / INT_SIZE] >> (j % INT_SIZE)) & 1) == 0)
        {
            matrix.array[i][j / INT_SIZE] ^= (1 << (j % INT_SIZE));
            index++;
        }
    }
}

void randomize_optimized_matrix(binary_matrix matrix)
{
    unsigned int nb_rows = matrix.line_size;
    unsigned int nb_columns = matrix.column_size;
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        for (unsigned int j = 0; j < nb_columns; j++)
        {
            matrix.array[i][j / INT_SIZE] ^= rand() % 2 << j % INT_SIZE;
        }
    }
}

binary_matrix optimized_sample_random(binary_matrix matrix, int sample_size)
{
    unsigned int nb_rows = matrix.line_size;
    unsigned int nb_columns = matrix.column_size;
    unsigned int nb_memory_columns = ceil((float)matrix.column_size / INT_SIZE);

    int *not_already_picked_index = (int *)malloc(sizeof(int) * nb_columns);
    for (int i = 0; i < nb_columns; i++)
        not_already_picked_index[i] = i;

    binary_matrix result = init_optimized_matrix(nb_rows, sample_size);

    int count = 0;
    int indice;
    while (count != sample_size)
    {
        indice = rand() % matrix.column_size;
        if (not_already_picked_index[indice] == -1)
            continue;
        else
        {
            not_already_picked_index[indice] = -1;
            for (int i = 0; i < nb_rows; i++)
            {
                unsigned int columns_index_original = indice / INT_SIZE;
                unsigned int index_column = count / INT_SIZE;
                unsigned int index_line = i / INT_SIZE;

                unsigned int right_shift_value = ((min(INT_SIZE, nb_columns - columns_index_original * INT_SIZE)) - 1 - indice % INT_SIZE);
                unsigned int left_shift_value = ((min(INT_SIZE, sample_size - index_column * INT_SIZE)) - 1 - count % INT_SIZE);
                unsigned int value = ((matrix.array[i][columns_index_original] >> right_shift_value) & 1);

                value = value << left_shift_value;
                result.array[i][index_column] |= value;
            }
            count++;
        }
    }

    free(not_already_picked_index);
    return result;
}

void add_optimized_matrix(binary_matrix matrix1, binary_matrix matrix2, unsigned int start)
{
    unsigned int nb_rows_1 = matrix1.line_size;
    unsigned int nb_columns_1 = matrix1.column_size;
    assert(nb_rows_1 == matrix2.line_size);
    assert(nb_columns_1 == matrix2.column_size);
    for (int i = 0; i < nb_rows_1; i++)
    {
        optimized_add_line(matrix1.array[i], matrix2.array[i], nb_columns_1);
    }
}

binary_matrix transpose_optimized_matrix(binary_matrix matrix)
{
    unsigned int nb_rows_original = matrix.line_size;
    unsigned int nb_columns_original = matrix.column_size;
    unsigned int nb_memory_columns_original = ceil((float)matrix.column_size / INT_SIZE);

    binary_matrix result = init_optimized_matrix(nb_columns_original, nb_rows_original);
    unsigned int nb_rows_transpose = result.line_size;
    unsigned int nb_columns_transpose = result.column_size;
    unsigned int nb_memory_columns_transpose = ceil((float)result.column_size / INT_SIZE);

    for (int i = 0; i < nb_rows_transpose; i++)
    {
        for (int j = 0; j < nb_rows_original; j++)
        {
            unsigned int index_column = i / INT_SIZE;
            unsigned int index_line = j / INT_SIZE;
            unsigned int value = (matrix.array[j][i / INT_SIZE] >> (min(INT_SIZE, nb_columns_original - index_column * INT_SIZE) - 1 - i % INT_SIZE)) & 1;
            value = value << (min(INT_SIZE, nb_columns_transpose - index_line * INT_SIZE) - 1 - j);
            result.array[i][j / INT_SIZE] ^= value;
        }
    }

    return result;
}

// Not usable because optimized_shift_line is not usable
/*
binary_matrix rotation_optimized_matrix(binary_matrix matrix)
{
    unsigned int nb_rows = matrix.line_size;
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        optimized_shift_line(matrix.array[i], matrix.column_size, 1);
    }
}*/

binary_matrix concatenation_optimized_matrix(binary_matrix matrix1, binary_matrix matrix2)
{
    assert(matrix1.line_size == matrix2.line_size);
    unsigned int nb_rows = matrix1.line_size;
    unsigned int nb_columns1 = matrix1.column_size;
    unsigned int nb_columns2 = matrix2.column_size;
    binary_matrix concatenated_matrix = init_optimized_matrix(nb_rows, nb_columns1 + nb_columns2);
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        // Each completed 32 bit element of matrix1
        for (unsigned int j = 0; j < nb_columns1 / INT_SIZE; j++)
        {
            concatenated_matrix.array[i][j] = matrix1.array[i][j];
        }
        unsigned int shift = nb_columns1 % INT_SIZE;
        unsigned int started_index = nb_columns1 / INT_SIZE;

        // Matrix 1 is not exactly a multiple of sizeof unsigned int
        if (shift)
        {
            if (nb_columns2 < INT_SIZE)
            {
                int shift_matrix_2 = INT_SIZE - min(INT_SIZE, nb_columns2);
                concatenated_matrix.array[i][started_index] = (matrix1.array[i][started_index] << shift) ^ ((matrix2.array[i][0] << shift_matrix_2) >> shift_matrix_2);
                return concatenated_matrix;
            }
            concatenated_matrix.array[i][started_index] = (matrix1.array[i][started_index] << (INT_SIZE - shift)) ^ matrix2.array[i][0] >> shift;
            started_index++;
            unsigned int stop_index_cols_2 = nb_columns2 / INT_SIZE;
            for (unsigned int j = 0; j < stop_index_cols_2 - 1; j++)
            {
                concatenated_matrix.array[i][started_index + j] = (matrix2.array[i][j] << INT_SIZE - shift) ^ (matrix2.array[i][j + 1] >> shift);
            }
            concatenated_matrix.array[i][started_index + stop_index_cols_2 - 1] = (matrix2.array[i][stop_index_cols_2 - 1]) << shift ^ (matrix2.array[i][stop_index_cols_2]);
        }
        else
        {
            for (unsigned int j = 0; j < nb_columns2; j++)
            {
                concatenated_matrix.array[i][started_index + j] = matrix2.array[i][j];
            }
        }
    }
    return concatenated_matrix;
}

binary_matrix inversion_optimized_matrix(binary_matrix m, int *result)
{
    unsigned int nb_rows = m.line_size;
    unsigned int nb_columns = m.column_size;
    unsigned int nb_memory_columns = ceil((float)nb_columns / INT_SIZE);

    binary_matrix matrix = copy_optimized_matrix(m);
    binary_matrix inverted_matrix = create_optimized_identity_matrix(nb_columns);
    for (int i = 0; i < nb_columns; i++)
    {
        int maxi = i;
        int index_column = i / INT_SIZE;

        for (int k = i; k < nb_rows; k++)
        {
            if (((matrix.array[k][index_column] >> min(INT_SIZE, nb_columns - index_column * INT_SIZE) - i % INT_SIZE - 1) & 1) != 0)
            {
                maxi = k;
            }
        }

        if (((matrix.array[maxi][index_column] >> min(INT_SIZE, nb_columns - index_column * INT_SIZE) - i % INT_SIZE - 1) & 1) != 0)
        {
            optimized_swap_lines(matrix, i, maxi);
            optimized_swap_lines(inverted_matrix, i, maxi);
            for (int u = i + 1; u < nb_columns; u++)
            {
                if (((matrix.array[u][index_column] >> min(INT_SIZE, nb_columns - index_column * INT_SIZE) - i % INT_SIZE - 1) & 1) != 0)
                {
                    optimized_add_line(matrix.array[u], matrix.array[i], nb_columns);
                    optimized_add_line((inverted_matrix).array[u], (inverted_matrix).array[i], nb_columns);
                }
            }
        }
        else
        {
            // More than one solution exists
            free_optimized_matrix(matrix);
            *(result) = 0;
            return inverted_matrix;
        }
    }

    int is_inversible;
    if (is_inversible = optimized_matrix_is_upper(matrix))
    {
        int current_column = nb_columns - 1;
        int index = 0;
        while (current_column != 0)
        {
            int column_index = current_column / INT_SIZE;
            for (int i = current_column - 1; i >= 0; i--)
            {
                if (((matrix.array[i][column_index] >> index % INT_SIZE) & 1) == 1)
                {
                    optimized_add_line(matrix.array[i], matrix.array[current_column], nb_columns);
                    optimized_add_line((inverted_matrix).array[i], (inverted_matrix).array[current_column], nb_columns);
                }
            }
            if ((current_column) % INT_SIZE == 0)
                index = 0;
            else
                index++;
            current_column--;
        }
    }

    free_optimized_matrix(matrix);
    *(result) = is_inversible;
    return inverted_matrix;
}

binary_matrix multiply_optimized_matrix(binary_matrix matrix1, binary_matrix matrix2)
{
    unsigned int nb_rows_1 = matrix1.line_size;
    unsigned int nb_rows_2 = matrix1.line_size;
    unsigned int nb_columns_1 = matrix1.column_size;
    unsigned int nb_columns_2 = matrix2.column_size;
    unsigned int nb_memory_columns_1 = ceil((float)nb_columns_1 / INT_SIZE);
    unsigned int nb_memory_columns_2 = ceil((float)nb_columns_2 / INT_SIZE);

    binary_matrix result_matrix = init_optimized_matrix(nb_rows_1, nb_columns_2);
    for (int i = 0; i < nb_rows_1; i++)
    {
        for (int j = 0; j < nb_columns_2; j++)
        {
            int true_column_m2 = j / INT_SIZE;
            unsigned int value = 0;
            for (int k = 0; k < nb_columns_1; k++)
            {
                int true_column_m1 = k / INT_SIZE;
                unsigned int first_value = ((matrix1.array[i][true_column_m1] >> min(INT_SIZE, nb_columns_1 - true_column_m1 * INT_SIZE) - k % INT_SIZE - 1) & 1);
                unsigned int second_value = ((matrix2.array[k][true_column_m2] >> min(INT_SIZE, nb_columns_2 - true_column_m2 * INT_SIZE) - j % INT_SIZE - 1) & 1);
                value ^= first_value & second_value;
            }
            result_matrix.array[i][j / INT_SIZE] ^= (value << min(INT_SIZE, nb_columns_2 - true_column_m2 * INT_SIZE) - j % INT_SIZE - 1);
        }
    }
    return result_matrix;
}

int optimized_matrix_is_upper(binary_matrix matrix)
{
    unsigned int nb_rows = matrix.line_size;
    unsigned int nb_columns = matrix.column_size;
    unsigned int nb_memory_columns = ceil((float)nb_columns / INT_SIZE);
    for (int i = 0; i < nb_rows; i++)
    {
        int index_column = i / INT_SIZE;
        for (int j = 0; j < index_column; j++)
        {

            if (matrix.array[i][j] != 0)
                return 0;
        }

        if (i % INT_SIZE == 0)
        {
            continue;
        }
        if (matrix.array[i][index_column] > (1 << min(INT_SIZE, nb_columns - index_column * INT_SIZE) - i % INT_SIZE) + 1)
            return 0;
    }
    return 1;
}

unsigned int optimized_hamming_weight(binary_matrix matrix)
{
    unsigned int nb_rows = matrix.line_size;
    unsigned int nb_columns = matrix.column_size;
    unsigned int nb_memory_columns = ceil((float)matrix.column_size / INT_SIZE);

    unsigned int count = 0;
    for (unsigned int i = 0; i < nb_rows; i++)
    {
        for (unsigned int j = 0; j < nb_memory_columns; j++)
        {
            unsigned int value = matrix.array[i][j];
            for (unsigned int k = 0; k < INT_SIZE; k++)
            {
                if (value & 1)
                    count++;
                value = value >> 1;
            }
        }
    }
    return count;
}

int optimized_is_matrix_null(binary_matrix matrix)
{
    unsigned int nb_rows = matrix.line_size;
    unsigned int nb_columns = matrix.column_size;
    unsigned int nb_memory_columns = ceil((float)nb_columns / INT_SIZE);
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_memory_columns; j++)
        {
            if (matrix.array[i][j] != 0)
                return 0;
        }
    }
    return 1;
}

// Not usable
/*unsigned int *optimized_shift_line(unsigned int *initial_line, unsigned int nb_columns, unsigned int shift_size)
{
    unsigned int nb_memory_columns = ceil((float)nb_columns / INT_SIZE);
    unsigned int *return_line = (unsigned int *)malloc(sizeof(unsigned int) * nb_memory_columns);
    unsigned int size_last_line = nb_columns % INT_SIZE;

    if (shift_size % INT_SIZE != 0)
    {
        int index = 0;
        int start_value = shift_size / INT_SIZE;
        // Shift until the last bloc
        for (int i = 0; i < (int)floor((float)(nb_columns - shift_size) / INT_SIZE) - 1; i++)
        {
            printf("A[0][%d] : On prend %ld bit du bloc %d et %ld bit du bloc %d \n", index, INT_SIZE - shift_size % INT_SIZE, i + start_value, shift_size % INT_SIZE, i + 1 + start_value);
            return_line[index] = ((initial_line[i + start_value]) << shift_size % INT_SIZE) ^ (initial_line[i + 1 + start_value] >> INT_SIZE - (shift_size % INT_SIZE));
            index++;
        }

        int new_shift = (nb_columns - shift_size) % INT_SIZE;
        printf("A[0][%d] : On prend %ld bit du bloc %d et %ld bit du bloc %d (shift de %d ) \n", index, INT_SIZE - shift_size % INT_SIZE, (index + start_value) % nb_memory_columns, shift_size % INT_SIZE, (index + start_value + 1) % nb_memory_columns, new_shift);

        return_line[index] = (initial_line[(index + start_value) % nb_memory_columns] << (shift_size % INT_SIZE)) ^ (initial_line[(index + start_value + 1) % nb_memory_columns] >> new_shift);
        index++;

        if (size_last_line > INT_SIZE - (shift_size % INT_SIZE))
        {
            printf("A[0][%d] : On prend %d bit du bloc %d et %ld bit du bloc %d  \n", index, new_shift, nb_memory_columns - 1, INT_SIZE - new_shift, 0);
            return_line[index] = (initial_line[nb_memory_columns - 1] << INT_SIZE - new_shift) ^ (initial_line[0] >> new_shift);
        }
        else
        {
            printf("A[0][%d] : On prend %d bit du bloc %d et %ld bit du bloc %d  \n", index, new_shift, nb_memory_columns - 1, INT_SIZE - new_shift, 0);
            return_line[index] = (initial_line[nb_memory_columns - 1] << INT_SIZE - new_shift) ^ (initial_line[0] >> new_shift);
        }

        index++;

        int start = index + start_value;
        for (index; index < nb_memory_columns - 1; index++)
        {
            printf("A[0][%d] : On prend %d bit du bloc %d et %ld bit du bloc %d \n", index, new_shift, (index + start_value) % nb_memory_columns, INT_SIZE - new_shift, (index + start_value + 1) % nb_memory_columns);
            return_line[index] = (initial_line[(index + start_value) % nb_memory_columns] << (INT_SIZE - new_shift)) ^ (initial_line[(index + start_value + 1) % nb_memory_columns] >> new_shift);
        }
        printf("A[0][%d] : On prend %d bit du bloc %d et %ld bit du bloc %d \n", index, new_shift, (index + start_value) % nb_memory_columns, shift_size % INT_SIZE, (index + start_value + 1) % nb_memory_columns);
        return_line[index] = (initial_line[(shift_size / INT_SIZE) - 1] << size_last_line - new_shift) ^ (initial_line[shift_size / INT_SIZE] >> INT_SIZE - (shift_size % INT_SIZE));
    }
    // Shift multiple of 32
    else
    {
        // For loop until we reach the final bloc of smaller size
        for (unsigned int i = 0; i < nb_memory_columns - 1 - (shift_size / INT_SIZE); i++)
        {
            return_line[i] = initial_line[i + shift_size / INT_SIZE];
        }
        for (unsigned int i = nb_memory_columns - 1 - (shift_size / INT_SIZE); i < nb_memory_columns - 1; i++)
        {
            return_line[i] = initial_line[(i + shift_size / INT_SIZE) % nb_memory_columns] << INT_SIZE - size_last_line ^ initial_line[(i + 1 + shift_size / INT_SIZE) % nb_memory_columns] >> size_last_line;
        }
        return_line[nb_memory_columns - 1] = initial_line[(shift_size / INT_SIZE) - 1];
    }

    return return_line;
}*/

void optimized_swap_lines(binary_matrix matrix, unsigned int line1, unsigned int line2)
{
    unsigned int *temp_line = matrix.array[line1];
    matrix.array[line1] = matrix.array[line2];
    matrix.array[line2] = temp_line;
}

void optimized_add_line(unsigned int *line1, unsigned int *line2, unsigned int nb_columns)
{
    unsigned int nb_memory_columns = ceil((float)nb_columns / INT_SIZE);

    for (unsigned int i = 0; i < nb_memory_columns; i++)
    {
        line1[i] ^= line2[i];
    }
}

void print_line(unsigned int *line, unsigned int nb_columns)
{
    unsigned int nb_memory_columns = ceil((float)nb_columns / INT_SIZE);
    printf("Line : \n");
    for (unsigned int i = 0; i < nb_memory_columns; i++)
    {
        printf("A[0][%u] : [", i);
        for (int count = min(INT_SIZE, nb_columns - i * INT_SIZE) - 1; count >= 0; count--)
        {
            unsigned int value = (line[i] >> count) & 1;
            if (count != 0)
                printf("%d, ", value);
            else
                printf("%d]\n", value);
        }
    }
}

void test_optimized_print_matrix()
{
    binary_matrix test_matrix_print = init_optimized_matrix(2, 3);
    test_matrix_print.array[0][0] = 3;
    test_matrix_print.array[1][0] = 1;

    binary_matrix test_matrix_weight = init_optimized_matrix(4, 8);
    test_matrix_weight.array[0][0] = 5;
    test_matrix_weight.array[1][0] = 10;
    test_matrix_weight.array[2][0] = 64;
    test_matrix_weight.array[3][0] = 255;

    // Should print 0 1 1 \n 0 0 1
    printf("----------@RUNNING TEST : optimized_print_matrix----------\n");
    optimized_print_matrix(test_matrix_print);

    free_optimized_matrix(test_matrix_print);
    free_optimized_matrix(test_matrix_weight);
}

void test_hamming_weight()
{
    binary_matrix test_matrix_print = init_optimized_matrix(2, 3);
    test_matrix_print.array[0][0] = 3;
    test_matrix_print.array[1][0] = 1;

    binary_matrix test_matrix_weight = init_optimized_matrix(4, 8);
    test_matrix_weight.array[0][0] = 5;
    test_matrix_weight.array[1][0] = 10;
    test_matrix_weight.array[2][0] = 64;
    test_matrix_weight.array[3][0] = 255;

    binary_matrix test_matrix_target = init_optimized_matrix(200, 400);
    target_weight_optimized_matrix(test_matrix_target, 125);

    printf("----------@RUNNING TEST : hamming_weight | target_weight----------\n");
    // Should be 3
    printf("Hamming weight of test_matrix_print : %d \n", optimized_hamming_weight(test_matrix_print));
    // Should be 13
    printf("Hamming weight of test_matrix_weight : %d \n", optimized_hamming_weight(test_matrix_weight));
    // Should be 125
    printf("Hamming weight of test_matrix_target : %d \n", optimized_hamming_weight(test_matrix_target));

    free_optimized_matrix(test_matrix_print);
    free_optimized_matrix(test_matrix_target);
    free_optimized_matrix(test_matrix_weight);
}

void test_add_line()
{
    binary_matrix test_matrix_add = init_optimized_matrix(2, 3);
    test_matrix_add.array[0][0] = 3;
    test_matrix_add.array[1][0] = 1;

    binary_matrix test_matrix_add2 = init_optimized_matrix(4, 8);
    test_matrix_add2.array[0][0] = 255;
    test_matrix_add2.array[1][0] = 1;
    test_matrix_add2.array[2][0] = 1;
    test_matrix_add2.array[3][0] = 255;

    free_optimized_matrix(test_matrix_add);
    free_optimized_matrix(test_matrix_add2);
    printf("---------@RUNNING TEST : add_line----------\n");
    optimized_add_line(test_matrix_add.array[0], test_matrix_add.array[1], test_matrix_add.column_size);
    // Should print 0 1 0\n 0 0 1
    optimized_print_matrix(test_matrix_add);
    optimized_add_line(test_matrix_add2.array[0], test_matrix_add2.array[3], test_matrix_add2.column_size);
    // Should print ::\n 0 0 1 \n 0 0 1 \n 1 1 1 1 1 1 1 1
    optimized_print_matrix(test_matrix_add2);
}

void test_concatenation_matrix()
{
    printf("----------@RUNNING TEST : concatenation_matrix----------\n");
    binary_matrix test_matrix_concat = init_optimized_matrix(10, 33);
    binary_matrix test_matrix_concat2 = init_optimized_matrix(10, 66);
    test_matrix_concat2.array[0][0] = 255;
    test_matrix_concat2.array[0][1] = 38;
    test_matrix_concat2.array[0][2] = 79;
    binary_matrix test_concatenated_matrix = concatenation_optimized_matrix(test_matrix_concat, test_matrix_concat2);
    optimized_print_matrix(test_concatenated_matrix);
    free_optimized_matrix(test_matrix_concat);
    free_optimized_matrix(test_matrix_concat2);
    free_optimized_matrix(test_concatenated_matrix);
}

/*void test_shift_line()
{
    printf("----------@RUNNING TEST : shift_line----------\n");
    int binary_size_test_matrix_shift = 250;
    binary_matrix test_matrix_shift = init_optimized_matrix(2, binary_size_test_matrix_shift);
    target_weight_optimized_matrix(test_matrix_shift, binary_size_test_matrix_shift);
    optimized_print_matrix(test_matrix_shift);
    printf("Shift > 64\n");
    int *line = optimized_shift_line(test_matrix_shift.array[0], test_matrix_shift.column_size, 65);
    print_line(line, binary_size_test_matrix_shift);
    printf("Shift < 32\n");
    int *line2 = optimized_shift_line(test_matrix_shift.array[0], test_matrix_shift.column_size, 27);
    print_line(line2, binary_size_test_matrix_shift);
    free_optimized_matrix(test_matrix_shift);
}*/

void test_identity_matrix()
{
    printf("----------@RUNNING TEST : Identity Matrix----------\n");
    binary_matrix identity = create_optimized_identity_matrix(35);
    optimized_print_matrix(identity);
}

void test_transpose_matrix()
{
    printf("----------@RUNNING TEST : Transpose----------\n");
    binary_matrix test_matrix_transpose = init_optimized_matrix(36, 40);
    target_weight_optimized_matrix(test_matrix_transpose, 600);
    optimized_print_matrix(test_matrix_transpose);
    binary_matrix transpose_result_test = transpose_optimized_matrix(test_matrix_transpose);
    optimized_print_matrix(transpose_result_test);
    free_optimized_matrix(test_matrix_transpose);
    free_optimized_matrix(transpose_result_test);
}

void test_multiplication_matrix()
{
    printf("----------@RUNNING TEST : multiplication----------\n");
    binary_matrix test_matrix_multiplication = init_optimized_matrix(39, 36);
    target_weight_optimized_matrix(test_matrix_multiplication, 600);
    printf("Matrix 1 :\n");
    optimized_print_matrix(test_matrix_multiplication);
    binary_matrix test_matrix_multiplication2 = init_optimized_matrix(36, 5);
    target_weight_optimized_matrix(test_matrix_multiplication2, 100);
    printf("Matrix 2 :\n");
    optimized_print_matrix(test_matrix_multiplication2);
    binary_matrix multipliee = multiply_optimized_matrix(test_matrix_multiplication, test_matrix_multiplication2);
    printf("Matrix multipliee :\n");
    optimized_print_matrix(multipliee);

    free_optimized_matrix(test_matrix_multiplication);
    free_optimized_matrix(test_matrix_multiplication2);
    free_optimized_matrix(multipliee);
}

void test_inversion_matrix()
{
    printf("----------@RUNNING TEST : Inversion----------\n");
    int *inversion_found = malloc(sizeof(int));
    *(inversion_found) = 0;
    binary_matrix inverted_matrix;

    binary_matrix matrix_inversible;
    while (!inversion_found)
    {
        matrix_inversible = init_optimized_matrix(40, 40);

        target_weight_optimized_matrix(matrix_inversible, 600);

        inverted_matrix = inversion_optimized_matrix(matrix_inversible, inversion_found);
    }
    printf("Matrix :\n");
    optimized_print_matrix(matrix_inversible);
    printf("Inverted Matrix :\n");
    optimized_print_matrix(inverted_matrix);
    binary_matrix result_matrix = multiply_optimized_matrix(matrix_inversible, inverted_matrix);
    printf("Matrix x Inverted Matrix \n");
    optimized_print_matrix(result_matrix);

    free_optimized_matrix(inverted_matrix);
}

void test_sample_random()
{
    printf("----------@RUNNING TEST : sample_random----------\n");
    binary_matrix test_matrix_sample_random = init_optimized_matrix(20, 40);
    target_weight_optimized_matrix(test_matrix_sample_random, 400);
    optimized_print_matrix(test_matrix_sample_random);
    binary_matrix sample_random_result_test = optimized_sample_random(test_matrix_sample_random, 20);
    optimized_print_matrix(sample_random_result_test);
    free_optimized_matrix(test_matrix_sample_random);
    free_optimized_matrix(sample_random_result_test);
}
