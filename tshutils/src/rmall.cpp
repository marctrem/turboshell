
#include <iostream>
#include <list>
#include <string.h>
#include <sys/stat.h>
#include <utility>
#include <unistd.h>
#include <ftw.h>
#include <vector>

#include "../../libpathm/include/path.hpp"

using namespace std;
using namespace pathm;

static bool deletable = 0;

int nftw_check_deletable(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {

    if (tflag == FTW_D && access(fpath, W_OK & X_OK)) {
        int errval = errno;
        fprintf(stderr, "%s '%s': %o\n", strerror(errval), fpath, sb->st_mode);
        deletable = 0;
        errno = 0;
        return -1;
    }
    return 0; // continue with nftw
}

bool is_folder_deletable(const path &p) {

    int flags = FTW_DEPTH;
    // Is erasable unless told otherwise.
    deletable = 1;
    nftw(p.c_str(), nftw_check_deletable, 200, flags);

    int errval = errno;
    if(errval) {
        std::cerr << strerror(errval) << std::endl;
        return false;
    }

    return deletable;
}

void
unlink_folder(const path &p) {
    // Todo: Replace with nftw
    int errval;

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

        // If folder: recur and unlink
        if (full_path.is_a(S_IFDIR)) {
            unlink_folder(full_path);
        }
        // If other: unlink
        else {
            errno = 0;
            if (unlink(full_path.c_str())) {
                errval = errno;
                std::cout << "Erreur: " << strerror(errval) << std::endl;
                throw 30;
            }
        }
    }

    errno = 0;
    if (rmdir(p.c_str())) {
        errval = errno;
        std::cout << errval << " : " << strerror(errval) << std::endl;
        // Todo: Better throw.
        throw 50;
    }
}

int main(int argc, char *argv[]) {

    path cwd = path::get_current_path();

    if (argc == 1) {
        std::cout << "Arguments invalides." << std::endl;
        return 0;
    }

    std::vector<path> paths;

    for (int i = 1; i < argc; i++) {
        paths.push_back(path(argv[i]).make_absolute(cwd));
    }

    // Make sure they are all deletable
    bool deletable = true;

    for (path path : paths) {
        if (!is_folder_deletable(path)) {
            deletable = false;
            break;
        }
    }

    if (deletable) {
        for (path path : paths) {
            unlink_folder(path);
        }
        std::cout << "Répertoire supprimé" << std::endl;
    }


    return 0;
}

