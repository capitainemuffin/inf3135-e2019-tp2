options=-Wall -pedantic -std=c11
CP = $(shell cat cp.txt)
FICHIER = ./data/crypto-msg0

.PHONY : data clean default test resultat push valgrind

default : tp2

tp2 : tp2.o structure.o outils.o
	gcc $^ -o $@ $(options)

tp2.o : tp2.c
	gcc $< -c -o $@ $(options)

structure.o : structure.c structure.h
	gcc $< -c -o $@ $(options)

outils.o : outils.c outils.h
	gcc $< -c -o $@ $(options)

clean : 
	rm -fr *.o tp2 alphabet.txt data *.sof

cleanguy :
	make clean
	rm -fr ok *.guy

test : tp2
	cp $(FICHIER)1.alphabet alphabet.txt
	./tp2 -c $(CP) $(shell cat $(FICHIER)1.action) -k $(shell cat $(FICHIER)1.cle) -i $(FICHIER)1.in -o res1.sof
	diff $(FICHIER)1.out res1.sof

debug : tp2.c outils.c structure.c
	gcc -c outils.c -o outils.o $(options) -DDEBUG
	gcc -c structure.c -o structure.o $(options) -DDEBUG
	gcc -c tp2.c -o tp2.o $(options) -DDEBUG
	gcc -o tp2.debug outils.o structure.o tp2.o $(options) -DDEBUG
	./tp2.debug -c $(CP) -b -l ./modele -i $(FICHIER)1.out -a ./data

bruteforce : tp2
	./tp2 -c $(CP) -b -l modele -i $(FICHIER)1.out -a ./data -o bruteforce.sof
	diff bruteforce.sof $(FICHIER)1.in
	rm bruteforce.sof

data :
	curl https://www.github.com/guyfrancoeur/INF3135_E2019_TP/raw/master/crypto-data.zip -sLO -o crypto-data.zip
	if [ ! -d "data" ]; then mkdir data; fi
	mv crypto-data.zip data/
	unzip -o data/crypto-data.zip -d data/
	cp $(FICHIER)1.alphabet ./data/alphabet.txt
	rm -f data/crypto-data.zip

resultat : note-du-tp.txt
	git add note-du-tp.txt
	git commit -m 'note du tp'
	git push origin master

push :
	git add .
	git commit -m 'commit et push automatique'
	git push origin master

valgrind : tp2 data
	valgrind ./tp2 -c $(CP) $(shell cat $(FICHIER)1.action) -k $(shell cat $(FICHIER)1.cle) -i $(FICHIER)1.in -o res1.sof -a data/


