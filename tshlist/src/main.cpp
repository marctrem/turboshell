#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;

namespace fs = boost::filesystem;


int main(int argc, char *argv[]) {

    fs::path cwd = boost::filesystem::current_path();

    bool showDirectories;

    // Todo: Make it tougher.
    if (argc > 1 && !strncmp(argv[1], "-d", 2)) {
        cout << "Sous-répertoires de " << cwd.string() << endl;
        showDirectories = true;
    }
    else {
        cout << "Fichiers de " << cwd.string() << endl;
        showDirectories = false;
    }

    for(auto it = fs::directory_iterator(cwd); it != fs::range_end(it); it++) {
        if (showDirectories && fs::is_directory(*it)) {
            cout << it->path().filename().string() << endl;
        }
        else if (!showDirectories && fs::is_regular_file(*it)) {
            cout << it->path().filename().string() << endl;
        }
    }
    return 0;
}