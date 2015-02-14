#include <iostream>
#include <string>
#include "Shell.hpp"

const std::string VERSION = "0.0.1";

using namespace std;

int main() {
    cout << "TSH v" << VERSION << endl;

    Shell tsh;
    tsh.run();

    return 0;
}