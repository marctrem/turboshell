
#include <iosfwd>
#include <fstream>
#include "../../libpathm/include/file_util.hpp"

long line_count(std::ifstream &fileToRead){
    int lc = 1;
    char buf;

    if (!fileToRead.is_open())
        return -1;

    while (fileToRead.get(buf)) {
        if (buf == '\n')
            ++lc;
    }
    fileToRead.clear();
    fileToRead.seekg(0, fileToRead.beg);
    return lc;
}

long line_count(pathm::path fileToRead) {
    std::ifstream file_stream(fileToRead);
    long lc = line_count(file_stream);
    file_stream.close();
    return lc;
}
