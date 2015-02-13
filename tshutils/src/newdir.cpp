#include <iostream>
#include <list>
#include <string.h>
#include <sys/stat.h>

#include "../../libpathm/include/path.hpp"

using namespace std;
using namespace pathm;

int main(int argc, char *argv[]) {

    path cwd = path::get_current_path();

    if (argc == 1) {
        // Todo: Display usage
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        path full_path = path(argv[i]).make_absolute(cwd);

        // Check if there is something on this path
        struct stat path_stat;

        errno = 0;
        stat(full_path.c_str(), &path_stat);

        // We want stat to fail.
        if (!errno) {
            std::cout << "Impossible de créer le répertoire. Chemin déjà occupé: '" << full_path << "'" << std::endl;
            return 0;
        }

        errno = 0;

        mkdir(full_path.c_str(), 0770);

        if(!errno) {
            // Success
            std::cout << "Répertoire créé" << std::endl;
        }
        else {
            // Problem
            std::cout << "Impossible de créer le répertoire: '" << full_path << "'" << std::endl;
        }
    }

    return 0;
}