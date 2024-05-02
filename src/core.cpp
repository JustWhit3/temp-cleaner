#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <tcleaner/core.hpp>

namespace fs = std::filesystem;

namespace tcleaner {
    tcleaner::tcleaner(const std::string& path) : _path(path) {}

    void tcleaner::_analyze_files(const std::string& path) {
        for (const auto& entry: fs::directory_iterator(path)) {
            const auto& current_path = entry.path().string();

            bool skip = std::any_of(_paths_to_ignore.begin(), _paths_to_ignore.end(),
                                    [&current_path](const std::string& ignore_path) {
                                        return current_path.find(ignore_path) != std::string::npos;
                                    });

            if (!skip) {
                if (entry.is_directory()) {
                    this->_analyze_files(current_path);
                } else if (entry.is_regular_file() && entry.path().filename() == ".gitignore") {
                    this->_process_gitignore(current_path);
                }
            }
        }
    }

    void tcleaner::ignore_path(const std::string& path) { this->_paths_to_ignore.push_back(path); }

    void tcleaner::analyze_files() {
        if (!this->_paths_to_ignore.empty()) {
            std::cout << "\nThe following paths will be ignored:\n";
            for (const auto& entry: this->_paths_to_ignore) {
                std::cout << "- " << entry << "\n";
            }
            std::cout << "Do you want to proceed? (y/n): ";
            char proceed;
            std::cin >> proceed;
            if (proceed == 'y') {
                std::cout << "\n";
                tcleaner::_analyze_files(this->_path);
            } else {
                std::exit(0);
            }
        }
        tcleaner::_analyze_files(this->_path);
    }

    void tcleaner::_process_gitignore(const std::string& file) {
        std::cout << "Found .gitignore file: " << file << "\n";
        std::ifstream file_stream(file);
        std::string line;
        while (std::getline(file_stream, line)) {
            if (line.empty() || line[0] == '#' || line.find('*') != std::string::npos) continue;

            std::string single_file_path = file.substr(0, file.size() - 10) + line;
            if (fs::exists(single_file_path)) {
                std::cout << "- " << single_file_path << "\n";
            }
        }
        file_stream.close();
        std::cout << "\n";
    }
}  // namespace tcleaner