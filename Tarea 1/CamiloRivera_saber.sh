wget  http://www.finiterank.com/saber/2011.csv
wget https://raw.github.com/forero/ComputationalPhysicsUniandes/master/hands_on/unix/columnas_2011.csv.txt
echo 'Número de colegios públicos (oficiales)'
awk -F "\"*,\"*" '{print $5}' 2011.csv | grep "SI"|wc -l
echo 'Número de colegios privados (no oficiales)'
awk -F "\"*,\"*" '{print $5}' 2011.csv | grep "NO"|wc -l
echo 'Número de colegios privados calendario A'
awk -F "\"*,\"*" '{print $5$8}' 2011.csv|grep "NOA"|wc -l
echo 'Número de colegios privados calendario B'
awk -F "\"*,\"*" '{print $5$8}' 2011.csv|grep "NOB"|wc -l
echo 'Número de colegios públicos calendario B'
awk -F "\"*,\"*" '{print $5$8}' 2011.csv|grep "SIB"|wc -l
