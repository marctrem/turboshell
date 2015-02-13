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

    long size = pathconf(".", _PC_PATH_MAX);
    char *path_buf = (char *) malloc((size_t) size);

    this->cwd = path(getcwd(path_buf, (size_t) size));

    this->findExecutablesInPath();


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
        // TODO: Implement better tokenizer function.
        const boost::char_separator<char> sep(" ");
        boost::tokenizer<boost::char_separator<char>> tok(line, sep);

        for(auto it = tok.begin(); it != tok.end(); it++) {
            tokens.push_back(*it);
            *this->out << *it << std::endl;
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
        *this->out << "Répertoire courrant : " << this->cwd;
    }
    else if ("cd" == tokens.front()) {
        // Todo: Throw exception if tokens[1] inexistant aka invalid commmand.

        path p(tokens[1]);

        this->changeWorkingDirectory(p);
    }
    else if ("./" == tokens.front()) {



    }
    else {
        // Try to find command
        try {
            path &exec_path = executablesInPath.at(tokens[0]);

            std::cout << exec_path << std::endl;

            char *cmd[tokens.size() + 1];

            int i = 0;
            for(std::string token : tokens) {
                cmd[i] = (char *) token.c_str();
                i++;
            }
            cmd[i] = NULL;

            std::string env_pwd = "PWD=";
            env_pwd.append(this->cwd);

            char *env[] = { (char *) env_pwd.c_str(), (char *) 0};

            std::cout << "Spawning in: " << env_pwd << std::endl;

            pid_t pid;
            pid = fork();

            if (pid == 0) {
                // Child
                execve(exec_path.c_str(), cmd, env);

            }
            else {
                int result = 0;
                wait(&result);
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
    auto bin_path = path("~/inf3172").make_absolute(this->cwd);
    std::cout << bin_path << std::endl;

    for(auto entry : bin_path.list_directory()) {
        path full_path = path(entry.d_name).make_absolute(bin_path);

        if (full_path.is_a(S_IEXEC) && !full_path.is_a(S_IFDIR)) { // Cache the struct stat.
            std::cout << full_path << std::endl;

            *this->out << "Binary found: '" << entry.d_name << "'" << std::endl;
            executablesInPath.insert(std::pair<std::string, path>(entry.d_name, full_path));
        }
    }

    std::cout << "Found " << executablesInPath.size() << " executables." << std::endl;
}


