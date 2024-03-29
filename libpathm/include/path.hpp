
#pragma once

#include <string>
#include <list>
#include <dirent.h>

namespace pathm {

    class path : public std::string  {

    public:
        path() {};
        path(const char* val) : std::string(val) {};
        path(const std::string val) : std::string(val) {};

        static path get_current_path();

        bool is_absolute();
        bool is_a(unsigned int types) const;

        path& replace_home_symbol();
        path& make_absolute(const path &base);
        path& normalize();

        std::list<struct dirent> list_directory() const; // Todo: Consider implementing an iterator.

    };
}
