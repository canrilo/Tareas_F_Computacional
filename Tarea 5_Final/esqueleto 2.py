# encoding: UTF-8
import matplotlib.pyplot as plt
import numpy as np
import functions as fc
from scipy.fftpack import fft, fftfreq

def main():
	#Espaciamiento entre puntos en meses
	n=0.1
	
	
	#Nombre del archivo de entrada
	nom_archivo='sparse_sample_monthrg.dat'
	
	#Lectura de archivo y almacenamiento en numpy.ndarray
	datos = fc.cargar_archivo(nom_archivo)
	
	#Se seleccionan los datos de las observaciones y se organizan para la interpolación. 
	obs=datos[:,3]			#Vector de observaciones (se tienen que remover mediciones nulas -99) 
	#tiempo=....
	tiempo=np.arange(0,10) 	#tiempo el vector temporal
	#REVISAAAAARRRRRR
	
	#Se genera el vector temporal n-espaciado que se usará para las interpolaciones 
	tiempointer=np.arange(tiempo[0],tiempo[int(len(tiempo)-1)]+n,n) #Vector con mismas cotas que el original
	
	#Realiza una interpolación constante con n puntos
	datoscons=fc.interpol_con(tiempo,obs,tiempointer)
	
	#Realiza una interpolación lineal con n puntos
	datoslin=fc.interpol_lin(tiempo,obs,tiempointer)
	
	#Realiza una interpolación cúbica con n puntos
	datoscub=fc.interpol_cub(tiempo,obs,tiempointer)
	
	#Cálculo de transformadas de fourier
	freq=freq = fftfreq(int(len(datoscub)), n) #Eje de frecuencias
	FFcons=fft(datoscons)/len(datoscons)
	FFlineal=fft(datoslin)/len(datoslin)
	FFcub=fft(datoscub)/len(datoscub)
	
	#Graficar los espectros de potencia **FALTA CAMBIAR LABELS
	fig = plt.figure()
	const = fig.add_subplot(3,1,1)
	const.plot(freq,np.abs(FFcons))
	const.set_yscale('log')
	const.set_xscale('log')
	lin = fig.add_subplot(3,1,2)
	lin.plot(freq,np.abs(FFlineal))
	lin.set_yscale('log')
	lin.set_xscale('log')
	cub = fig.add_subplot(3,1,3)
	cub.plot(freq,np.abs(FFcub))
	cub.set_yscale('log')
	cub.set_xscale('log')
	
	#Extraer frecuencias de interés
	FFconsCor=fc.extraer(FFcons,freq)
	FFlinealCor=fc.extraer(FFlineal,freq)
	FFcubCor=fc.extraer(FFcub,freq)
	
	#Transformada inversa de Fourier
	aproxCons=np.fft.ifft(FFconsCor)			#Hay que revisar la función en caso de una normalización
	aproxLineal=np.fft.ifft(FFlinealCor)
	aproxCub=np.fft.ifft(FFcubCor)
	
	#Graficar nuevamente    #Hay que revisar nuevamente los ejes temporales para que coincidan con los iniciales
	fc.plot_dispar(tiempo,tiempointer, obs, aproxCons, aproxLineal,aproxCub)
	
	#Encontrar pico
	periodo_cons=fc.pico(FFconsCor)
	periodo_lin=fc.pico(FFlinealCor)
	periodo_cub=fc.pico(FFcubCor)
	
	
if __name__ == "__main__":
    main()

