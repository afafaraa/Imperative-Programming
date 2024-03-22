#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int row, int col, int *A){
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            printf("%d ",*A);
            A++;
        }
    }
    printf("%d ",*A);
    return *A;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    int sum=0;
    for(int i=0; i<rowsA; i++){
        for(int j=0; j<colsB; j++){
            for(int k=0; k<colsA; k++){
                sum=sum+*A*get(j,i,B);
            }
            *AB=sum;
            AB+=1;
            sum=0;
        }
    }
    return AB;
}

void read_mat(int rows, int cols, int *t) {
    for(int i=0; i<cols; i++){
        for(int j=0; j<rows; j++){
            scanf("%d",t);
            t+=1;
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            printf("%d ",*t);
            t+=1;
        }
        printf("\n");
    }
}

int read_char_lines(char *array[]) {
}

void write_char_line(char *array[], int n) {
}

void delete_lines(char *array[]) {
}

int read_int_lines_cont(int *ptr_array[]) {
    char t[TAB_SIZE]={};
    int n=0,i=0;
    while(fgets(t,TAB_SIZE,stdin)!=NULL){
        n=strlen(t);
        for(int j=0; j<n; j++){
            *ptr_array[i]+j=t[j];
        }
        i++;
    }
}

void write_int_line_cont(int *ptr_array[], int n) {

}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
}

void write_int_line(line_type lines_array[], int n) {
}

void delete_int_lines(line_type array[], int line_count) {
}

int cmp (const void *a, const void *b) {
}

void sort_by_average(line_type lines_array[], int line_count) {
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
}

int cmp_triplets(const void *t1, const void *t2) {
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
}

void read_vector(int *v, int n) {
}

void write_vector(int *v, int n) {
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);

			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			print_mat(rowsA, colsA, A);
			print_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
		    read_mat(2,3,A);
		    read_mat(3,2,B);
		    prod_mat(2,3,2,A,B,AB);
		    print_mat(2,2,AB);
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

