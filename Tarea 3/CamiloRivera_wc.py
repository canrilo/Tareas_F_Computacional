# encoding: UTF-8
import string,os

#Se importa el archivo y se cuentan las líneas
def contar_lineas(nombre):
	infile=open(nombre,'r')
	texto=infile.readlines()
	return len(texto)

archivos=os.listdir(os.getcwd())
for archivo in archivos:
	numero=contar_lineas(archivo)
	print archivo+" "+`numero`
