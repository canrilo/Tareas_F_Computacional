# encoding: UTF-8
import matplotlib.pyplot as plt
import numpy as np
import functions as fc
from scipy.fftpack import fft, fftfreq

def main():
	#Espaciamiento entre puntos en meses
	n=1	
	
	
	
	#Nombre del archivo de entrada
	nom_archivo='sparse_sample_monthrg.dat'
	
	#Lectura de archivo y almacenamiento en numpy.ndarray
	datos = fc.cargar_archivo(nom_archivo)
	
	#Se seleccionan los datos de las observaciones y se genera un vector de tiempo.
	obs,tiempo = fc.arreglar_datos(datos)
	plt.plot(tiempo,obs)
	plt.title('Datos Observaciones')
	plt.xlabel('Meses')
	plt.ylabel('Observaciones')
	plt.show()		
	
	#Se genera el vector temporal n-espaciado que se usará para las interpolaciones 
	tiempointer=np.arange(tiempo[0],tiempo[int(len(tiempo)-1)]+n,n) #Vector con mismas cotas que el original
	#print tiempo,tiempointer
	
	#Realiza una interpolación constante con n puntos
	datoscons=fc.interpol_con(tiempo,obs,tiempointer)
	
	#Realiza una interpolación lineal con n puntos
	datoslin=fc.interpol_lin(tiempo,obs,tiempointer)
	
	#Realiza una interpolación cúbica con n puntos
	datoscub=fc.interpol_cub(tiempo,obs,tiempointer)
	
	
	#Se define la longitud final de los vectores la cual será útil de aquí en adelante
	num=len(tiempointer)
	#Cálculo de transformadas de fourier
	freq = fftfreq(int(num), n) #Eje de frecuencias
	FFcons=fft(datoscons)/num
	FFlineal=fft(datoslin)/num
	FFcub=fft(datoscub)/num
	
	#Reorganización de los vectores de transformadas de Fourier
	freq=np.fft.fftshift(freq)
	FFcons=np.fft.fftshift(FFcons)
	FFlineal=np.fft.fftshift(FFlineal)
	FFcub=np.fft.fftshift(FFcub)
	
	#Graficar los espectros de potencia con sus respectivos labels
	plt.close('all')
	fig = plt.figure()
	plt.subplots_adjust(hspace=0.7)
	const = fig.add_subplot(3,1,1)
	const.plot(freq,np.abs(FFcons))
	const.set_yscale('log')
	tt=const.set_title(u"Espectro de potencia de Interpolacion constante")
	const.set_xlabel(u'Frecuencia')
	const.set_ylabel(u'[Log] Potencia')
	#const.set_xlim([-0.01,0.01])
	#const.set_xscale('log')
	lin = fig.add_subplot(3,1,2)
	lin.plot(freq,np.abs(FFlineal))
	lin.set_yscale('log')
	ttt=lin.set_title(u"Espectro de potencia de Interpolacion lineal")
	lin.set_xlabel(u'Frecuencia')
	lin.set_ylabel(u'[Log] Potencia')
	#lin.set_xlim([-0.01,0.01])
	#lin.set_xscale('log')
	cub = fig.add_subplot(3,1,3)
	cub.plot(freq,np.abs(FFcub))
	cub.set_yscale('log')
	cub.set_title(u"Espectro de potencia de Interpolacion cúbica")
	cub.set_xlabel(u'Frecuencia')
	cub.set_ylabel(u'[Log] Potencia')
	#cub.set_xlim([-0.01,0.01])
	#cub.set_xscale('log')
	plt.show()
	
	
	#Extraer frecuencias de interés
	FFconsCor=fc.extraer(FFcons,freq)
	FFlinealCor=fc.extraer(FFlineal,freq)
	FFcubCor=fc.extraer(FFcub,freq)
	
	#Transformada inversa de Fourier
	aproxCons=np.fft.ifft(FFconsCor)*num			
	aproxLineal=np.fft.ifft(FFlinealCor)*num
	aproxCub=np.fft.ifft(FFcubCor)*num
	
	
	#Graficar nuevamente    #Hay que revisar nuevamente los ejes temporales para que coincidan con los iniciales
	fc.plot_dispar(tiempo,tiempointer, obs, aproxCons, aproxLineal,aproxCub)
	
	#Encontrar pico a partir de la norma al cuadrado de las transformadas de Fourier
	periodo_cons=fc.pico(np.power(np.abs(FFconsCor),2),freq,'Aproximación constante')
	periodo_lin=fc.pico(np.power(np.abs(FFlinealCor),2),freq,'Aproximación lineal')
	periodo_cub=fc.pico(np.power(np.abs(FFcubCor),2),freq,'Aproximación cúbica')
	
	
if __name__ == "__main__":
    main()

