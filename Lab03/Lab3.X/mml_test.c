// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"


// Microchip libraries
#include <xc.h>

// User libraries:
#include "MatrixMath.h"

// Module-level variables:
float result[3][3];
float submatrixResult[2][2];
float zero_matrix[3][3] = {
    {},
    {},
    {}
};
float matrix1[3][3] = {
    {1, 2, 3},
    {1, 2, 3},
    {1, 2, 3}
};
float matrix2[3][3] = {
    {1, 1, 1},
    {2, 2, 2},
    {3, 3, 3}
};
float matrix3[3][3] = {
    {1, 2, 1},
    {3, 2, 4},
    {3, 2, 5}
};
float matrix4[3][3] = {
    {1, 2, 1},
    {7, 2, 4},
    {8, 2, 5}
};
float matrix5[3][3] = {
    {-1, 2, 1},
    {3, -2, 4},
    {-3, 2, -5}
};
float matrix6[3][3] = {
    {1.02, 2.71, 1},
    {7, 2.51, 4},
    {8, 2, 5.03}
};
float matrix7[3][3] = {
    {1, 2, 1},
    {2, 5, 4},
    {1, 1, 0}
};
float matrix8[3][3] = {
    {-0.0101, -0.0018, 0.0178},
    {0.0107, 0.0071, -0.0035},
    {0.0089, 0.0056, -0.0048}
};
float matrix9[3][3] = {
    {-0.0007, 0.0021, 0.0001},
    {-0.0342, 0.0024, 0.0016},
    {0.0332, -0.0026, -0.0001}
};
float matrixadd1[3][3] = {
    {2, 3, 4},
    {3, 4, 5},
    {4, 5, 6}
};
float matrixadd2[3][3] = {
    {2, 4, 6},
    {2, 4, 6},
    {2, 4, 6}
};
float matrixmultiply1[3][3] = {
    {14, 14, 14},
    {14, 14, 14},
    {14, 14, 14}
};
float matrixmultiply2[3][3] = {
    {6, 12, 18},
    {6, 12, 18},
    {6, 12, 18}
};
float matrixscalarmul1[3][3] = {
    {2, 4, 6},
    {2, 4, 6},
    {2, 4, 6}
};
float matrixscalarmul2[3][3] = {
    {3, 6, 9},
    {3, 6, 9},
    {3, 6, 9}
};
float matrixscalaradd1[3][3] = {
    {3, 4, 5},
    {3, 4, 5},
    {3, 4, 5}
};
float matrixscalaradd2[3][3] = {
    {4, 4, 4},
    {5, 5, 5},
    {6, 6, 6}
};
float matrixtranspose1[3][3] = {
    {1, 1, 1},
    {2, 2, 2},
    {3, 3, 3}
};
float matrixtranspose2[3][3] = {
    {1, 2, 3},
    {1, 2, 3},
    {1, 2, 3}
};
float matrixsubmatrix1[2][2] = {
    {2, 3},
    {2, 3}
};
float matrixsubmatrix2[2][2] = {
    {1, 1},
    {3, 3}
};
float matrixinverse1[3][3] = {
    {-0.5, 2, -1.5},
    {0.75, -0.5, 0.25},
    {0, -1, 1}
};
float matrixinverse2[3][3] = {
    {-0.33333333, 1.333333, -1},
    {0.5, 0.5, -0.5},
    {0.3333333, -2.3333333, 2}
};
float matrixinverse3[3][3] = {
    {0.5, 3, 2.5},
    {.75, 2, 1.75},
    {0, -1, -1}
};
float matrixinverse4[3][3] = {
    {-0.4597122398, 1.15604, -0.82793},
    {0.319044449, 0.28519, -0.29022},
    {0.6042960279, -1.95204, 1.63098}
};
float a, b = 0, c = 0, d = 0;
int i = 0, j = 0;

//void SubmatrixPrint(float mat[2][2]); // checks submatrixes
//int SubmatrixEquals(float mat1[2][2], float mat2[2][2]); // function prints submatrixes

int main(void)
{
    BOARD_Init();

    printf("Beginning kmzhang's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);

    a = MatrixEquals(matrix1, matrix1); // matrix1 with matrix1
    if (a != 0) {
        b += 1;
    }
    a = MatrixEquals(matrix2, matrix2); // matrix2 with matrix2
    if (a != 0) {
        b += 1;
    }
    a = MatrixEquals(matrix1, matrix2); // matrix2 with matrix1
    if (a == 0) {
        b += 1;
    }
    if (b == 3) {
        c += 1;
    }
    printf("PASSED (%1.0f/3): MatrixEquals()\n", (double) b);

    b = 0;
    MatrixAdd(matrix1, matrix2, result); // matrix1 + matrix2
    a = MatrixEquals(matrixadd1, result);
    if (a != 0) {
        b += 1;
    }
    MatrixAdd(matrix1, matrix1, result); //matrix1 + matrix1
    a = MatrixEquals(matrixadd2, result);
    if (a != 0) {
        b += 1;
    }
    if (b == 2) {
        c += 1;
    }
    printf("PASSED (%1.0f/2): MatrixAdd()\n", (double) b);

    b = 0;
    MatrixMultiply(matrix1, matrix2, result); //matrix1 * matrix2
    a = MatrixEquals(matrixmultiply1, result);
    if (a != 0) {
        b += 1;
    }
    MatrixMultiply(matrix1, matrix1, result); //matrix1 * matrix1
    a = MatrixEquals(matrixmultiply2, result);
    if (a != 0) {
        b += 1;
    }
    if (b == 2) {
        c += 1;
    }
    printf("PASSED (%1.0f/2): MatrixMultiply()\n", (double) b);

    b = 0;
    MatrixScalarAdd(2, matrix1, result); // 2 + matrix1
    a = MatrixEquals(matrixscalaradd1, result);
    if (a != 0) {
        b += 1;
    }
    MatrixScalarAdd(3, matrix2, result); //3 + matrix2
    a = MatrixEquals(matrixscalaradd2, result);
    if (a != 0) {
        b += 1;
    }
    if (b == 2) {
        c += 1;
    }
    printf("PASSED (%1.0f/2): MatrixScalarAdd()\n", (double) b);

    b = 0;
    MatrixScalarMultiply(2, matrix1, result); // matrix1 * 2
    a = MatrixEquals(matrixscalarmul1, result);
    if (a != 0) {
        b += 1;
    }
    MatrixScalarMultiply(3, matrix1, result); // matrix1 * 3
    a = MatrixEquals(matrixscalarmul2, result);
    if (a != 0) {
        b += 1;
    }
    if (b == 2) {
        c += 1;
    }
    printf("PASSED (%1.0f/2): MatrixScalarMultiply()\n", (double) b);

    b = 0;
    a = MatrixTrace(matrix1);
    printf("Result of MatrixTrace: %1.0f\n", (double) a);
    if (a == 6) {
        b += 1;
    }
    a = MatrixTrace(matrix2);
    printf("Result of MatrixTrace: %1.0f\n", (double) a);
    if (a == 6) {
        b += 1;
    }
    if (b == 2) {
        c += 1;
    }
    printf("PASSED (%1.0f/2): MatrixTrace()\n", (double) b);

    b = 0;
    MatrixTranspose(matrix1, result); // transpose of matrix1
    a = MatrixEquals(matrixtranspose1, result);
    if (a != 0) {
        b += 1;
    }
    MatrixTranspose(matrix2, result); // transpose of matrix2
    a = MatrixEquals(matrixtranspose2, result);
    if (a != 0) {
        b += 1;
    }
    if (b == 2) {
        c += 1;
    }
    printf("PASSED (%1.0f/2): MatrixTranspose()\n", (double) b);

    b = 0;
    printf("Demonstrating MatrixSubmatrix():\n");
    MatrixSubmatrix(1, 0, matrix1, submatrixResult); // submatrix(1,0) of matrix1
    a = SubmatrixEquals(matrixsubmatrix1, submatrixResult);
    if (a != 0) {
        b += 1;
    }
    printf("MatrixSubmatrix Test 1\n");
    SubmatrixPrint(submatrixResult);
    MatrixSubmatrix(1, 0, matrix2, submatrixResult); // submatrix(1,0) of matrix2
    a = SubmatrixEquals(matrixsubmatrix2, submatrixResult);
    if (a != 0) {
        b += 1;
    }
    printf("MatrixSubmatrix Test 2\n");
    SubmatrixPrint(submatrixResult);
    if (b == 2) {
        c += 1;
    }
    printf("PASSED (%1.0f/2): MatrixSubmatrix()\n", (double) b);

    b = 0;
    a = MatrixDeterminant(matrix5); // determinant of matrix5
    printf("Result of MatrixDeterminant Test 1: %4.4f\n", (double) a);
    if (a == 4) {
        b += 1;
    }
    a = MatrixDeterminant(matrix6); // determinant of matrix6
    printf("Result of MatrixDeterminant Test 2: %4.4f\n", (double) a);
    if (a - -10.061294 < FP_DELTA) {
        b += 1;
    }
    if (b == 2) {
        c += 1;
    }
    printf("PASSED (%1.0f/2): MatrixDeterminant()\n", (double) b);


    b = 0;
    printf("Demonstrating MatrixInverse():\n");
    MatrixInverse(matrix5, result); // inverse of matrix5
    a = MatrixEquals(matrixinverse3, result);
    if (a != 0) {
        b += 1;
    }
    printf("MatrixInverse Test 1\n");
    MatrixPrint(result);
    MatrixInverse(matrix6, result); // inverse of matrix6
    a = MatrixEquals(matrixinverse4, result);
    if (a != 0) {
        b += 1;
    }
    printf("MatrixInverse Test 2\n");
    MatrixPrint(result);
    if (b == 2) {
        c += 1;
    }
    printf("PASSED (%1.0f/2): MatrixInverse()\n", (double) b);

    d = (c / 10) * 100;
    printf("--------------------------------\n");
    printf("%1.0f out of 10 functions passed (%3.1f%%)\n\n", (double) c, (double) d);
    printf("Demonstrating MatrixPrint():\n");
    printf("Output of MatrixPrint():\n");
    MatrixPrint(matrix1); // matrix1 printed
    printf("Expected Output of MatrixPrint():\n"); // expected output of matrix1
    int i = 0, j = 0;
    while (i < DIM) {
        printf("\n -------------------------------- \n|");
        j = 0;
        while (j < DIM) {
            a = matrix1[i][j];
            if (a < 0) {
                printf("  %3.4f |", (double) a);
            } else {
                printf("  %3.4f  |", (double) a);
            }
            j++;
        }
        i++;
    }
    printf("\n -------------------------------- \n");
    while (1);
    return 0;
}

