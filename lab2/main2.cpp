#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define MAXCHAR 100

struct Param{
    int **A;
    int **B;
    int **C;
    int r, x, c;
};

void * mat_mul(void * arg){
    Param p = *((Param*)arg);
    int sum = 0;
    for(int k=0; k<p.x; k++){
        sum += p.A[p.r][k]*p.B[k][p.c];
    }
    p.C[p.r][p.c] = sum;
    pthread_exit(NULL);
}
void readMat(int*** A, int*** B, int* r, int* x, int* c){
    char fa[] = "A.txt";
    char fb[] = "B.txt";
    char line[MAXCHAR];
    FILE* fpa = fopen(fa, "r");
    FILE* fpb = fopen(fb, "r");

    fgets(line, MAXCHAR, fpa);
    sscanf(line, "%d %d", r, x);
    
    fgets(line, MAXCHAR, fpb);
    sscanf(line, "%d %d", x, c);

    *A = (int**)malloc(sizeof(int*) * (*r));
    for(int i=0; i<*r; i++){
        (*A)[i] = (int*)malloc(sizeof (int)*(*x));
        for(int j=0; j<*x; j++){
            fscanf(fpa,"%d", &(*A)[i][j]);
        }
    }

    *B = (int**)malloc(sizeof(int*) * (*x));
    for(int i=0; i<*x; i++){
        (*B)[i] = (int*)malloc(sizeof (int)*(*c));
        for(int j=0; j<*c; j++){
            fscanf(fpb,"%d", &(*B)[i][j]);
        }
    }    

    fclose(fpa);
    fclose(fpb);
}
int main(int argc, char const *argv[])
{
    int** A;
    int** B;
    int** C;
    pthread_t pid[MAXCHAR][MAXCHAR];

    int r,x,c;

    readMat(&A, &B, &r, &x, &c);
    C = (int**)malloc(sizeof(int*)*r);
    for(int i=0; i<r; i++){
        C[i] = (int*)malloc(sizeof(int)*c);
    }
    Param p[r*c];
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){

            p[i*c+j].A = A;
            p[i*c+j].B = B;
            p[i*c+j].C = C;
            p[i*c+j].r = i;
            p[i*c+j].x = x;
            p[i*c+j].c = j;

            //C[i][j] = \sum_{k=0}^{x-1} A[i][k] x B[k][j]
            pthread_create(&pid[i][j], NULL, mat_mul, &p[i*c+j]);
        }
    }

    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            pthread_join(pid[i][j], NULL);
        }
    }

    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    return 0;
}
