#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <tcleaner/core.hpp>
#include <unordered_set>

namespace fs = std::filesystem;

namespace tcleaner {

    // ********************* PUBLIC *********************

    /**
     * @brief Constructor of the tcleaner class.
     *
     * @param path The path through which recursively every subfolder is analyzed.
     */
    tcleaner::tcleaner(const std::string& path) : _path(path) {}

    /**
     * @brief Adds a path to the list of paths to be ignored during file analysis.
     *
     * @param path The path to add to the list of ignored paths.
     */
    void tcleaner::ignore_path(const std::string& path) { this->_paths_to_ignore.push_back(path); }

    /**
     * @brief Initiates the analysis of files in the specified path, ignoring specified paths if present.
     *
     * @note If the user chooses not to proceed, the program exits.
     */
    void tcleaner::analyze_files() {
        if (!this->_paths_to_ignore.empty()) {
            std::cout << "The following paths will be ignored:\n";
            for (const auto& entry: this->_paths_to_ignore) {
                std::cout << "- \033[1;36m" << entry << "\033[0m\n";
            }
            std::cout << "Do you want to proceed? (y/n): ";
            char proceed;
            std::cin >> proceed;
            if (proceed != 'y') {
                std::exit(0);
            }
        }
        std::cout << "\n";
        this->_analyze_files(this->_path);
    }

    // ********************* PRIVATE *********************

    /**
     * @brief Recursively analyzes files within a directory, handling paths to be ignored and processing .gitignore
     * files.
     *
     * @param path The path to analyze.
     */
    void tcleaner::_analyze_files(const std::filesystem::path& path) {
        static const std::string gitignore_extension = ".gitignore";
        static const std::unordered_set<std::string> paths_to_ignore_set(_paths_to_ignore.begin(),
                                                                         _paths_to_ignore.end());

        for (const auto& entry: fs::directory_iterator(path)) {
            const auto& entry_path = entry.path();

            if (paths_to_ignore_set.find(entry_path.string()) != paths_to_ignore_set.end()) {
                continue;
            }

            if (entry.is_directory()) {
                _analyze_files(entry_path);
            } else if (entry.is_regular_file() && entry_path.filename() == gitignore_extension) {
                _process_gitignore(entry_path.string());
            }
        }
    }

    /**
     * @brief Removes a file.
     *
     * @param file The path to the file to remove.
     */
    void tcleaner::_remove_file(std::string_view file) {
        try {
            if (fs::is_regular_file(file)) {
                fs::remove(file);
            } else if (fs::is_directory(file)) {
                fs::remove_all(file);
            }
            std::cout << "REMOVED.\n";
        } catch (const std::exception& e) {
            std::cerr << "\033[1;31mERROR\033[0m: Cannot remove file " << file << ": " << e.what() << "\n";
        }
    }

    /**
     * @brief Prompts the user to remove a file or directory based on the given file path.
     *
     * @param file The path to the file or directory.
     */
    void tcleaner::_choose_if_remove(std::string_view file) {
        while (true) {
            if (fs::is_regular_file(file))
                std::cout << "- Remove \033[1;32m" << file << "\033[0m file? (y/n): ";
            else if (fs::is_directory(file))
                std::cout << "- Remove \033[1;32m" << file << "\033[0m directory? (y/n): ";

            char proceed;
            std::cin >> proceed;

            if (proceed == 'y') {
                this->_remove_file(file);
                break;
            } else if (proceed == 'n') {
                break;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    /**
     * @brief Processes a .gitignore file, prompting the user to remove corresponding files.
     *
     * @param file The path to the .gitignore file.
     */
    void tcleaner::_process_gitignore(const std::string& file) {
        std::cout << "Found .gitignore file: " << file << "\n";
        std::ifstream file_stream(file);
        if (!file_stream.is_open()) {
            std::cerr << "\033[1;31mERROR\033[0m opening file: " << file << "\n";
            return;
        }

        std::unordered_set<std::string> files_to_remove;
        std::string parent_path = fs::path(file).parent_path().string();

        std::string line;
        while (std::getline(file_stream, line)) {
            try {
                // Cases to skip
                if (line.empty() || line[0] == '#' || line.find('!') != std::string::npos) continue;

                // Minor corrections
                if (line.back() == '/') line.pop_back();

                // Case for *
                if (line.find('*') != std::string::npos) {
                    std::string regex_pattern = std::regex_replace(line, std::regex("\\*"), ".*");
                    for (const auto& entry: fs::recursive_directory_iterator(parent_path)) {
                        std::string entry_path = entry.path().string();
                        if (std::regex_match(entry_path, std::regex(regex_pattern)) &&
                            entry_path.find("/.git/") == std::string::npos) {
                            files_to_remove.insert(entry_path);
                        }
                    }

                // Case for ** path
                } else if (line.find("**/") != std::string::npos) {
                    std::string dir_pattern = line.substr(0, line.find("**/"));
                    for (const auto& entry: fs::recursive_directory_iterator(parent_path)) {
                        std::string entry_path = entry.path().string();
                        if (entry_path.find(dir_pattern) != std::string::npos &&
                            entry_path.find("/.git/") == std::string::npos) {
                            files_to_remove.insert(entry_path);
                        }
                    }

                // Other cases
                } else {
                    std::string single_file_path = parent_path + "/" + line;
                    if (fs::exists(single_file_path)) {
                        files_to_remove.insert(single_file_path);
                    }
                }
            } catch (const fs::filesystem_error& e) {
                continue;
            }
        }

        file_stream.close();

        if (files_to_remove.empty()) {
            std::cout << "No regular files or directories found to remove. Skipping!\n";
        } else {
            for (const auto& file_path: files_to_remove) {
                this->_choose_if_remove(file_path);
            }
        }

        std::cout << "\n";
    }
}  // namespace tcleaner