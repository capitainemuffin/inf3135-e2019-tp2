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

IFS=$'\t'

while read -r line; do

	points=`echo $line | cut -d ' ' -f1`
	temps_max=`echo $line | cut -d ' ' -f2`
	code_retour=`echo $line | cut -d ' ' -f3`
	description=${line:7:22}
	commande=${line:29};

	$commande;

done < ${correction}