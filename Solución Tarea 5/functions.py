# encoding: UTF-8
import numpy as np
from scipy.interpolate import interp1d
import matplotlib.pyplot as plt

"""
Módulo que contiene las funciones para el análisis de datos de manchas solares
Authors: 	Nancy Ruiz 
			Camilo Rivera
Creation date: Tue Oct  8 
"""

def cargar_archivo(nombre):
	"""
	Esta función se encarga de cargar el archivo con el nombre dado
	
	Input:nombre
	Output: numpy.ndarray con los datos de observaciones de manchas solares
	"""
	
	datos=np.loadtxt(nombre)
	
	return datos


def arreglar_datos(datos):
	"""
	Esta función se encarga de recibir la matriz de observaciones directamente del archivo y a partir de ésta genera un vector
	temporal coherente y elimina aquellas observaciones para las cuales se tiene un valor de -99
	Input: numpy matrix correspondiente a la del archivo
	Output: tiempo numpy array con el vector temporal
			obser numpy array con el vector de observaciones correspondiente
	"""
	
	#Primero se eliminan los datos previos a 1645 por su carencia de aporte al proceso de interpolación (anteriores a línea 50)
	datos=datos[50:,:]
	#Se crea un nuevo vector omitiendo las mediciones con -99
	dat2=np.array([0,0,0,0])
	for i in range(len(datos)):
		if(datos[i,3]!=-99.0):
			dat2=np.row_stack((dat2,datos[i,:]))
	#Se elimina la primera fila que se creó de ceros para el buen funcionamiento de la función row_stack
	dat2=np.delete(dat2,0,0)
	#Se crea un vector temporal a partir de los años y meses con el mes como unidad
	tiempo=np.zeros((len(dat2),))
	for i in range(len(dat2)):
		tiempo[i]=(dat2[i,0]-dat2[0,0])*12+dat2[i,1]
	
	obser=dat2[:,3] 
	return obser,tiempo  


def interpol_con(x_in,y_in,x_fin):
	
	"""
	Función que realiza una interpolación de datos de manera constante
	generando un vector con la cantidad de puntos especificada
	
	Input: 	numpy array x_in vector de eje x
			numpy array y_in vector de eje y
			numpy array x_fin es el eje temporal del resultado de la interpolación
	Output:	y_out vector de eje y de salida con interpolación
	"""
	f= interp1d(x_in,y_in, kind="nearest")
	return f(x_fin)


def interpol_lin(x_in,y_in,x_fin):
	"""
	Función que realiza una interpolación de datos de manera lineal
	generando un vector con la cantidad de puntos especificada
	
	Input: 	numpy array x_in vector de eje x
			numpy array y_in vector de eje y
			numpy array x_fin es el eje temporal del resultado de la interpolación
	Output:	y_out vector de eje y de salida con interpolación
	"""
	f1 = interp1d(x_in, y_in, kind='linear')
	
	return f1(x_fin)
	
	
def interpol_cub(x_in,y_in,x_fin):
	"""
	Función que realiza una interpolación con spline cúbico
	generando un vector con la cantidad de puntos especificada
	
	Input: 	numpy array x_in vector de eje x
			numpy array y_in vector de eje y
			numpy array x_fin es el eje temporal del resultado de la interpolación
	Output:	y_out vector de eje y de salida con interpolación
	"""
	f2 = interp1d(x_in, y_in, kind='cubic')
	"""
	xnew = np.linspace(0, 10, 40)
	plt.plot(x_in,y_in,'o',xnew,f2(xnew),'-')
	plt.legend(['data', 'cubic'], loc='best')
	plt.show()
	"""
	
	return f2(x_fin)

def extraer(eje_y, eje_x):
	"""
	Función que iguala a cero aquellas potencias que están en frecuencias
	menores a 1/(20 años) y mayores 1/(2 años)
	Input: 	numpy array eje_x vector de frecuencias
			numpy array eje_y vector de potencias
	Output: numpy array de potencias extraídas
	"""
	lon=len(eje_x)
	ini= 1/240.0
	fin= 1/24.0
	#Se crea un nuevo vector de manera que no se destruya el inicial
	vector = np.empty_like(eje_y)
	vector[:] = eje_y
	for i in range(lon):
		if(np.abs(eje_x[i])<ini or np.abs(eje_x[i])>fin):
			vector[i]=0
	return vector
	
def plot_dispar(t_ini,t_inter,orig,cons,lin,cub):
	"""
	Esta función se encarga de realizar una gráfica con los datos originales
	y los datos aproximados de las 3 interpolaciones y con el filtrado.
	Debe tener en cuenta que los datos originales y los nuevos no están 
	bajo el mismo vector temporal.
	Todos los inputs son numpy array
	Input:	t_ini es un vector temporal correspondiente a los datos originales sin interpolar
			t_inter es el vector temporal correspondiente a la interpolación
			orig es el vector de mediciones originales
			lin, cons, cub son los respectivamente los vectores resultado de las interpolaciones 
					lineal, constante y cúbica filtradas
	"""
	#Modificar vector de tiempo de manera que se tenga en términos de años
	t_ini=t_ini/12
	t_inter=t_inter/12
	#Generación de gráfica General
	fig = plt.figure()
	plt.subplots_adjust(hspace=0.7)
	#Gráfica de aproximación constante
	const = fig.add_subplot(3,1,1)
	const.plot(t_ini,orig,'b',t_inter,np.abs(cons),'r')
	tt=const.set_title(u"Gráfica original con aproximación constante")
	const.set_xlabel(u'Años')
	const.set_ylabel(u'# Manchas')
	const.legend([u'Original', u'Apr. Constante'])
	#Gráfica de aproximación Lineal
	linea = fig.add_subplot(3,1,2)
	linea.plot(t_ini,orig,'b',t_inter,np.abs(lin),'r')
	tt=linea.set_title(u"Gráfica original con aproximación linear")
	linea.set_xlabel(u'Años')
	linea.set_ylabel(u'# Manchas')
	linea.legend([u'Original', u'Apr. Lineal'])
	#Gráfica de aproximación cúbica
	cubi = fig.add_subplot(3,1,3)
	cubi.plot(t_ini,orig,'b',t_inter,np.abs(cub),'r')
	tt=cubi.set_title(u"Gráfica original con aproximación cúbica")
	cubi.set_xlabel(u'Años')
	cubi.set_ylabel(u'# Manchas')
	cubi.legend([u'Original', u'Apr. Cúbica'])
	plt.show()
	
	
	
	
def pico(vector,freq,tipo):
	"""
	Esta función encuentra el pico más alto diferente del cero a partir del espectro de potencias
	y a partir de esta halla el período solar en las coordenadas correctas
	Input: 	numpy vector de espectro de potencias
			numpy vector de frecuencias
	Output:	Double con período
	"""
	mayor=0
	ind_mayor=-1
	#Se halla el mayor que corresponda a un pico y que no sea en f=0
	for i in range(1,len(vector)-1):
		if(vector[i]>vector[i-1] and vector[i]>vector[i+1] and freq[i]!=0):
			if(vector[i]>mayor):
				mayor=vector[i]
				ind_mayor=i
	#Se calcula el período en meses y años
	per=np.abs(1/freq[ind_mayor])
	pera=per/12
	print "El Período según la "+tipo+" es aproximadamente "+str(per)+" meses; es decir aproximadamente "+str(per/12)+" años"
	
	
	return pera
	
