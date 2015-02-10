#include "Shell.hpp"

#include <boost/tokenizer.hpp>

const std::string PROMPT_MSG = "TSH> ";

Shell::Shell(std::istream *in, std::ostream *out, FILE *err) {
    this->in = in;
    this->out = out;
    this->err = err;
    this->cwd = boost::filesystem::current_path();
}

int Shell::run() {

    int lastStatus = 0;

    do {
        auto linetokens = std::vector<std::string>();
        this->displayPrompt();
        lastStatus &= this->tokenizeInput(linetokens);
        lastStatus &= this->processInput(linetokens);
    }
    while(!lastStatus);

    return 0;
}

void Shell::displayPrompt() {
    std::cout << "\n" << PROMPT_MSG;
}

int Shell::tokenizeInput(std::vector<std::string> &tokens) {
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


int Shell::processInput(std::vector<std::string> &tokens) {

    if (tokens.empty()) {
        return 0;
    }
    else if ("cdir" == tokens.front()) {
        *this->out << this->cwd.string();
    }
    

    return 0;
}

