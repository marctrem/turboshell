#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <list>
#include <sys/stat.h>
#include "../../libpathm/include/file_util.hpp"

using namespace std;

mutex paths_m, output_m;
list<string> paths;

void task() {
    bool has_work = true;
    do {
        paths_m.lock();
        if (paths.empty()) {
            paths_m.unlock();
            has_work = false;
        }
        else {
            string path = paths.back();
            paths.pop_back();
            paths_m.unlock();

            long count = line_count(path);
            if (count) {
                output_m.lock();
                cout << path << " contient " << count << " lignes" << std::endl;
                output_m.unlock();
            } else {
                output_m.lock();
                cout << " (" << path << ")" << std::endl;
                output_m.unlock();
            }
        }
    } while (has_work);
}

int main() {
    auto location = pathm::path::get_current_path();
    for (auto file : location.list_directory()) {
        pathm::path absolute_path = location;
        absolute_path.append(file.d_name);
        if (absolute_path.is_a(S_IFREG)) {
            paths.push_back(absolute_path);
        }
    }
    vector<thread> thread_pool(10);
    for (auto &t : thread_pool) {
        t = thread(task);
    }
    for (auto &t : thread_pool) {
        t.join();
    }
    cout.flush();
    return 0;
}