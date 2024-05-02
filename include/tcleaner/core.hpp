#pragma once
#ifndef TEMP_CLEANER_CORE_HPP
#define TEMP_CLEANER_CORE_HPP

#include <string>
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
            void _analyze_files(const std::string& path);
            void _process_gitignore(const std::string& path);

            // Members
            std::string _path;
            std::vector<std::string> _paths_to_ignore;
    };
}  // namespace tcleaner

#endif