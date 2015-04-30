#include <iostream>
#include <fstream>
#include "../../libpathm/include/file_util.hpp"
#include <string.h>
#include <sstream>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Nombre d'arguments invalides." << std::endl;
        return 0;
    }
    std::ifstream fileToRead(argv[2], std::ifstream::binary);

    int tempErrno = errno;
    if (tempErrno) {
        std::cout << strerror(tempErrno);
        return 0;
    }
    long file_lc = line_count(fileToRead);
    if (file_lc == -1) {
        std::cout << "Erreur lors de la lecture du fichier." << std::endl;
        return 0;
    }

    long qty = atol(argv[1]);
    if (qty < 0){
        std::cout << "Le nombre de ligne a afficher doit etre positif." << std::endl;
        return 0;
    }

    std::stringstream numberStream;
    numberStream << qty;
    if (numberStream.str() != argv[1]) {
        std::cerr << "\"" << argv[1] << "\" n'est pas un chiffre" << std::endl;
        return 0;
    }

    long startToRead = file_lc - (qty - 1);
    startToRead = startToRead > 0 ? startToRead : 0;

    char buff;
    while (fileToRead.get(buff)) {
        if (buff == '\n' && startToRead > 1) {
            startToRead--;
            continue;
        }

        if (startToRead <= 1)
            std::cout << buff;
    }

    fileToRead.close();

    return 0;
}
