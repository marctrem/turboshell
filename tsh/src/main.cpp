#include <iostream>
#include <string>
#include "Shell.hpp"

const std::string VERSION = "1.0.0";

using namespace std;

int main() {
    cout << "TSH v" << VERSION << endl;

    Shell tsh;
    tsh.run();

    return 0;
}