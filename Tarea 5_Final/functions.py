# encoding: UTF-8
import numpy as np

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
	
	#loadtxt(nombre)
	
	return np.ones((10,4))
	
def interpol_con(x_in,y_in,x_fin):
	"""
	Función que realiza una interpolación de datos de manera constante
	generando un vector con la cantidad de puntos especificada
	
	Input: 	numpy array x_in vector de eje x
			numpy array y_in vector de eje y
			numpy array x_fin es el eje temporal del resultado de la interpolación
	Output:	y_out vector de eje y de salida con interpolación
	"""
	x_out=np.ones((int(len(x_in)),1))
	y_out=np.ones((int(len(x_in)),1))
	return y_out


def interpol_lin(x_in,y_in,x_fin):
	"""
	Función que realiza una interpolación de datos de manera lineal
	generando un vector con la cantidad de puntos especificada
	
	Input: 	numpy array x_in vector de eje x
			numpy array y_in vector de eje y
			numpy array x_fin es el eje temporal del resultado de la interpolación
	Output:	y_out vector de eje y de salida con interpolación
	"""
	x_out=np.ones((int(len(x_in)),1))
	y_out=np.ones((int(len(x_in)),1))
	return y_out
	
	
def interpol_cub(x_in,y_in,x_fin):
	"""
	Función que realiza una interpolación con spline cúbico
	generando un vector con la cantidad de puntos especificada
	
	Input: 	numpy array x_in vector de eje x
			numpy array y_in vector de eje y
			numpy array x_fin es el eje temporal del resultado de la interpolación
	Output:	y_out vector de eje y de salida con interpolación
	"""
	x_out=np.ones((int(len(x_in)),1))
	y_out=np.ones((int(len(x_in)),1))
	return y_out

def extraer(eje_x, eje_y):
	"""
	Función que iguala a cero aquellas potencias que están en frecuencias
	menores a 1/(20 años) y mayores 1/(2 años)
	Input: 	numpy array eje_x vector de frecuencias
			numpy array eje_y vector de potencias
	Output: numpy array de potencias extraídas
	"""
	lon=len(eje_x)
	ini=1/20
	fin=1/2
	for i in range(lon):
		if(eje_x[i]<ini or eje_x[i]>fin):
			eje_y[i]=0
	
	return eje_y
	
def plot_dispar(t_ini,t_inter,orig,lin,cons,cub):
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
	
def pico(vector):
	"""
	Esta función encuentra el pico más alto diferente del cero a partir del espectro de potencias
	y a partir de esta halla el período solar en las coordenadas correctas
	Input: 	numpy vector de espectro de potencias
	Output:	Double con período
	"""
	
	return 42
	
