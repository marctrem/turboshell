

#ifndef SHELL_H_
#define SHELL_H_

#include <iostream>
#include <list>
#include <vector>
#include <boost/filesystem.hpp>
#include <map>


class Shell {

public:

    Shell(std::istream *in=&std::cin, std::ostream *out=&std::cout, FILE *err=stderr);

    int run();

private:
    std::istream *in;
    std::ostream *out;
    FILE* err;

    boost::filesystem::path cwd;

    void displayPrompt();
    int tokenizeInput(std::vector<std::string> &tokens);
    int processInput(std::vector<std::string> &tokens);
    int processBuiltin(std::vector<std::string> &tokens);

    void changeWorkingDirectory(boost::filesystem::path dest);
};

#endif