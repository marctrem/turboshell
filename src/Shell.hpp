#include <iostream>
#include <list>
#include <boost/filesystem.hpp>

class Shell {

public:

    Shell(std::istream *in=&std::cin, FILE *out=stdout, FILE *err=stderr);

    int run();

private:
    std::istream *in;
    FILE* out;
    FILE* err;

    boost::filesystem::path cwd;


    void displayPrompt();
    int tokenizeInput(std::list<std::string> &tokens);
    int processInput(std::list<std::string> &tokens);
    int processBuiltin(std::list<std::string> &tokens);
};