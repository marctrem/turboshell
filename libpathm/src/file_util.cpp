
#include <iosfwd>
#include <fstream>
#include <string.h>
#include <iostream>
#include <mutex>
#include "../../libpathm/include/file_util.hpp"

std::mutex cout_m;

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
    int tempErrno = errno;
    if (tempErrno) {
        cout_m.lock();
        std::cout << strerror(tempErrno);
        cout_m.unlock();
        return 0;
    }
    long lc = line_count(file_stream);
    file_stream.close();
    return lc;
}
