#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <tcleaner/core.hpp>

namespace fs = std::filesystem;

namespace tcleaner {

    // ********************* CONSTRUCTORS *********************

    /**
     * @brief Constructor of the tcleaner class.
     *
     * @param path The path through which recursively every subfolder is analyzed.
     */
    tcleaner::tcleaner(const std::string& path) : _path(path) {}

    // ********************* PRIVATE *********************

    /**
     * @brief Recursively analyzes files within a directory, handling paths to be ignored and processing .gitignore
     * files.
     *
     * @param path The path to analyze.
     */
    void tcleaner::_analyze_files(std::string_view path) {
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

    /**
     * @brief Processes a .gitignore file, prompting the user to remove corresponding files.
     *
     * @param file The path to the .gitignore file.
     */
    void tcleaner::_process_gitignore(const std::string& file) {
        std::cout << "Found .gitignore file: " << file << "\n";
        std::ifstream file_stream(file);
        std::string line;
        static std::int32_t paths_to_remove;
        while (std::getline(file_stream, line)) {
            if (line.empty() || line[0] == '#' || line.find('*') != std::string::npos) continue;

            std::string single_file_path = file.substr(0, file.size() - 10) + line;
            if (fs::exists(single_file_path)) {
                paths_to_remove += 1;
                std::cout << "- Remove " << single_file_path << " file? (y/n): ";
                char proceed;
                std::cin >> proceed;
                if (proceed == 'y') {
                    this->_remove_file(single_file_path);
                }
            }
        }
        if (paths_to_remove == 0) {
            std::cout << "No regular files or directories found to remove. Skipping!\n";
        }
        paths_to_remove = 0;
        file_stream.close();
        std::cout << "\n";
    }

    /**
     * @brief Removes a file.
     *
     * @param file The path to the file to remove.
     */
    void tcleaner::_remove_file(std::string_view file) {
        try {
            fs::remove_all(file);
            std::cout << "REMOVED.\n";
        } catch (const fs::filesystem_error& e) {
            std::cerr << "ERROR: Cannot remove file " << file << ": " << e.what() << "\n";
        }
    }

    // ********************* PUBLIC *********************

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
                std::cout << "- " << entry << "\n";
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
}  // namespace tcleaner