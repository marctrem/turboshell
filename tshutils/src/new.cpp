#include <iostream>
#include <list>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "../../libpathm/include/path.hpp"

using namespace std;
using namespace pathm;

int main(int argc, char *argv[]) {

    int errval;
    path cwd = path::get_current_path();

    if (argc == 1) {
        // Todo: Display usage
        return 0;
    }

    // Create a file for each argument.
    for (int i = 1; i < argc; i++) {
        path full_path = path(argv[i]).make_absolute(cwd);

        // Check if there is something on this path

        errno = 0;
        access(full_path.c_str(), F_OK);
        errval = errno;

        switch (errval) {
            case ENOENT: { // Nothing on path: this is what we want.
                errno = 0;
                FILE *new_file = fopen(full_path.c_str(), "w");
                errval = errno;
                if (!errval) {
                    // Success
                    fclose(new_file);
                    std::cout << "Fichier créé" << std::endl;
                }
                else {
                    // Problem
                    std::cout << strerror(errval) << std::endl;
                }
                break;
            }

            case 0: { //SUCCESS
                std::cout << "Impossible de créer le fichier" << std::endl;
                break;
            }

            default: {
                std::cout << strerror(errval) << std::endl;
                break;
            }
        }

    }

    return 0;
}