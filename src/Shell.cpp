#include "Shell.hpp"

#include <boost/tokenizer.hpp>

const std::string PROMPT_MSG = "TSH> ";

Shell::Shell(std::istream *in, FILE *out, FILE *err) {
    this->in = in;
    this->out = out;
    this->err = err;

    this->cwd = boost::filesystem::current_path();
}

int Shell::run() {

    int lastStatus = 0;

    do {
        auto linetokens = std::list<std::string>();

        this->displayPrompt();
        lastStatus &= this->tokenizeInput(linetokens);
        lastStatus &= this->processInput(linetokens);


    }
    while(!lastStatus);

    return 0;
}

void Shell::displayPrompt() {
    std::cout << "\n" << " " << PROMPT_MSG;
}

int Shell::tokenizeInput(std::list<std::string> &tokens) {
    /*
    Takes the input and puts it's token in the provided list.
     */
    std::string line;

    if(std::getline(*this->in, line)) {
        boost::tokenizer<boost::char_separator<char>> tok(line);

        for(auto it = tok.begin(); it != tok.end(); it++) {
            tokens.push_back(*it);
        }
        return 0;
    }
    else {
        // Input failed.
        return -1;
    }
}

int Shell::processBuiltin(std::list<std::string> &tokens) {
    // TODO: Process builtins.
}

int Shell::processInput(std::list<std::string> &tokens) {


    return 0;

}