#include "Shell.hpp"

#include <unistd.h>
#include <boost/tokenizer.hpp>
#include <sys/stat.h>
#include <sys/wait.h>
#include <exception>

using namespace pathm;

const std::string PROMPT_MSG = "tsh> ";

Shell::Shell(std::istream *in, std::ostream *out, FILE *err) {
    this->in = in;
    this->out = out;
    this->err = err;

    this->cwd = path::get_current_path();

    this->findExecutablesInPath();


}

int Shell::run() {

    int lastStatus = 0;

    do {
        auto linetokens = std::vector<std::string>();
        std::string input;
        std::string output;
        this->displayPrompt();
        lastStatus |= this->tokenizeInput(linetokens, input, output);
        lastStatus |= this->processInput(linetokens);

    }
    while(!lastStatus);

    return 0;
}

void Shell::displayPrompt() {
    std::cout << "\n" << PROMPT_MSG;
}

int Shell::tokenizeInput(std::vector<std::string> &tokens, std::string &input, std::string &output) {
    /*
    Takes the input and puts it's token in the provided list.
     */
    std::string line;

    if(std::getline(*this->in, line)) {
        // TODO: Implement better tokenizer function.
        const boost::char_separator<char> sep(" ");
        const boost::char_separator<char> sep_in("<");
        const boost::char_separator<char> sep_out(">");

        // Get the output
        boost::tokenizer<boost::char_separator<char>> tok_out(line, sep_out);
        std::vector<std::string> vec_out(tok_out.begin(), tok_out.end());
        if (vec_out.size() > 2) {
            std::cerr << "error while redirecting output" << std::endl;
            return 0;
        }
        else if (vec_out.size() == 2) {
            output = vec_out.back();
        }


        // Get the input
        boost::tokenizer<boost::char_separator<char>> tok_in(vec_out.at(0), sep_in);
        std::vector<std::string> vec_in(tok_in.begin(), tok_in.end());
        if (vec_in.size() > 2) {std::cerr << "error while redirecting input" << std::endl; }

        // Get the command
        boost::tokenizer<boost::char_separator<char>> tok(vec_in.at(0), sep);

        tokens.assign(tok.begin(), tok.end());

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

        if (tokens.size() != 1) {
            *this->out << ARGUMENT_ERROR << std::endl;
            return 0;
        }
        *this->out << "Répertoire courrant : " << this->cwd;
    }
    else if ("cd" == tokens.front()) {
        
        if (tokens.size() != 2) {
            *this->out << ARGUMENT_ERROR << std::endl;
            return 0;
        }

        path p(tokens[1]);

        this->changeWorkingDirectory(p);
    }
    else if ("exit" == tokens.front()) {
        return -1;
    }
    else {
        // Try to find command
        try {
            path &exec_path = executablesInPath.at(tokens[0]);

            char *cmd[tokens.size() + 1];

            int i = 0;
            for(std::string token : tokens) {
                cmd[i] = (char *) token.c_str();
                i++;
            }
            cmd[i] = NULL;

            pid_t pid;
            pid = fork();

            if (pid == 0) {
                // Child
                chdir(this->cwd.c_str());
                execv(exec_path.c_str(), cmd);
            }
            else {
                // Parent
                int result = 0;
                wait(&result);
                // Todo: Do something on weird return... maybe say that command failed?
            }

            return 0;
        }
        catch (std::exception &e) {
            std::cout << "Programme '" << tokens[0] << "' introuvable." << std::endl;
        }
    }

    return 0;
}

void Shell::changeWorkingDirectory(pathm::path &dest) {

    dest.replace_home_symbol();

    path nextPlausiblePath = dest;

    nextPlausiblePath.make_absolute(this->cwd);

    if (nextPlausiblePath.is_a(S_IFDIR)) {
        this->cwd = nextPlausiblePath;
    }
    else {
        *this->out << "Répertoire introuvable: " << nextPlausiblePath << "'" << std::endl;
        // Todo: Throw an exception to show that the desired path is not valid... Or maybe do something better like telling if it's a file or if it's just not existing.
    }
}



void Shell::findExecutablesInPath() {

    // Todo: Extract path
    auto bin_path = path(REL_BIN_PATH).make_absolute(this->cwd);

    for(auto entry : bin_path.list_directory()) {
        path full_path = path(entry.d_name).make_absolute(bin_path);

        if (full_path.is_a(S_IEXEC) && !full_path.is_a(S_IFDIR)) { // Cache the struct stat.
            executablesInPath.insert(std::pair<std::string, path>(entry.d_name, full_path));
        }
    }
}


