#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    double sum=0;
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<p; k++){
                sum=sum+A[i][k]*B[k][j];
            }
            AB[i][j]=sum;
            sum=0;
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    double denm=0,det=1;
    for(int i=0; i<n; i++){
        if(A[i][i]==0) return NAN;
        for(int j=i+1; j<n; j++){
            denm=A[j][i]/A[i][i];
            for(int k=i; k<n; k++){
                A[j][k]=A[j][k]-denm*A[i][k];
            }
        }
    }
    for(int i=0; i<n; i++){
        det*=A[i][i];
    }
    return det;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
    double denm=0,det=1,pom=0,sign=1;
    for(int i=0; i<n; i++) x[i]=b[i];

    for(int i=0; i<n; i++){
        if(A[i][i]==0){
            for(int j=i; j<n; j++){
                pom=A[i][j];
                A[i][j]=A[i+1][j];
                A[i+1][j]=pom;
            }
            pom=x[i];
            x[i]=x[i+1];
            x[i+1]=pom;
            sign=sign*(-1);
            i+=1;
        }
        for(int j=i+1; j<n; j++){
            denm=A[j][i]/A[i][i];
            x[j]=x[j]-denm*x[i];
            for(int k=i; k<n; k++){
                A[j][k]=A[j][k]-denm*A[i][k];
            }
        }
    }
    for(int i=n-1; i>=0; i--){
        for(int j=i-1; j>=0; j--){
            denm=A[j][i]/A[i][i];
            x[j]=x[j]-denm*x[i];
            for(int k=i; k>=0; k--){
                A[j][k]=A[j][k]-denm*A[i][k];
            }
        }
    }
    for(int i=0; i<n; i++){
        det=det*A[i][i];
        x[i]=x[i]/A[i][i];
    }
    det=det*sign;
    return det;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    double denm=0,det=1,pom=0,sign=1,denm2;
    int idx=0;
    //B-macierz jednostkowa
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            B[i][j]=0;
            if(i==j) B[i][j]=1;
        }
    }
    //A-macierz trojkotna gorna
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            if(A[i][i]==0){
                for(int k=i; k<n; k++){
                    if(A[k][i]!=0){
                        idx=k;
                    }
                }
                for(int j=i; j<n; j++){
                    pom=A[i][j];
                    A[i][j]=A[i+idx][j];
                    A[i+idx][j]=pom;
                    //B
                    pom=B[i][j];
                    B[i][j]=B[i+idx][j];
                    B[i+idx][j]=pom;
                }
                sign=sign*(-1);
                continue;
            }
            denm=A[j][i]/A[i][i];
            for(int k=0; k<n; k++){
                A[j][k]=A[j][k]-denm*A[i][k];
                B[j][k]=B[j][k]-denm*B[i][k];
            }
        }
    }
    //A-macierz diagonalna
    for(int i=n-1; i>=0; i--){
        for(int j=i-1; j>=0; j--){
            denm=A[j][i]/A[i][i];
            for(int k=n-1; k>=0; k--){
                A[j][k]=A[j][k]-denm*A[i][k];
                B[j][k]=B[j][k]-denm*B[i][k];
            }
        }
    }
    for(int i=0; i<n; i++){
        if(A[i][i]<eps && A[i][i]>-eps) return 0;
        det=det*A[i][i];
    }
    det=det*sign;
    if(det==0){
        return 0;
    }
    //A-macierz jednostkowa
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            B[i][j]/=A[i][i];
        }
    }
    return det;
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det!=0) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

