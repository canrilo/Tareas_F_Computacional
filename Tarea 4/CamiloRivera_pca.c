#include <stdio.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_cblas.h>

void hallar_covarianza(gsl_matrix *A, int fil, int col, gsl_matrix *COV);
int count_lines(char *filename);

int main(int argc, char **argv){
	int i,j;
	double c;
	FILE *in;
	int numelec=24,filas;
	
	
	/*G es la "matriz" para guardar los datos del archivo*/
	float *G;
	
	/*D es la matriz en gsl que contiene los datos de los electrodos
	 *C es la matriz de Covarianza*/
	gsl_matrix *D, *C;
	
	
	
	/*Comprobación de argumentos*/
	if(argc!=2){
	printf("Este programa necesita ejecutarse con exactamente una entrada adicionale!\n EXIT!\n");
	exit(1);
	}
	
	filas=count_lines(argv[1]);
	/*Alocación de memoria*/
	if(!(G = malloc(sizeof(float)*filas*numelec))){
		fprintf(stderr, "Problem with memory allocation");
		exit(1);
	}
	D = gsl_matrix_alloc (filas, numelec);
	C = gsl_matrix_alloc (filas, filas);
	gsl_vector *eval = gsl_vector_alloc (filas);
	gsl_matrix *evec = gsl_matrix_alloc (filas,filas);
	
	/*Inicialización de variables*/
	for(i=0;i<filas;i++){
		for(j=0;j<numelec;j++){
			G[i*numelec+j]=0;
		}
	}
		
	/*Lectura de archivo*/
	in = fopen(argv[1],"r"); 
	if(!in){
		printf("problem opening the file %s\n", argv[1]);
		exit(1);
	}  
	for(i=0;i<filas;i++){
		fscanf(in, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n", 
		&G[i*numelec], &G[i*numelec+1], &G[i*numelec+2], &G[i*numelec+3], &G[i*numelec+4], 
		&G[i*numelec+5], &G[i*numelec+6], &G[i*numelec+7], &G[i*numelec+8], &G[i*numelec+9], 
		&G[i*numelec+10], &G[i*numelec+11], &G[i*numelec+12], &G[i*numelec+13], &G[i*numelec+14], 
		&G[i*numelec+15], &G[i*numelec+16], &G[i*numelec+17], &G[i*numelec+18], &G[i*numelec+19], 
		&G[i*numelec+20], &G[i*numelec+21], &G[i*numelec+22], &G[i*numelec+23]); 
	}
	fclose(in);
	
	/*Paso de datos de matriz a una matriz gsl*/
	for(i=0;i<filas;i++){
		for(j=0;j<numelec;j++){
			gsl_matrix_set (D, i, j, G[i*numelec+j]);
		}
	}
	
	/*Hallar Matriz de Covarianzas*/
	hallar_covarianza(D,filas,numelec,C);
	
	/*Hallar vectores y valores propios y posteriormente organizarlos*/
	gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc (filas);
	gsl_eigen_symmv (C, eval, evec, w);
	gsl_eigen_symmv_free (w);
	gsl_eigen_symmv_sort (eval, evec,GSL_EIGEN_SORT_ABS_DESC);
}


/*Conteo de líneas para asignación en memoria*/
int count_lines(char *filename){
		
  FILE *in;
  int n_lines;
  int c;
  if(!(in=fopen(filename, "r"))){
    printf("problem opening file %s\n", filename);
    exit(1);
  }
  n_lines = 0;
  do{
    c = fgetc(in);
    if(c=='\n'){
      n_lines++;
    }
  }while(c!=EOF);
  
  fclose(in);
  return n_lines;
}

/*Cálculo de la matriz de covarianza*/
void hallar_covarianza(gsl_matrix *A, int fil, int col, gsl_matrix *COV){
	int i,j,k;
	double *B1,*B2,c;
	gsl_vector *V1= gsl_vector_alloc (col);
	gsl_vector *V2= gsl_vector_alloc (col);
	B1=malloc(sizeof(double)*col);
	B2=malloc(sizeof(double)*col);
	for(i=0;i<fil;i++){
		for(j=0;j<fil;j++){
			gsl_matrix_get_row (V1, A, i);
			gsl_matrix_get_row (V2, A, j);
			for(k=0;k<col;k++){
				B1[k]=gsl_vector_get(V1, k);
				B2[k]=gsl_vector_get(V2, k);
			}
			c=gsl_stats_covariance (B1, sizeof(double), B2, sizeof(double), col);
			gsl_matrix_set (COV, i, j, c);
		}
	}
}
