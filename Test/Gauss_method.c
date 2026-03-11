#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define epsilon 1e-12

int check_diag_view(double ** mat, int n){
    int flag = 0;
    for (int i = 0; i < n && flag == 0; i++){
        for (int j = 0; j < n && flag == 0; j++){
            if (i != j){
                if (fabs(mat[i][j]) > epsilon){
                    flag = 1;
                }
            }
        }
    }
    if (flag == 1){
        return 0;
    }
    return 1;
}

void swap(double ** a, double ** b){
    double * tmp = *a;
    *a = *b;
    *b = tmp;
}


double gauss_method_det_calculate(double ** mat, int n){
    double det = 1.0;
    int sign = 1;
    while (check_diag_view(mat, n) == 0){
        for (int i = 0; i < n; i++){
            int tmp = i;
            double maxi = fabs(mat[i][i]);
            for (int j = i + 1; j < n; j++){
                if (fabs(mat[j][i]) > maxi){
                    maxi = fabs(mat[j][i]);
                    tmp = j;
                }
            }
            if (maxi <= epsilon){
                return 0.0;
            }
            if (tmp != i){
                swap(&mat[i], &mat[tmp]);
                sign *= (-1);
            }
            for (int j = 0; j <n; j++){
                if (i != j){
                    double multiplicator = (mat[j][i] / mat[i][i]);
                    for (int k = 0; k < n ; k++){
                        mat[j][k] -= multiplicator * mat[i][k];
                    }
                }
            }
        }
    }
    for (int i = 0; i< n; i++){
        det *= mat[i][i];
    }
    return det * sign;
}


int main(void){
    int n = 0;
    scanf("%d", &n);
    double **mat = (double **)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++){
        mat[i] = (double *)malloc(n * sizeof (double));
        for (int j = 0; j < n; j++){
            scanf("%lf", &mat[i][j]);
        }
    }
    printf("%lf", gauss_method_det_calculate(mat, n));

    for (int i = 0; i < n; i++){
        free(mat[i]);
    }
    free(mat);
}