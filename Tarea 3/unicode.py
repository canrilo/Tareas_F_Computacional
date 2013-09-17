# encoding: UTF-8
import sys
import unicodedata
 
""" Normalise (normalize) unicode data in Python to remove umlauts, accents etc. """
 
data = u'naïve café'
normal = unicodedata.normalize('NFKD', data).encode('ASCII', 'ignore')
print normal

infile=open(sys.argv[1],'r')
texto=infile.readlines()
for linea in texto:
	linea3=linea.decode("utf-8")
	linea2=unicodedata.normalize('NFKD', linea3)
	for letra in linea2:
		print letra

