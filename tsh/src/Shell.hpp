

#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <map>
#include "../../libpathm/include/path.hpp"


class Shell {

public:

    Shell(std::istream *in=&std::cin, std::ostream *out=&std::cout, FILE *err=stderr);

    int run();

private:
    std::istream *in;
    std::ostream *out;
    FILE* err;

    std::map<std::string, pathm::path> executablesInPath;

    pathm::path cwd;

    void displayPrompt();
    int tokenizeInput(std::vector<std::string> &tokens, std::string&, std::string&);
    int processInput(std::vector<std::string> &tokens);
    void findExecutablesInPath();

    void changeWorkingDirectory(pathm::path &dest);
};

static char const *const REL_BIN_PATH = "~/inf3172/bin";
static char const *const ARGUMENT_ERROR = "Arguments invalide.";