
#include <iostream>
#include <list>
#include <string.h>
#include <sys/stat.h>
#include <utility>

#include "../../libpathm/include/path.hpp"

using namespace std;
using namespace pathm;

// <Size, Filecount>
std::pair<unsigned long, unsigned long>
get_directory_size(const path &p) {

    unsigned long size = 0;
    unsigned long file_count = 0;

    for(struct dirent entry : p.list_directory()) {

        // Let's ignore those two folders ".." and "."
        if (!strncmp(entry.d_name, "..", 3)
                or !strncmp(entry.d_name, ".", 2)) {
            continue;
        }

        // Build the full path to the entry
        path full_path = p;
        full_path.normalize();
        full_path.append(entry.d_name);

        // If folder: recur
        if (full_path.is_a(S_IFDIR)) {
            std::pair<unsigned long, unsigned long> result =
                    get_directory_size(full_path);
            size += result.first;
            file_count += result.second;
        }
        // If other: compute
        else {
            struct stat filestat;

            errno = 0;
            stat(full_path.c_str(), &filestat);
            int errval = errno;
            if (!errval) {
                file_count++;
                size += filestat.st_size;
            }
            else {
                std::cerr << strerror(errval) << ": "<< full_path << std::endl;
                //throw 30;
            }
        }
    }
    return std::pair<unsigned long, unsigned long>(size, file_count);
}

int main(int argc, char *argv[]) {

    path cwd = path::get_current_path();

    if (argc != 2) {
        cout << "Arguments invalides." << endl;
        return 0;
    }

    path full_path = path(argv[1]).make_absolute(cwd);


    if (!(full_path.is_a(S_IFDIR))) {
        std::cout << "Répertoire introuvable" << std::endl;
        return 0;
    }

    auto res = get_directory_size(full_path);

    std::cout << "Le répertoire "
            << full_path
            << " contient "
            << res.second
            << " fichiers pour un total de "
            << res.first
            << " octets."
            << std::endl;

    return 0;
}

