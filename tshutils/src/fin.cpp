/**
fin <nbLignes> <fichier>
La commande fin sert à afficher un certain nombre de lignes d'un fichier à partir de la fin du fichier.
Le paramètre nbLignes est le nombre de lignes à afficher, ce doit être un entier positif. Le paramètre
fichier est un chemin relatif ou absolu vers le fichier à lire. Une erreur est affichée si le fichier est
inexistant.
tsh> fin 5 inf3172/tp1/source.c
Fichier introuvable
tsh> fin 3 inf3172/tp1/sourceTP1.c
 // fin normale du programme
 exit(1);
}
tsh> fin test fichier.txt
Argument invalide
tsh>
 */