
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
        if (it == ".." and this->size() != 0) {
            path_stack.pop_back();
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

    if (this->is_a(S_IFDIR)) {
        this->push_back('/');
    }
    return *this;
}

bool path::is_a(unsigned int types) {
    struct stat path_stat;
    if (!stat(this->c_str(), &path_stat)) {
        // Success
        return (path_stat.st_mode & types) != 0;
    }
    else {
        // Failure
        std::cout << "ERRNO: " << errno << " : " << *this << std::endl;
        return false;
    }

}

std::list<struct dirent> path::list_directory() {

    std::cout << "Listing '" << *this << "'" << std::endl;

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