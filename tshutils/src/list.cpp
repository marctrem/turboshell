#include <iostream>
#include <list>
#include <string.h>
#include <sys/stat.h>

#include "../../libpathm/include/path.hpp"

using namespace std;
using namespace pathm;

int main(int argc, char *argv[]) {

    path cwd = path::get_current_path();

    bool showDirectories;

    // Todo: Make it tougher.
    if (argc == 2 && !strncmp(argv[1], "-d", 2)) {
        cout << "Sous-répertoires de " << cwd << endl;
        showDirectories = true;
    }
    else if (argc == 1) {
        cout << "Fichiers de " << cwd << endl;
        showDirectories = false;
    }
    else {
        cout << "Arguments invalides." << endl;
        return 0;
    }

    auto dir_listing = cwd.list_directory();

    for (auto entry : dir_listing) {
        auto full_path = cwd;
        full_path.append(entry.d_name);

        if ((showDirectories
                and full_path.is_a(S_IFDIR)
                and strcmp(entry.d_name, ".")
                and strcmp(entry.d_name, ".."))

                or
                (!showDirectories and full_path.is_a(S_IFREG))) {

            std::cout << entry.d_name << std::endl;
        }
    }

    return 0;
}