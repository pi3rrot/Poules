#!/bin/bash


cat <<EOF > ./lib/calsol.h
#ifndef CALSOL_H
#define CALSOL_H
/*
 * Trouvé ici et exporté en CSV puis forgé par notepad++
 * http://ptaff.ca/soleil/?l1pays=France&l1etat=Haute-Vienne&l1ville=Limoges&l2pays=&l2etat=&l1cityname=Limoges%2C+Haute-Vienne%2C+France&l1ltd=45&l1ltm=50&l1lts=07&l1ltx=N&l1lgd=1&l1lgm=15&l1lgs=45&l1lgx=E&l1tz=0.0&l1dst=&l2cityname=&l2ltd=&l2ltm=&l2lts=&l2ltx=N&l2lgd=&l2lgm=&l2lgs=&l2lgx=E&l2tz=0&l2dst=&year=19&month=02&day=15&lang=fr_CA&go=Voir+le+graphe%21
 * */
const uint8_t DateSol_t[][9] ={
EOF


sed '1d' $1 > out.tmp

while read line; do
	YEAR=$(echo $line | cut -d "," -f1 | cut -d "-" -f1)

	MONTH=$(echo $line | cut -d "," -f1 | cut -d"-" -f2)
#	if ${MONTH:0} == "0"

	DAY=$(echo $line | cut -d"," -f1 | cut -d"-" -f3)	

	HOUR_OPEN=$(echo $line | cut -d " " -f2 | cut -d":" -f1)
	MIN_OPEN=$(echo $line | cut -d " " -f2 | cut -d":" -f2)

	HOUR_CLOSE=$(echo $line | cut -d " " -f3 | cut -d":" -f1)
	MIN_CLOSE=$(echo $line | cut -d " " -f3 | cut -d ":" -f2)

	HOUR_SUN=$(echo $line | cut -d " " -f4 | cut -d":" -f1)
	MIN_SUN=$(echo $line | cut -d " " -f4 | cut -d":" -f2)

	echo {${YEAR:3}, $MONTH, ${DAY:0:2}, ${HOUR_OPEN:1}, ${MIN_OPEN:0:2}, ${HOUR_CLOSE:1}, ${MIN_CLOSE:0:2}, ${HOUR_SUN:1}, ${MIN_SUN:0:2}},    # >> ./lib/calsol.h

done < out.tmp



cat <<EOF >> ./lib/calsol.h
};

#endif // CALSOL_H
EOF

rm out.tmp
