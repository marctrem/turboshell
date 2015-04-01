
#pragma once

#include <string>
#include <list>
#include <dirent.h>
#include <fstream>
#include "path.hpp"

long line_count(std::ifstream &fileToRead);
long line_count(pathm::path fileToRead);
