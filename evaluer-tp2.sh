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
index=1
note=0
code_perm=`cat cp.txt`
repertoire=`pwd | rev | cut -d' ' -f1 | rev`

for line in `cat ${correction}`; do

	points=`echo $line | cut -d ' ' -f1`
	temps_max=`echo $line | cut -d ' ' -f2`
	code_retour=`echo $line | cut -d ' ' -f3`
	description=${line:7:22}
	commande=${line:29};

	eval $commande &> /dev/null
	test $? -eq ${code_retour} && echo $index : réussi : ${points} pts && note=$((note + points)) || echo $index : echec; 
	index=$((index + 1))
done

echo "Note (total) pour $code_perm dans ${repertoire}: $note" 
