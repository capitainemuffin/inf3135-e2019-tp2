options=-Wall -pedantic -std=c11
CP = $(shell cat cp.txt)
IN = data/crypto-msg01.in
OUT = data/crypto-msg01.out
OUT_TEST = data/crypto-msg01.test
CLE = $(shell cat data/crypto-msg01.cle)
action = $(shell cat data/crypto-msg01.action)

.PHONY : data clean default test resultat push

default : tp1

tp1 : tp1.o
	gcc tp1.o -o tp1 $(options)

tp1.o : tp1.c
	gcc tp1.c -c -o tp1.o $(options)

clean : 
	rm -fr *.o tp1 alphabet.txt data

test : tp1
	cp data/crypto-msg01.alphabet data/alphabet.txt
	./tp1 -c $(CP) $(action) -k $(CLE) -i $(IN) -o $(OUT_TEST) -a ./data/
	diff $(OUT) $(OUT_TEST)

data :
	curl https://www.github.com/guyfrancoeur/INF3135_E2019_TP/raw/master/crypto-data.zip -sLO -o crypto-data.zip
	if [ ! -d "data" ]; then mkdir data; fi
	mv crypto-data.zip data/
	unzip -o data/crypto-data.zip -d data/
	rm -f data/crypto-data.zip

resultat : note-du-tp.txt
	git add note-du-tp.txt
	git commit -m 'note du tp'
	git push origin master

push :
	git add .
	git commit -m 'commit et push automatique'
	git push origin master


