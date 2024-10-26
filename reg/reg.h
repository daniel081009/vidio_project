#ifndef REG_H
#define REG_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "user_ui.h"
#include "../db/db.h"

typedef struct {
    int action;    // 0~10
    int comedy;    // 0~10
    int family;    // 0~10
    int fantasy;   // 0~10
    int horror;    // 0~10
    int romance;   // 0~10
    int sf;        // 0~10
    int rating;    // 평점 0~10 (종속 변수)
} Position_pre;

// 함수 선언
Position_pre pos_to_pre(Position pos, int good);
void multiple_linear_regression(Position_pre *data, int n, double *coefficients);
double predict_rating(const Position *position, const double *coefficients);

void matrix_t(double **X, double **Xt, int n, int m);
void matrix_mult(double **A, double **B, double **C, int n, int m, int p);
void matrix_vector_multiply(double **A, double *B, double *C, int n, int m);
int matrix_inverse(double **A, double **inverse, int n);

#endif // REG_H
