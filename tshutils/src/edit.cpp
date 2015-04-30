#include <iostream>
#include <string.h>
#include <fstream>
#include <thread>
#include <list>
#include <unistd.h>
#include <termios.h>
#include <bits/stream_iterator.h>
#include <bits/stl_list.h>
#include <bits/stl_bvector.h>

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
        if (c == EOF || c == 27) break;
        write(outputFileD, &c, 1);
    }
}

void writeText(int &inputFileD, std::fstream &file) {
    std::list<std::pair<bool, bool>> history;
    history.push_front(std::pair<bool, bool>(true, true));
    bool oldEndOfSentence, oldSpace, newEndOfSentence, newSpace;
    char buff, sp = ' ';
    while (read(inputFileD, &buff, 1) > 0) {
        oldEndOfSentence = history.front().first;
        oldSpace = history.front().second;
        if (buff == '.' || buff == '!' || buff == '?') {
            newEndOfSentence = true;
            newSpace = false;
        } else if (isspace(buff)) {
            newEndOfSentence = oldEndOfSentence;
            newSpace = true;
        } else if (buff == 127){
            write(STDOUT_FILENO, &buff, 1);
            history.pop_front();
            long cursor = file.tellp();
            file.seekp(cursor-1);
            continue;
        } else {
            if (oldEndOfSentence && !oldSpace) {
                newEndOfSentence = oldEndOfSentence;
                newSpace = true;
                history.push_front(std::pair<bool, bool>(newEndOfSentence, newSpace));
                write(STDOUT_FILENO, &sp, 1);
                file << ' ';
            }
            buff = oldEndOfSentence ? (char) toupper(buff) : buff;
            newEndOfSentence = false;
            newSpace = false;
        }
        history.push_front(std::pair<bool, bool>(newEndOfSentence, newSpace));
        write(STDOUT_FILENO, &buff, 1);
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
    std::cout << std::endl;
    return 0;
}