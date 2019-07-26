#!/bin/bash
# evaluer-tp2.sh
if [ -z $1 ]
then
  correction=inf3135-e2019-tp1.correction
else
  correction=$1
fi
if [ ! -f ${correction} ] ; then
  curl https://github.com/guyfrancoeur/INF3135_E2019_TP/raw/master/${correction} -o ${correction}
fi
if [ ! -f ${correction} ]
then
  echo "erreur fatale, fichier correction inexistant."
  exit 1
fi

while read -r line
do

	# si commence par $ -> executer, 
	# sinon executer à partir du byte 30 
  echo -e $line
done < ${correction}