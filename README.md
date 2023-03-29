# Projet C
Projet de Langage C L2 de l'Université de Paris 2021/2022
***
## Compilation
1. Pour compiler tout le code:
    > `make`
2. Pour compiler le test_unbounded:
    > `make test_unbounded`
3. Pour compiler l'interpreteur:
    > `make calc_unbounded_int`
4. Pour supprimer les fichiers compilés:
    > `make clean`
***
## Execution
1. Pour lancer les tests:
    > `./test_unbounded`
2. Pour lancer l'interpreteur
   > `./calc_unbounded_int`
   ```
    Options (facultatives):
        -i fichier      Le nom du fichier texte qui contient une suite d’instructions. (Le fichier doit exister)
        -o fichier      Le nom du fichier ou il faut écrire les résultats.
        -h              Pour afficher ce message.
   
    Exemples:
       ./calc_unbounded_int -i src.txt -o dst.txt
       ./calc_unbounded_int -o dst.txt -i dst.txt
       ./calc_unbounded_int -i src.txt
       ./calc_unbounded_int -o dst.txt
       ./calc_unbounded_int
    ```
***
## Extensions
1. Pas de limite sur la longeur d'une ligne
2. Pas de limite sur la longeur du nom de la variable (Une variable doit commencer par une lettre)
3. Implémentation de la division euclidienne
4. Implémentation du modulo
***
