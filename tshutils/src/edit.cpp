#include <iostream>
#include <string.h>
#include <fstream>
#include <thread>

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


char buff;
bool end = false, loaded = false;

void getText(){
    int read;
    while ((read = getc(stdin)) != EOF){
        if (end || read == 27)
            break;

        if (read == 'z') break; //TODO: Retirer cette ligne a la remise.

        while (loaded);
        buff = (char)read;
        loaded = true;
    }
    end = true;
}

void writeText(std::fstream& file){
    bool endOfSentence = true, space = true;
    while (1){
        if (loaded) {
            if (buff == '.' || buff == '!' || buff == '?') {
                endOfSentence = true;
                space = false;
            } else if (isspace(buff)) {
                space = true;
            } else {
                if (endOfSentence && !space){
                    std::cout << ' ';
                    file << ' ';
                }
                buff = endOfSentence ? (char) toupper(buff) : buff;
                endOfSentence = false;
                space = false;
            }
            if (buff == '\b') std::cerr << '!';

            file << buff;
            std::cout << buff; //TODO: Retirer cette ligne a la remise.
            loaded = false;
        } else if (end) {
            break;
        }
    }
}

int main (int argc, char *argv[]) {

    if (argc != 2){
        std::cout << "Nombre d'arguments invalides." << std::endl;
        return 0;
    }

    std::fstream file;
    file.open(argv[1], std::ifstream::out);
    int tempErrno = errno;
    if (tempErrno) {
        std::cout << strerror(tempErrno);
        return 0;
    }

    std::thread load(getText);
    std::thread save(writeText, std::ref(file));

    load.join();
    file.close();
    save.join();

}