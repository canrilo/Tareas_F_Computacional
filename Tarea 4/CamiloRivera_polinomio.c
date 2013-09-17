#include <stdio.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_cblas.h>

void print_matrix(float * m, int n_x, int n_y);
int count_lines(char *filename);
void print_matrixd(double * m, int n_x, int n_y);
void print_matrixg(gsl_matrix *m, int n_x, int n_y);
gsl_matrix* matr_mult(gsl_matrix *A,  gsl_matrix *B, int d1, int d2, int di);
void inverse_matrix(gsl_matrix *orig,  gsl_matrix *dest, int dim);

int main(int argc, char **argv){
	/*deg es el grado del polinomio y num el número de entradas*/
	int i,j,deg,num;
	
	/*F es la matriz que contiene los datos en el archivo de entrada*/
	float *F;
	
	/*G es la matriz G de las notas, pero como un vector de doubles*/
	double *G,chi;
	
	/*H es la matriz G, HT la matriz G transpuesta, T es la multiplicación de las dos anteriores
	D es el vector de respuesta, UNO es la inversa de la multiplicación GG', 
	DOS es la multiplicación de la transpuesta por el vector de respuestas, M es la aproximación de coeficientes,
	T1 es una matriz auxiliar idéntica a T que es necesaria, ya que el método de inversion modifica la matriz de entrada
	Ff es el vector de salida aproximado*/
	gsl_matrix *H,*HT,*T,*D,*UNO,*DOS,*M,*I,*T1,*Ff;
	FILE *in;
	
	/*Comprobación de argumentos*/
	if(argc!=3){
	printf("Este programa necesita ejecutarse con exactamente dos entradas adicionales!\n EXIT!\n");
	exit(1);
	}
	
	deg = atoi(argv[2]);
	
	if(deg<=0){
		printf("Error!\n El grado del polinomio debe ser mayor a 0\n");
	exit(1);
	}
	
	/*Alocación de memoria*/
	num=count_lines(argv[1]);
	if(!(F = malloc(sizeof(float)*num*2))){
		fprintf(stderr, "Problem with memory allocation");
		exit(1);		
	}
	if(!(G = malloc(sizeof(double)*num*deg))){
		fprintf(stderr, "Problem with memory allocation");
		exit(1);		
	}
	H = gsl_matrix_alloc (num, deg);
	HT = gsl_matrix_alloc (deg, num);
	D = gsl_matrix_alloc (num, 1);
	Ff = gsl_matrix_alloc (num, 1);
	T = gsl_matrix_alloc (deg,deg);
	T1 = gsl_matrix_alloc (deg,deg);
	UNO=gsl_matrix_alloc (deg,deg);
	I=gsl_matrix_alloc (deg,deg);
	M=gsl_matrix_alloc (deg,1);
	
	
	/*Inicialización de variables*/
	for(i=0;i<num;i++){
		for(j=0;j<2;j++){
			F[i*2+j]=0;
		}
	}
	for(i=0;i<num;i++){
		for(j=0;j<deg;j++){
			G[i*deg+j]=0;
		}
	}
	
	/*Lectura de archivo*/
	in = fopen(argv[1],"r"); 
	if(!in){
		printf("problem opening the file %s\n", argv[1]);
		exit(1);
	}  
	for(i=0;i<num;i++){
		fscanf(in, "%f %f \n", &F[2*i], &F[2*i+1]); 
	}
	fclose(in);
	
	//print_matrix(F, num,2);
	
	/*Creación Matriz de valores y matriz GSL de salidas fi*/
	for(i=0;i<num;i++){
		gsl_matrix_set (D, i, 0, (double)F[i*2+1]);
		for(j=0;j<deg;j++){
			G[i*deg+j]=(double)pow(F[i*2],j);
		}
	}
	//print_matrixg(D,num,1);
	
	for(i=0;i<num;i++){
		for(j=0;j<deg;j++){
			gsl_matrix_set (H, i, j, G[i*deg+j]);
		}
	}
	
	/*Liberación de memoria asignada incialmente a G y a F*/
	free(G);
	free(F);
	
	
	gsl_matrix_transpose_memcpy(HT,H);
	
	T=matr_mult(HT,H,deg,deg,num);
	
	/*Copiar todos los datos de T en T1 para no perder la información en la inversión*/
	for(i=0;i<deg;i++){
		for(j=0;j<deg;j++){
			gsl_matrix_set (T1, i, j, gsl_matrix_get(T, i, j));
		}
	}
	
	//print_matrixg(T,deg,deg);
	
	DOS=matr_mult(HT,D,deg,1,num);
	
	/*Para deg>3 la inversa comienza a fallar*/
	inverse_matrix(T1,UNO,deg);
	
	//print_matrixg(DOS,deg,1);
	
	I=matr_mult(T,UNO,deg,deg,deg);
	
	//print_matrixg(I,deg,deg);
	
	M=matr_mult(UNO,DOS,deg,1,deg);
	
	//print_matrixg(M,deg,1);
	
	Ff=matr_mult(H,M,num,1,deg);
	
	chi=hallar_chi(Ff,D,num);
	
	for(i=0;i<deg;i++){
		printf("a_%d %f\n",i,gsl_matrix_get(M, i, 0));
	}
		
	printf("%f\n",chi);
	
	return 0;
}

/*Método Para realizar la multiplicación de matrices. di es la dimensión en común de las matrices,
 d1 es la dimensión diferente de la primera matriz y d2 de la segunda*/
gsl_matrix* matr_mult(gsl_matrix *A,  gsl_matrix *B, int d1, int d2, int di){
	double conteo=0;
	int i,j,r;
	gsl_matrix *D= gsl_matrix_alloc (d1, d2);
	for(i=0;i<d1;i++){
		for(j=0;j<d2;j++){
			conteo=0;
			for(r=0;r<di;r++){
				conteo+= gsl_matrix_get(A, i, r)*gsl_matrix_get(B, r, j);
			}
			gsl_matrix_set (D, i, j, conteo);
		}
	}
	
	return D;
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

/*Método auxiliar para imprimir matrices con datos tipo float*/
void print_matrix(float * m, int n_x, int n_y){
	int i,j,pos;
	fprintf(stdout, "\n");
	/*Prints to screen*/
	for(i=0;i<n_x;i++){
		for(j=0;j<n_y;j++){
			pos = j + (n_y * i);/*position in the array*/
			fprintf(stdout, " %.2f ",m[pos]);
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
}

/*Método auxiliar para imprimir matrices con datos tipo double*/
void print_matrixd(double * m, int n_x, int n_y){
	
	int i,j,pos;
	fprintf(stdout, "\n");
	/*Prints to screen*/
	for(i=0;i<n_x;i++){
		for(j=0;j<n_y;j++){
			pos = j + (n_y * i);/*position in the array*/
			fprintf(stdout, " %f ",m[pos]);
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
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

/*Método para calcular la inversa de una matriz orig en dest*/
/*OJO! AL FINAL DEL MÉTODO ORIG QUEDA CAMBIADA*/
void inverse_matrix(gsl_matrix *orig,  gsl_matrix *dest,int dim){
	gsl_permutation *p;
	int f;
	
	p = gsl_permutation_alloc (dim);
	
	gsl_linalg_LU_decomp (orig, p, &f);
	gsl_linalg_LU_invert (orig, p, dest);
	
	
}

/*Función que halla el chi² reducido del ajuste. Recibe los vectores reales y aproximados,
 *más la dimensión de los vectores*/
int hallar_chi(gsl_matrix *A,gsl_matrix *B,int dim){
	int suma=0,i;
	for(i=0;i<dim;i++){
		suma+=pow(gsl_matrix_get(A, i, 0)-gsl_matrix_get(B, i, 0),2);
	}
	
	return suma/dim;
}
