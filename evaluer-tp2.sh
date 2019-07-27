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

IFS=$'\n'
for line in `cat ${correction}`; do

	points=`echo $line | cut -d ' ' -f1`
	temps_max=`echo $line | cut -d ' ' -f2`
	code_retour=`echo $line | cut -d ' ' -f3`
	description=${line:7:22}
	commande=${line:29};
	timeout ${temps_max} eval $commande &> /dev/null

	test $? -eq ${code_retour} && echo Réussi : ${points} pts || echo Erreur; 

done

echo "Note totale" 