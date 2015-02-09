#include <iostream>

class Shell {

public:

    Shell(FILE* in=stdin, FILE* out=stdout, FILE* err=stderr);

    int run();

private:
    FILE* in;
    FILE* out;
    FILE* err;

};