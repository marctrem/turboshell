#include <iostream>
#include "../include/fs_entry.hpp"

using namespace pathm;

struct stat &pathm::fs_entry::get_stat() {

    if (!this->stat_is_loaded) {
        errno = 0;
        int result = stat(this->path.c_str(), &this->sstat);

        if (errno != 0 != result) {
            // Todo: throw better exception
            throw 10;
        }
    }

    return this->sstat;

}

std::list<fs_entry> &fs_entry::list_directory(std::list<fs_entry> &content) {

    //std::cout << "Listing '" << *this->path << "'" << std::endl;

    if (this->path.is_a(S_IFDIR)) {;

        errno = 0;
        auto* dirp = opendir(this->path.c_str());

        struct dirent *dp;
        while ((dp = readdir(dirp)) != NULL) {
            //std::cout << dp->d_name << std::endl;

            pathm::path new_path = this->path;
            new_path.append(dp->d_name);

            content.push_back(fs_entry(new_path));
        }
        closedir(dirp);
        return content;

    }
    else {
        std::cout << "Cannot list directory" << std::endl;
        throw 20;
    }
}

std::list<fs_entry> fs_entry::list_directory() {
    std::list<fs_entry> content;
    return list_directory(content);
}