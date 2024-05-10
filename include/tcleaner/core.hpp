// Search also files with **

#pragma once
#ifndef TEMP_CLEANER_CORE_HPP
#define TEMP_CLEANER_CORE_HPP

#include <string>
#include <unordered_set>
#include <vector>

namespace tcleaner {
    class tcleaner {
        public:

            // Constructors
            explicit tcleaner(const std::string& path);

            // Methods
            void analyze_files();
            void ignore_path(const std::string& path);

        private:

            // Methods
            void _analyze_files(const std::filesystem::path& path);
            void _process_gitignore(const std::string& path);
            void _remove_file(std::string_view file);
            void _choose_if_remove(std::string_view);

            // Members
            std::string _path;
            std::vector<std::string> _paths_to_ignore;
            static std::string _parent_path;
    };
}  // namespace tcleaner

#endif