#include <iostream>
#include <string.h>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <bits/stream_iterator.h>

char getch(void) {
    struct termios oldTerm, newTerm;
    char retChar;
    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    newTerm.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
    retChar = (char) getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
    return retChar;
}

void getText(int &outputFileD) {
    char c;
    while (1) {
        c = getch();
        write(STDOUT_FILENO, &c, 1);
        if (c == EOF || c == 27) break;
        write(outputFileD, &c, 1);
    }
}

void writeText(int &inputFileD, std::fstream &file) {
    bool endOfSentence = true, space = true;
    char buff;
    while (read(inputFileD, &buff, 1) > 0) {
        if (buff == '.' || buff == '!' || buff == '?') {
            endOfSentence = true;
            space = false;
        } else if (isspace(buff)) {
            space = true;
        } else if (buff == 127){
            //TODO: Aller chercher l'etat precedent.
        } else {
            if (endOfSentence && !space) {
                file << ' ';
            }
            buff = endOfSentence ? (char) toupper(buff) : buff;
            endOfSentence = false;
            space = false;
        }
        file << buff;
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "Nombre d'arguments invalides." << std::endl;
        return 1;
    }

    std::fstream file;
    file.rdbuf()->pubsetbuf(0, 0);
    file.open(argv[1], std::ifstream::out);
    int tempErrno = errno;
    if (tempErrno) {
        std::cout << strerror(tempErrno) << std::endl;
        return 2;
    }
    int threadPipe[2];

    if (pipe(threadPipe) == -1) {
        std::cout << "Erreur a la creation du pipe." << std::endl;
        return 3;
    }

    std::thread load(getText, std::ref(threadPipe[1]));
    std::thread save(writeText, std::ref(threadPipe[0]), std::ref(file));

    load.join();
    close(threadPipe[1]);

    save.join();
    close(threadPipe[0]);

    file.close();
    return 0;
}