#include <stdlib.h>
#include <stdio.h>

void contar_vocales(char *filename, float *num);

int main(int argc, char **argv){
	float *num;
	int i=0;
	
	if(argc!=2){
	printf("Este programa necesita ejecutarse con exactamente una entrada adicional!\n EXIT!\n");
	exit(1);
	}
	
	num=malloc(5*sizeof(float));
	
	for(i=0;i<5;i++){
		num[i]=0;
	}
	contar_vocales(argv[1],num);
	printf("El número de veces que aparece cada vocal en el archivo es:\n");
	printf("a= %.f\n",num[0]);
	printf("e= %.f\n",num[1]);
	printf("i= %.f\n",num[2]);
	printf("o= %.f\n",num[3]);
	printf("u= %.f\n",num[4]);
		
	return 0;
}


void contar_vocales(char *filename, float *num){
	FILE *in;
	float a=0,e=0,i=0,o=0,u=0;
	int c, ind;
	if(!(in=fopen(filename, "r"))){
		printf("problem opening file %s\n", filename);
		exit(1);	
	}
	
	do{
		c = fgetc(in);
		switch(c)
		{
			case 'a' :
				a++;
				break;
			case 'A' :
				a++;
				break;
			case 'e' :
				e++;
				break;
			case 'E' :
				e++;
				break;
			case 'I' :
				i++;
				break;
			case 'i' :
				i++;
				break;
			case 'O' :
				o++;
				break;
			case 'o' :
				o++;
				break;
			case 'U' :
				u++;
				break;
			case 'u' :
				u++;
				break;				
		}
	}while(c!=EOF);
	fclose(in);
	num[0]=a;
	num[1]=e;
	num[2]=i;
	num[3]=o;
	num[4]=u;
	
}
