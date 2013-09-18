#include <stdio.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_statistics.h>

void hallar_covarianza(gsl_matrix *A, int fil, int col, gsl_matrix *COV);
int count_lines(char *filename);
void print_matrixg(gsl_matrix *m, int n_x, int n_y);
void escribir_autovec(gsl_matrix *A,int num, int filas,char *nombre);
int hallar_chi(gsl_vector *A,gsl_vector *B,int dim);
double prod_punto(gsl_vector *un, gsl_vector *dos,int dim);

int main(int argc, char **argv){
	int i,j;
	double c,chicua;
	FILE *in,*eiv,*ch;
	int numelec=24,filas;
	char valores[100]="CamiloRivera_eigenvalues.dat";
	char vectores[100]="CamiloRivera_eigenvectors.dat";
	char *chis;
	chis=malloc(sizeof(char)*100);
	chis="CamiloRivera_pca_chisquare.dat";	
	
	/*G es la "matriz" para guardar los datos del archivo*/
	float *G;
	
	/*D es la matriz en gsl que contiene los datos de los electrodos
	 *C es la matriz de Covarianza
	 *B es la matriz que contendrá los 10 primeros autovectores*
	 *A es la matriz que contiene las aproximaciones usando los 10 primeros autovectores 
	 *F es la matriz de coeficientes en la nueva base
	 *l1 y l2 son vectores auxiliares
	 *CHS es el vector de los Chi²*/ 
	gsl_matrix *D, *C,*B,*A, *F;
	gsl_vector*l1,*l2,*CHS;
	
	
	
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
	F = gsl_matrix_alloc (10,numelec);
	l1= gsl_vector_alloc (filas);
	l2= gsl_vector_alloc (filas);
	CHS= gsl_vector_alloc (numelec);
	D = gsl_matrix_alloc (filas, numelec);
	B = gsl_matrix_alloc (filas, 10);
	C = gsl_matrix_alloc (filas, filas);
	A = gsl_matrix_alloc (filas, numelec);
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
	
	//print_matrixg(D,filas,numelec);
	/*Hallar Matriz de Covarianzas*/
	hallar_covarianza(D,filas,numelec,C);
	
	/*Hallar vectores y valores propios y posteriormente organizarlos*/
	gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc (filas);
	gsl_eigen_symmv (C, eval, evec, w);
	gsl_eigen_symmv_free (w);
	gsl_eigen_symmv_sort (eval, evec,GSL_EIGEN_SORT_ABS_DESC);
	
	/*Escritura de archivo con Eigenvalues*/
	eiv = fopen(valores,"w"); 
	if(!eiv){
		printf("problem opening the file %s\n", valores);
		exit(1);
	}
	gsl_vector_fprintf (eiv,eval,"%f");
	fclose(eiv);
	
	/*Escritura de primeros autovectores*/
	escribir_autovec(evec,10,filas,vectores);
	
	/*----Hallar Chi² usando los los 10 primeros autovectores----*/
	/*Empezar por los 10 primeros autovectores*/
	for(i=0;i<filas;i++){
		for(j=0;j<10;j++){
			gsl_matrix_set(B,i,j,gsl_matrix_get(evec, i, j));
		}
	}
	/*Crear matriz de coeficientes*/
	for(i=0;i<numelec;i++){
		gsl_matrix_get_col(l2, D, i);
		for(j=0;j<10;j++){
			gsl_matrix_get_col (l1, B, j);
			gsl_matrix_set(F,j,i,prod_punto(l2,l1,filas));
		}
	}
	/*Crear matriz de vectores reconstruidos y aproximados*/
	for(i=0;i<numelec;i++){
		gsl_matrix_get_col(l1,B,0);
		gsl_vector_scale(l1,gsl_matrix_get(F,0,i));
		for(j=1;j<10;j++){
			gsl_matrix_get_col(l2,B,j);
			gsl_vector_scale(l2,gsl_matrix_get(F,j,i));
			gsl_vector_add(l1,l2);
		}
		gsl_matrix_set_col (A, i, l1);
	}
	
	/*Hallar Chi²*/
	for(i=0;i<numelec;i++){
		gsl_matrix_get_col(l1,A,i);
		gsl_matrix_get_col(l2,D,i);
		chicua=hallar_chi(l1,l2,filas);
		gsl_vector_set (CHS, i, chicua);
	}
	
	/*Crear Archivo con Chi²*/
	ch = fopen(chis,"w"); 
	if(!ch){
		printf("problem opening the file %s\n", chis);
		exit(1);
	}
	gsl_vector_fprintf (ch,CHS,"%f");
	fclose(ch);
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
	double *B1,*B2;
	double c;
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
			c=gsl_stats_covariance(B1, 1, B2, 1, col);
			//printf("%f\n",c);
			gsl_matrix_set (COV, i, j, c);
		}
	}
	free(B1);
	free(B2);
	gsl_vector_free(V1);
	gsl_vector_free(V2);
}

/*Método auxiliar para imprimir matrices GSL*/
void print_matrixg(gsl_matrix *m, int n_x, int n_y){
		
	int i,j;
	fprintf(stdout, "\n");
	/*Prints to screen*/
	for(i=0;i<n_x;i++){
		for(j=0;j<n_y;j++){
			fprintf(stdout, " %f ",gsl_matrix_get(m, i, j));
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");

}

/*Programa que escribe los primeros autovectores indicados*/
void escribir_autovec(gsl_matrix *A,int num, int filas, char *nombre){
	
	FILE *eive;
	double *B;
	int i,j;
	
	B=malloc(sizeof(double)*num*filas);
	
	for(i=0;i<filas;i++){
		for(j=0;j<num;j++){
			B[i*num+j]=gsl_matrix_get(A, i, j);
		}
	}
	
	eive = fopen(nombre,"w"); 
	if(!eive){
		printf("problem opening the file %s\n", nombre);
		exit(1);
	}
	for(i=0;i<filas;i++){
		fprintf(eive, "%f %f %f %f %f %f %f %f %f %f\n",B[i*num],B[i*num+1],B[i*num+2],B[i*num+3],
							B[i*num+4],B[i*num+5],B[i*num+6],B[i*num+7],B[i*num+8],B[i*num+9]); 
	}
	fclose(eive);
	free(B);
	
}

/*Función que halla el chi² reducido del ajuste. Recibe los vectores reales y aproximados,
 *más la dimensión de los vectores*/
int hallar_chi(gsl_vector *A,gsl_vector *B,int dim){
	int suma=0,i;
	for(i=0;i<dim;i++){
		suma+=pow(gsl_vector_get(A, i)-gsl_vector_get(B, i),2);
	}
	
	return suma/dim;
}


/*Función que calcula el producto punto entre dos vectores gsl*/
double prod_punto(gsl_vector *un, gsl_vector *dos,int dim){
	int i;
	double suma=0;
	for(i=0;i<dim;i++){
		suma+=gsl_vector_get(un,i)*gsl_vector_get(dos,i);
	}
	return suma;
}
