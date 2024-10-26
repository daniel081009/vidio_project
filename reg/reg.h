#ifndef REG_H
#define REG_H

#include <stdio.h>
#include <stdlib.h>
#include "../db/db.h"

typedef struct Position_pre {
    char horror;    // 0~10
    char comedy;    // 0~10
    char action;    // 0~10
    char sf;        // 0~10
    char fantasy;   // 0~10
    char romance;   // 0~10
    char family;    // 0~10
    double rating;  // 평점 0~10 (종속 변수)
} Position_pre;

Position_pre pos_to_pre(Position pos,int good);
void multiple_linear_regression(Position_pre *data, int n, double *coefficients);
double predict_rating(const Position_pre *position, const double *coefficients);

void matrix_t(double **X, double **Xt, int n, int m);
void matrix_mult(double **A, double **B, double **C, int n, int m, int p);
void matrix_vector_multiply(double **A, double *B, double *C, int n, int m);
int matrix_inverse(double **A, double **inverse, int n);

#endif // REG_H
