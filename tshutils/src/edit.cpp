/**
La commande edit sert à sauvegarder, en temps réel ou presque, l'entrée standard dans un fichier
texte en modifiant le texte à la volée. La saisie du texte se termine lorsque le programme reçoit le
caractère <escape> ou EOF. Chaque caractère saisie est affiché à l'écran, mais le texte est modifié
avant d'être sauvegardé sur disque. Les modifications au texte touche la ponctuation. Lorsqu'un
caractère de ponctuation est saisie ".!?", on s'assure que le caractère suivant est un espace et que
l'espace est suivi d'une lettre majuscule. La toute première lettre du texte doit être une majuscule. Le
texte est modifié pour respecter ces exigences.
Pour la saisie suivante :
j'aime les fruits. c'est delicieux!rafraichissant!Surtout
l'ete.j'adore les fruits.
Le texte sauvegardé dans le fichier sera :
J'aime les fruits. C'est delicieux! Rafraichissant! Surtout l'ete.
J'adore les fruits.
Le caractère backspace doit également être supporté, c'est-à-dire que la saisie "Croustilll<BS>es"
produira "Croustilles" dans le fichier de sortie.
Deux threads doivent exister dans ce programme au minimum, un pour la saisie de l'entrée standard,
l'autre pour l'écriture dans le fichier de sortie. Il est possible d'utiliser plus de deux threads si c'est
nécessaire.
 */