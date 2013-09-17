# encoding: UTF-8
import sys
import unicodedata
import string
import re

#Primero se comprueba la validez de la entrada
if len(sys.argv)!=2:
	print "El comando debe ejecutarse con una única entrada numérica"
	quit()

#Se importa y lee el archivo
infile=open(sys.argv[1],'r')
texto=infile.readlines()

for linea in texto:
	linea=linea.decode("utf-8")
	linea=unicodedata.normalize('NFKD', linea).encode('ASCII', 'ignore')
	linea=linea.lower()
	linea=re.sub('[%s]' % re.escape(string.punctuation), '', linea)
	linea=linea.strip().replace(" ","")
	lo=len(linea)
	igual=True
	for letra in range(int(lo/2)):
		if(linea[letra]!=linea[lo-letra-1]):
			igual=False
			break
	print igual
