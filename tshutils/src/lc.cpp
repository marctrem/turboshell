/**
La commande lc (line count) ne prend aucun paramètre. Elle sert à faire la liste des fichiers d'un
répertoire avec le nombre de lignes de texte contenues à l'intérieur. Les fichiers de la liste sont les
fichiers du répertoire courant seulement. Les répertoires sont exclus et les fichiers dans les sousrépertoires
également. Le chemin absolu du fichier est indiqué.
tsh> lc
/user/kc591304/tp1/main.c contient 675 lignes
/user/kc591304/tp1/makefile contient 34 lignes
/user/kc591304/tp1/main.h contient 44 lignes
tsh>
Un thread doit être démarré pour chaque fichier à calculer. Lorsqu'un thread se termine,
l'information sur le fichier qu'il traitait est immédiatement affiché, donc l'ordre des fichiers n'est pas
important. En aucun cas, il ne devrait y avoir plus de dix threads actifs simultanément. S'il y a plus
de dix fichiers dans le répertoire, il faudra attendre que certains threads se terminent avant d'en
démarrer d'autres.
*/