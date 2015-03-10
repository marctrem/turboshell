
#include "../include/path.hpp"
#include <deque>
#include <boost/tokenizer.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>


using namespace pathm;

bool path::is_absolute() {
    return this->at(0) == '/';
}

path &path::replace_home_symbol() {

    if (this->at(0) == '~') {
        this->replace(0, 1, getenv("HOME"));
    }

    return *this;
}

path &pathm::path::make_absolute(const pathm::path &base) {

    this->replace_home_symbol();

    if (this->at(0) != '/') {
        this->insert(0, "/");
        this->insert(0, base);
    }

    this->normalize();

    return *this;
}

path &path::normalize() {

    assert(this->at(0) == '/');

    const boost::char_separator<char> separa("/");
    boost::tokenizer<boost::char_separator<char>> tokenizer(*this, separa);

    std::deque<std::string> path_stack;

    // Put the path in a stack
    for(auto& it : tokenizer) {
        if (it == "..") {
            if (!path_stack.empty())
                path_stack.pop_back();
        }
        else if (it == ".") {
            continue;
        }
        else {
            path_stack.push_back(it);
        }
    }

    // Build back the string
    *this = "";

    while (!path_stack.empty()) {
        this->push_back('/');
        this->append(path_stack.front());
        path_stack.pop_front();
    }

    if (*this == "") { this->push_back('/'); };

    if (this->is_a(S_IFDIR) && this->back() != '/') {
        this->push_back('/');
    }
    return *this;
}

bool path::is_a(unsigned int types) const {
    struct stat path_stat;

    errno = 0;
    if (!stat(this->c_str(), &path_stat)) {
        // Success
        return (path_stat.st_mode & types) != 0;
    }
    else {
        // Failure
        //std::cout << "ERRNO: " << errno << " : " << *this << std::endl;
        return false;
    }

}

std::list<struct dirent> path::list_directory() const {

    if (this->is_a(S_IFDIR)) {
        std::list<struct dirent> content;

        errno = 0;
        auto* dirp = opendir(this->c_str());
        struct dirent *dp;
        while ((dp = readdir(dirp)) != NULL) {
            //std::cout << dp->d_name << std::endl;
            content.push_back(*dp);
        }
        closedir(dirp);
        return content;

    }
    else {
        std::cout << "Cannot list directory" << std::endl;
        throw 20;
    }
}

path path::get_current_path() {
    long size = pathconf(".", _PC_PATH_MAX);
    char *path_buf = (char *) malloc((size_t) size);

    return path(getcwd(path_buf, (size_t) size)).normalize();
}