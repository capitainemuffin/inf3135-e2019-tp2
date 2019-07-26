   # Travail pratique 1

   ## Description

   Le programme encode et décode des messages en utilisant une clé simple et un alphabet.

   `Cours` : Construction et maintenance de logiciels

   `Sigle` : inf3135

   `Session` : Été 2019

   `Université` : UQAM

   ## Auteur

   Sofiane Selaoui (SELS28049204)

   ## Fonctionnement

   Le programme encode ou décode des messages en utilisant une clé et un alphabet. Par défaut, `stdin` et `stdout` sont utilisés comme entrée-sortie.

   Pour que le programme fonctionne, il faut minimalement :

   - Une clé, avec l'option `-k` suivie d'un nombre
   - L'option `-e` (encrypter) ou `-d` (décrypter)
   - Un alphabet, par défaut `alphabet.txt` du répertoire courant
   - L'option `-c`, suivi d'un code permanent de longueur 12

   Il est aussi possible d'utiliser les options suivantes :

   - `-i` suivi d'un fichier source en entrée
   - `-o` suivi d'un fichier en sortie 
   - `-a` suivi d'un chemin vers un fichier `alphabet.txt`

   Lorsque l'option `-e` (encrypter) est choisie, le programme décale la valeur de chaque caractère selon la clé donnée et les choix possibles dans l'alphabet.

   Si le caractère n'est pas présent dans l'alphabet, le programme ne peut pas le modifier.

   Si l'option `-d` (décrypter) est choisie, le décalage se fera dans le sens inverse. 

   ## Codes d'erreurs 
   
   Il n'y a pas de messages d'erreurs, seulement des codes d'erreurs : 

   - `1` : il n'y a aucun d'argument ou l'argument -c n'est pas présent;
   - `2` : l'argument -c est présent, mais le code n'est pas 12 de long;
   - `3` : un argument non voulu est présent. Voici un exemple : -t BLA;
   - `4` : l'argument -d ou -e n'est pas présent;
   - `5` : une erreur (lecture, ouverture, présence ...) avec le fichier en entrée;
   - `6` : une erreur (création, ...) avec le fichier en sortie; Attention Si le fichier existe il faut l'écraser;
   - `7` : l'argument -k n'est pas présent, ou la valeur passée n'est pas conforme;
   - `8` : l'argument -a est présent, mais impossible d'ouvrir le fichier alphabet.txt;

   Référence : [guyfrancoeur/INF3135_E2019_TP](https://github.com/guyfrancoeur/INF3135_E2019_TP/blob/master/tp1.md)

   ### Exemples

   Commande : 

   `echo wow | ./tp1 -c SELS28049204 -e -k 1`

   Sortie : 

   ```
   xpx
   ```

   Commande : 

   `cat mystere.txt ; ./tp1 -c SELS28049204 -d -k -2 -i mystere.txt`

   Sortie : 

   ```
   pmlyjbm
   ronaldo
   ```

   ## Contenu du projet

   `Makefile`
   ```
   Automatise la compilation, les tests et la gestion des fichiers.
   ```

   `cp.txt`
   ```
   Contient le code permanent.
   ```

   `data`
   ```
   Va contenir les fichiers obtenus avec la commande `make data`, pour les tests.
   ```

   `tp1.c`
   ```
   Contient le code source et la fonction main.
   ```

   ## Références

   - [guyfrancoeur/INF3135_E2019](https://github.com/guyfrancoeur/INF3135_E2019)
   - [Cplusplus](http://www.cplusplus.com/reference/clibrary)
   - [Tutorialspoint](https://www.tutorialspoint.com/cprogramming)
   - [Markdown cheatsheet](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)

   ## Statut

   - Complété