# encoding: UTF-8
import math
import sys

#Primero se comprueba la validez de la entrada
if len(sys.argv)!=2:
	print "El comando debe ejecutarse con una única entrada numérica"
	quit()

num=int(sys.argv[1])

if (num<0 or num>1000000):
	print "Entrada no válida. Número debe estar entre 0 y 10^6"
	quit()
cont=0
i=2
primos=[0,0,0]
while i<=num:
	if(num%i==0):
		primos[cont]=i
		cont+=1
		num/=i
		if(cont==2):
			break
	else:
		if(i==2):
			i=3
		else:
			i+=2

if(cont!=2 or num!=1):
	print "El número no puede descomponerse en el producto de dos primos. \nLo siento :("
else:
	print `primos[0]`+" "+`primos[1]`
