# encoding: UTF-8
import sys
import unicodedata

#Definiciones útiles
vocalesmin=['a','e','i','o','u']
contvoc=[0,0,0,0,0]

#Primero se comprueba la validez de la entrada
if len(sys.argv)!=2:
	print "El comando debe ejecutarse con una única entrada"
	quit()
 
#Se importa y lee el archivo
infile=open(sys.argv[1],'r')
texto=infile.readlines()
for linea in texto:
	linea=linea.decode("utf-8")
	linea=unicodedata.normalize('NFKD', linea).encode('ASCII', 'ignore')
	for letra in linea:
		for vo in range(5):
			if(letra.lower()==vocalesmin[vo]):
				contvoc[vo]+=1

for i in range(5):
	print (vocalesmin[i]+" "+str(contvoc[i]))
