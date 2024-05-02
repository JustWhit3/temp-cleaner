#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <tcleaner/core.hpp>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    std::string path;

    if (argc < 2 || std::string(argv[1]).empty()) {
        path = getenv("HOME");
    } else {
        path = argv[1];
    }

    if (!fs::exists(path) || !fs::is_directory(path)) {
        std::cerr << "ERROR: inserted path is not valid!\n";
        std::exit(1);
    }

    std::cout << "Searching temporary files by looking at .gitignore files of repositories located in " << path
              << "...\n";

    tcleaner::tcleaner temp_cleaner(path);

    if (argc > 2) {
        std::ifstream file(argv[2]);
        if (!file.is_open()) {
            std::cerr << "ERROR: cannot open " << argv[2] << " file!\n";
            std::exit(1);
        }

        std::string line;
        while (std::getline(file, line)) {
            temp_cleaner.ignore_path(line);
        }
        file.close();
    }

    temp_cleaner.analyze_files();
}