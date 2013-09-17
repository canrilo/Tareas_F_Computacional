#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int count_lines(char *filename);
int encontrar_menor(float *x, float *y, float *z, int max);
float proc(float pxi,float pyi, float pzi, float pxj,float pyj, float pzj);

int main(int argc, char **argv){
	FILE *in;
	int num=0,i,j;
	float *posx;
	float *posy;
	float *posz;
	float px=0,py=0,pz=0;
	
	/*Comprobación de argumentos*/
	
	if(argc!=2){
	printf("Este programa necesita ejecutarse con exactamente una entrada adicional!\n EXIT!\n");
	exit(1);
	}
	
	/*Alocación de memoria*/
	num=count_lines(argv[1]);
	posx=malloc(sizeof(float)*num);
	posy=malloc(sizeof(float)*num);
	posz=malloc(sizeof(float)*num);
	
	/*Búsqueda de errores*/
	if(!posx||!posy||!posz){
    printf("There is something wrong with array int\n");
    exit(1);
	}
	
	/*Inicialización de variables*/
	for(i=0;i<num;i++){
		posx[i]=0;
		posy[i]=0;
		posz[i]=0;
	}
	
	/*Lectura de archivo*/
	in = fopen(argv[1],"r"); 
	if(!in){
		printf("problem opening the file %s\n", argv[1]);
		exit(1);
	}  
	for(i=0;i<num;i++){
		fscanf(in, "%f %f %f\n", &posx[i], &posy[i],&posz[i]); 
	}
	fclose(in);
	
	j=encontrar_menor(posx,posy,posz, num);
	
	printf("%f %f %f\n",posx[j], posy[j], posz[j]);
	return 0;
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

/*Encuentra el menor potenciar recibiendo los arrays de puntos y su longitud*/
int encontrar_menor(float *x, float *y, float *z, int max){
	int i,j,ind,com=0;
	
	float menor=0, actual=0;
	for(i=1;i<max;i++){
		menor+=proc(x[0],y[0],z[0],x[i],y[i],z[i]);
	}
	ind=0;
	
	for(i=1;i<max;i++){
		com=0;
		actual=0;
		for(j=0;j<max;j++){
			if(i!=j){
				actual+=proc(x[i],y[i],z[i],x[j],y[j],z[j]);
			}
		}
		if(actual>menor){
			menor = actual;
			ind=i;
		}
	}
	
	return ind;
}

/*Recibe parejas de puntos y calcula el potencial entre éstos*/
float proc(float pxi,float pyi, float pzi, float pxj,float pyj, float pzj){
	float res=0, uno, dos, tres;
	uno=pow(pxi-pxj,2);
	dos=pow(pyi-pyj,2);
	tres=pow(pzi-pzj,2);
	
	res=1/sqrt(uno+dos+tres);
	
	return res;	
}
