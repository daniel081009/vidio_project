#include "reg.h"

// 구조체 정의
typedef struct {
    Vidio *u;
    double good; // 타입 변경
} Vidio_good;

// 비교 함수 수정
int compare_results_good(const void *a, const void *b) {
    Vidio_good *vg1 = (Vidio_good *)a;
    Vidio_good *vg2 = (Vidio_good *)b;
    if (vg2->good > vg1->good) return 1;
    else if (vg2->good < vg1->good) return -1;
    else return 0;
}

// pos_to_pre 함수
Position_pre pos_to_pre(Position pos, int good) {
    Position_pre a;
    a.action = pos.action;
    a.comedy = pos.comedy;
    a.family = pos.family;
    a.fantasy = pos.fantasy;
    a.horror = pos.horror;
    a.romance = pos.romance;
    a.sf = pos.sf;
    a.rating = good;
    return a;
}

// expand_to_square 함수
void expand_to_square(double **X, int n, int m) {
    int square_size = (n > m) ? n : m;
    for (int i = n; i < square_size; i++) {
        for (int j = 0; j < m; j++) {
            X[i][j] = X[i % n][j];
        }
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "reg.h"
#include "user_ui.h"

// 기존 구조체 및 함수 선언은 그대로 유지합니다.

// multiple_linear_regression 함수 수정
void multiple_linear_regression(Position_pre *data, int n, double *coefficients) {
    int i, j;
    int m = 8; // 독립 변수 수 + 절편 항
    double **X, **Xt, **XtX, *Y, *XtY, **inv_XtX;

    // 메모리 동적 할당
    X = (double **)malloc(n * sizeof(double *));
    Xt = (double **)malloc(m * sizeof(double *));
    XtX = (double **)malloc(m * sizeof(double *));
    inv_XtX = (double **)malloc(m * sizeof(double *));
    Y = (double *)malloc(n * sizeof(double));
    XtY = (double *)malloc(m * sizeof(double));

    for (i = 0; i < n; i++) {
        X[i] = (double *)malloc(m * sizeof(double));
    }
    for (i = 0; i < m; i++) {
        Xt[i] = (double *)malloc(n * sizeof(double));
        XtX[i] = (double *)malloc(m * sizeof(double));
        inv_XtX[i] = (double *)malloc(m * sizeof(double));
    }

    // 독립 변수와 종속 변수 배열 초기화
    for (i = 0; i < n; i++) {
        X[i][0] = 1.0; // 절편 항 추가
        X[i][1] = data[i].horror;
        X[i][2] = data[i].comedy;
        X[i][3] = data[i].action;
        X[i][4] = data[i].sf;
        X[i][5] = data[i].fantasy;
        X[i][6] = data[i].romance;
        X[i][7] = data[i].family;
        Y[i] = data[i].rating;
    }

    // Xt 계산
    matrix_t(X, Xt, n, m);

    // Xt * X 계산
    matrix_mult(Xt, X, XtX, m, n, m);

    // Xt * Y 계산
    matrix_vector_multiply(Xt, Y, XtY, m, n);

    // (Xt * X)^-1 계산
    if (matrix_inverse(XtX, inv_XtX, m) == 0) {
        wprintf(L"역행렬을 구할 수 없습니다.\n");
        goto cleanup;
    }
    //(Xt * X)^-1 * Xt*Y
    matrix_vector_multiply(inv_XtX, XtY, coefficients, m, m);
cleanup:
    // 메모리 해제
    for (i = 0; i < n; i++) {
        free(X[i]);
    }
    for (i = 0; i < m; i++) {
        free(Xt[i]);
        free(XtX[i]);
        free(inv_XtX[i]);
    }
    free(X);
    free(Xt);
    free(XtX);
    free(inv_XtX);
    free(Y);
    free(XtY);
}

// predict_rating 함수 수정
double predict_rating(const Position *position, const double *coefficients) {
    double prediction = coefficients[0] +
                        coefficients[1] * position->horror +
                        coefficients[2] * position->comedy +
                        coefficients[3] * position->action +
                        coefficients[4] * position->sf +
                        coefficients[5] * position->fantasy +
                        coefficients[6] * position->romance +
                        coefficients[7] * position->family;

    // 디버깅 출력: 예측 값 확인
    printf("예측 값 계산: %f\n", prediction);
    return prediction;
}

// 행렬 전치 함수
void matrix_t(double **X, double **Xt, int n, int m) {
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            Xt[i][j] = X[j][i];
        }
    }
}

// 행렬곱 함수
void matrix_mult(double **A, double **B, double **C, int n, int m, int p) {
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            C[i][j] = 0;
            for (k = 0; k < m; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 행렬-벡터 곱 함수
void matrix_vector_multiply(double **A, double *B, double *C, int n, int m) {
    int i, j;
    for (i = 0; i < n; i++) {
        C[i] = 0;
        for (j = 0; j < m; j++) {
            C[i] += A[i][j] * B[j];
        }
    }
}

// 행렬 역행렬 함수
int matrix_inverse(double **A, double **inverse, int n) {
    int i, j, k;
    double temp;

    double **aug = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < n; i++) {
        aug[i] = (double *)malloc(2 * n * sizeof(double));
    }

    // 단위 행렬을 우변에 추가
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            aug[i][j] = A[i][j];
            aug[i][j + n] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (i = 0; i < n; i++) {
        // 피벗이 0이면 행 교환
        if (aug[i][i] == 0) {
            int swapped = 0;
            for (k = i + 1; k < n; k++) {
                if (aug[k][i] != 0) {
                    double *temp_row = aug[i];
                    aug[i] = aug[k];
                    aug[k] = temp_row;
                    swapped = 1;
                    break;
                }
            }
            if (!swapped) {
                for (i = 0; i < n; i++) {
                    free(aug[i]);
                }
                free(aug);
                return 0;
            }
        }

        // 피벗을 1로
        temp = aug[i][i];
        for (j = 0; j < 2 * n; j++) {
            aug[i][j] /= temp;
        }

        // 다른 행 열 값 제거
        for (k = 0; k < n; k++) {
            if (k != i) {
                temp = aug[k][i];
                for (j = 0; j < 2 * n; j++) {
                    aug[k][j] -= aug[i][j] * temp;
                }
            }
        }
    }

    // 역행렬 복사
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            inverse[i][j] = aug[i][j + n];
        }
    }

    for (i = 0; i < n; i++) {
        free(aug[i]);
    }
    free(aug);

    return 1;
}
