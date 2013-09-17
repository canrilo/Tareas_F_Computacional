#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv){
	int num=0,d=2,cont=0;
	int primos[2]={0,0};
	
	if(argc!=2){
	printf("Este programa debe recibir un único número!\n EXIT!\n");
	exit(1);
	}
	
	num = atoi(argv[1]);
	
	if(num>1000000 || num<0){
		printf("Entrada no válida. Número debe estar entre 0 y 10^6\n");
		exit(1);
	}
	
	while(d<num && cont<2){
		if(num % d == 0) {
		  primos[cont]=d;
		  num /= d;
		  cont++;
		}
		
		else {
			if(d == 2){
				d = 3;
			}
		  else 
				d += 2;
		}
  }
  
  if (cont>=2 || cont==0){
	  printf("No se ha podido realizar la descomposición prima. \nEl número ingresado no puede expresarse como producto de dos primos\n");
  }else{
	  primos[1]=d;
	  printf("La descomposición prima es:\n %d %d\n",primos[0],primos[1]);
  }	

	return 0;
}
