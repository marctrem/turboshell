#include "Shell.hpp"

#include <unistd.h>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <sys/stat.h>
#include <sys/wait.h>
#include <exception>

#include <typeinfo>



namespace io = boost::iostreams;

const std::string PROMPT_MSG = "tsh> ";

Shell::Shell(std::istream *in, std::ostream *out, FILE *err) {

    this->cwd = pathm::path::get_current_path();

    this->findExecutablesInPath();


}

int Shell::run() {

    int lastStatus = 0;

    do {

        int savedErrno;

        auto linetokens = std::vector<std::string>();
        std::string input;
        std::string output;


        this->displayPrompt();
        lastStatus |= this->tokenizeInput(linetokens, input, output);
        std::cout << "Input: " << input << std::endl;
        std::cout << "Output " << output << std::endl;

        // Prepare files


        int previousIN = dup(STDIN_FILENO);
        bool inModified = !input.empty(); // Better be explicit

        int previousOUT = dup(STDOUT_FILENO);
        bool outModified = !output.empty();


        try {

            // Todo: Verify if input redirection is working
            if (inModified) {
                close(0);
                fopen(input.c_str(), "r");
                savedErrno = errno;

                if (savedErrno) {
                    std::cerr << "Input redirection: " << std::strerror(savedErrno) << std::endl;
                    throw std::exception();
                }
            }


            if (outModified) {
                close(1);
                FILE *outptr = fopen(output.c_str(), "w");
                savedErrno = errno;

                std::cerr << "Out descriptor: " << fileno(outptr) << std::endl;

                if (savedErrno) {
                    std::cerr << "Output redirection " << std::strerror(savedErrno) << std::endl;
                    throw std::exception();
                }
            }

            // Process the command
            lastStatus |= this->processInput(linetokens);

        } catch (std::exception e) {}

        if (inModified) {
            dup2(previousIN, STDIN_FILENO);
            close(previousIN);
        }

        if (outModified) {
            dup2(previousOUT, STDOUT_FILENO);
            close(previousOUT);
        }
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

    if(std::getline(std::cin, line)) {
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
            boost::algorithm::trim(output);
        }


        // Get the input
        boost::tokenizer<boost::char_separator<char>> tok_in(vec_out.at(0), sep_in);
        std::vector<std::string> vec_in(tok_in.begin(), tok_in.end());
        if (vec_in.size() > 2) {
            std::cerr << "error while redirecting input" << std::endl; 
            return 0;
        }
        else if (vec_in.size() == 2) {
            input = vec_in.back();
            boost::algorithm::trim(input);
        }

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
            std::cerr << ARGUMENT_ERROR << std::endl;
            return 0;
        }
        printf("Répertoire courrant : %s\n", this->cwd.c_str());
    }
    else if ("cd" == tokens.front()) {
        
        if (tokens.size() != 2) {
            std::cerr << ARGUMENT_ERROR << std::endl;
            return 0;
        }

        pathm::path p(tokens[1]);

        this->changeWorkingDirectory(p);
    }
    else if ("exit" == tokens.front()) {
        return -1;
    }
    else {
        // Try to find command
        try {
            pathm::path &exec_path = executablesInPath.at(tokens[0]);

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

                char * const * env = 0;

                execve(exec_path.c_str(), cmd, env);
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
            std::cerr << "Programme '" << tokens[0] << "' introuvable." << std::endl;
        }
    }

    return 0;
}

void Shell::changeWorkingDirectory(pathm::path &dest) {

    dest.replace_home_symbol();

    pathm::path nextPlausiblePath = dest;

    nextPlausiblePath.make_absolute(this->cwd);

    if (nextPlausiblePath.is_a(S_IFDIR)) {
        this->cwd = nextPlausiblePath;
    }
    else {
        std::cerr << "Répertoire introuvable: " << nextPlausiblePath << "'" << std::endl;
        // Todo: Throw an exception to show that the desired path is not valid... Or maybe do something better like telling if it's a file or if it's just not existing.
    }
}



void Shell::findExecutablesInPath() {

    // Todo: Extract path
    auto bin_path = pathm::path(REL_BIN_PATH).make_absolute(this->cwd);

    for(auto entry : bin_path.list_directory()) {
        pathm::path full_path = pathm::path(entry.d_name).make_absolute(bin_path);

        if (full_path.is_a(S_IEXEC) && !full_path.is_a(S_IFDIR)) { // Cache the struct stat.
            executablesInPath.insert(std::pair<std::string, pathm::path>(entry.d_name, full_path));
        }
    }
}


