#include <stdio.h>

#define N 3  // Size of the square vector and single vector

void multiplyVector(int square_vector[N][N], int single_vector[N], int result[N]) 
{
    printf("Square Vector and Single Vector Multiplication Process:\n");

    for (int i = 0; i < N; i++) 
    {
        result[i] = 0;
        printf("\nComputing result[%d] (Row %d of square vector * single vector):\n", i, i);

        for (int j = 0; j < N; j++) 
        {
            printf("    Multiplying square_vector[%d][%d] (value: %d) with single_vector[%d] (value: %d) -> %d * %d = %d\n", 
                    i, j, square_vector[i][j], j, single_vector[j], 
                    square_vector[i][j], single_vector[j], 
                    square_vector[i][j] * single_vector[j]);

            result[i] += square_vector[i][j] * single_vector[j];
        }
        printf("    Sum of row %d = %d (Stored in result[%d])\n", i, result[i], i);
    }
}

int main() 
{
    int square_vector[N][N] = 
    {
        {10, 20, 30},
        {40, 50, 60},
        {70, 80, 90}
    };

    int single_vector[N] = {5, 6, 7};  // Single vector
    int result[N];  // Resultant vector

    multiplyVector(square_vector, single_vector, result);

    // Print the final result
    printf("\nFinal Resultant Vector:\n");
    for (int i = 0; i < N; i++) 
    {
        printf("result[%d] = %d\n", i, result[i]);
    }

    return 0;
}

