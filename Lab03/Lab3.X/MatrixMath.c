/*
 * File:   MatrixMath.c
 * Author: Kyle Zhang | kmzhang
 *
 * Created on April 18, 2019, 12:00 PM
 */


#include "xc.h"
#include "BOARD.h"
#include "MatrixMath.h"
#include <stdio.h>
#include <stdlib.h>

void SubmatrixPrint(float mat[2][2]); // checks submatrixes
int SubmatrixEquals(float mat1[2][2], float mat2[2][2]); // function prints submatrixes

void MatrixPrint(float mat[3][3])
{
    int i = 0, j = 0;
    float a;
    while (i < DIM) {
        printf("\n -------------------------------- \n|");
        j = 0;
        while (j < DIM) {
            a = mat[i][j];
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
}

void SubmatrixPrint(float mat[2][2])
{
    int i = 0, j = 0;
    float a;
    while (i < 2) {
        printf("\n --------------------- \n|");
        j = 0;
        while (j < 2) {
            a = mat[i][j];
            if (a < 0) {
                printf("  %3.4f |", (double) a);
            } else {
                printf("  %3.4f  |", (double) a);
            }
            j++;
        }
        i++;
    }
    printf("\n --------------------- \n");
}

int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    float a = 0;
    int i = 0, j = 0;
    while (i < DIM) {
        j = 0;
        while (j < DIM) {
            a = mat1[i][j] - mat2[i][j];
            if (a < 0) {
                a *= -1;
            }
            if (a > FP_DELTA) {
                return 0;
            }
            j++;
        }
        i++;
    }
    return 1;
}

int SubmatrixEquals(float mat1[2][2], float mat2[2][2])
{
    int i, j;
    float a;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            a = mat1[i][j] - mat2[i][j];
            if (a > FP_DELTA) {
                return 0;
            }
        }
    }
    return 1;
}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int i = 0, j = 0;
    while (i < DIM) {
        j = 0;
        while (j < DIM) {
            result[i][j] = mat1[i][j] + mat2[i][j];
            j++;
        }
        i++;
    }
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    float a, b, c;
    int i = 0, j = 0, k = 0, l = 0, m = 0; //i=rows, j=columns, k = rows, l = columns;
    while (i < DIM) {
        j = 0;
        while (l < DIM) {
            while (j < DIM) {
                a = mat1[i][j];
                b = mat2[k][l];
                c += (a * b);
                j++;
                k++;
            }
            j = 0;
            k = 0;
            result[i][m] = c;
            c = 0;
            l++;
            m++;
        }
        l = 0;
        m = 0;
        i++;
    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3])
{
    int i = 0, j = 0;
    while (i < DIM) {
        j = 0;
        while (j < DIM) {
            result[i][j] = mat[i][j] + x;
            j++;
        }
        i++;
    }
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3])
{
    int i = 0, j = 0;
    while (i < DIM) {
        j = 0;
        while (j < DIM) {
            result[i][j] = mat[i][j] * x;
            j++;
        }
        i++;
    }
}

float MatrixTrace(float mat[3][3])
{
    float a, b = 0;
    int i = 0, j = 0;
    while (i < DIM) {
        j = 0;
        while (j < DIM) {
            if (i == j) {
                a = mat[i][j];
                b += a;
            }
            j++;
        }
        i++;
    }
    return b;
}

void MatrixTranspose(float mat[3][3], float result[3][3])
{
    int i = 0, j = 0;
    while (i < DIM) {
        j = 0;
        while (j < DIM) {
            result[j][i] = mat[i][j];
            j++;
        }
        i++;
    }
}

void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2])
{
    int a = 0, b = 0, c = 0, d = 0;
    for (a = 0; a < DIM; a++) {
        if (a == i) { //if a = i, then skip that row
            continue;
        } else if (c == 1) {
            for (b = 0; b < DIM; b++) {
                if (b == j) { //if b = j, skip that column
                    continue;
                } else if (d == 1) {
                    result[c][d] = mat[a][b];
                } else {
                    result[c][d] = mat[a][b];
                    d++;
                }
            }
        } else {
            for (b = 0; b < DIM; b++) {
                if (b == j) {
                    continue;
                } else if (d == 1) {
                    result[c][d] = mat[a][b];
                    d = 0;
                    c++;
                } else {
                    result[c][d] = mat[a][b];
                    d++;
                }
            }
        }
    }
}

float MatrixDeterminant(float mat[3][3])
{
    float det = 0, e = 0, f = 0, g = 0, submat[2][2];
    int i, j, c, d;
    for (j = 0; j < DIM; j++) {
        i = 0;
        MatrixSubmatrix(i, j, mat, submat); // calls MatrixSubmatrix
        c = 0;
        d = 0;
        e = submat[c][d] * submat[c + 1][d + 1];
        f = submat[c + 1][d] * submat[c][d + 1];
        if (j == 1) {
            g = -1 * (e - f);
        } else {
            g = e - f;
        }
        det += mat[i][j] * g;
    }
    return det;
}

void MatrixInverse(float mat[3][3], float result[3][3])
{
    float a, b, c = 0, f = 0, submat[2][2], untransposed[3][3];
    int i, j, d = 0, e = 0;
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            result[i][j] = 0;
            j++;
        }
        i++;
    }
    //finding 1/determinant
    c = MatrixDeterminant(mat); // calls MatrixDeterminant
    if (c != 0) {
        c = (1 / c);
        for (i = 0; i < DIM; i++) {
            for (j = 0; j < DIM; j++) {
                MatrixSubmatrix(i, j, mat, submat); // calls MatrixSubmatrix for 
                d = 0;
                e = 0;
                a = submat[e][d] * submat[e + 1][d + 1];
                b = submat[e + 1][d] * submat[e][d + 1];
                f = a - b;
                f *= c;
                if (((i + j) % 2) == 0) {
                    untransposed[i][j] = f;
                } else {
                    untransposed[i][j] = (-1 * f);
                }
            }
        }
        MatrixTranspose(untransposed, result);
    }
}