#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int rows;
    int cols;
    double**data;
}Matrix;

void create_mat(Matrix*pm, int rows, int cols){
    (*pm).rows=rows;
    (*pm).cols=cols;
    (*pm).data=(double**)malloc(rows*sizeof(double));
    for(int i=0; i<rows; i++){
        (*pm).data[i]=(double*)malloc(cols*sizeof(double));
    }
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            scanf("%lf",&(*pm).data[i][j]);
        }
    }
}

void create_identity_mat(Matrix*pm,int size){
    (*pm).rows=size;
    (*pm).cols=size;
    (*pm).data=(double**)malloc(size*sizeof(double*));
    for (int i=0; i<size; i++)
        (*pm).data[i]=(double*)malloc(size*sizeof(double));
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            (*pm).data[i][j]=0;
        }
        (*pm).data[i][i]=1;
    }
}

double* create_vec(double *v,int size){
    v=(double*)malloc(size*sizeof(double));
    for(int i=0; i<size; i++){
        scanf("%lf",v+i);
    }
    return v;
}

double dot_product_vec(const double*v1, const double*v2, int size){
    double res=0;
    for(int i=0; i<size; i++){
        res=res+v1[i]*v2[i];
    }
    return res;
}

double* mul_vec(double *result,const Matrix*pm,const double*vect){
    int rows=(*pm).rows;
    int cols=(*pm).cols;
    result=(double*)malloc(rows*sizeof(double));
    for(int i=0; i<rows; i++){
        result[i]=dot_product_vec(vect,(*pm).data[i],cols);
    }
    return result;
}

double*max_element_in_rows(double*max_data,const Matrix*pm){
    double maks=0;
    max_data=(double*)malloc((*pm).rows*sizeof(double));
    for(int i=0; i<(*pm).rows; i++){
        maks=0;
        for(int j=0; j<(*pm).cols; j++){
            if((*pm).data[i][j]>maks){
                maks=(*pm).data[i][j];
            }
        }
        max_data[i]=maks;
    }
    return max_data;
}

void dump_vec(const double *v1, int size){
    for(int i=0; i<size; i++){
        printf("%lf ",*(v1+i));
    }
    printf("\n");
}

void dump_mat(Matrix*pm){
    int rows=(*pm).rows;
    int cols=(*pm).cols;
    double**data=(double**)(*pm).data;
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            printf("%lf ",data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int swap_rows(Matrix*m,int r1, int r2){
    int c=0;
    if(r1<0 || r2<0 || r1>(*m).rows || r2>(*m).rows)
        return 0;
    for(int i=0; i<(*m).cols; i++){
        c=(*m).data[r1][i];
        (*m).data[r1][i]=(*m).data[r2][i];
        (*m).data[r2][i]=c;
    }
    return 1;
}

int main()
{
    Matrix matrix;
    int size,to_do,rows,cols;
    double *v1,*v2,*res,*max_data;
    scanf("%d",&to_do);
    switch(to_do){
        case 1:
            scanf("%d",&size);
            create_identity_mat(&matrix,size);
            dump_mat(&matrix);
        break;
        case 2:
            scanf("%d",&size);
            v1=create_vec(v1,size);
            v2=create_vec(v2,size);
            printf("%lf\n",dot_product_vec(v1,v2,size));
        break;
        case 3:
            scanf("%d %d",&rows,&cols);
            create_mat(&matrix,rows,cols);
            v1=create_vec(v1,cols);
            res=mul_vec(res,&matrix,v1);
            dump_vec(res,rows);
        break;
        case 4:
            scanf("%d %d",&rows,&cols);
            create_mat(&matrix,rows,cols);
            max_data=max_element_in_rows(max_data,&matrix);
            dump_vec(max_data,rows);
        break;
        default:
            printf("Missclick\n");
    }
    printf("All done!\n");
    return 0;
}
