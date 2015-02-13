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
            std::cout << "Impossible de créer le fichier. Chemin déjà occupé: '" << full_path << "'" << std::endl;
            return 0;
        }

        errno = 0;
        FILE *new_file = fopen(full_path.c_str(), "w");

        if(!errno) {
            // Success
            fclose(new_file);
        }
        else {
            // Problem
            std::cout << "Impossible de créer le fichier: '" << full_path << "'" << std::endl;
        }
    }

    return 0;
}