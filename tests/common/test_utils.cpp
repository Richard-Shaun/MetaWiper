/**
 * @file test_utils.cpp
 * @brief Implementation of test utility functions
 */
#include "test_utils.h"
#include <iostream>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

namespace test_utils {

void init_console() {
#ifdef _WIN32
    // Set console output to UTF-8 encoding
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::cout << "Console initialized" << std::endl;
}

bool check_test_file(const std::string& file_path) {
    if (std::filesystem::exists(file_path)) {
        std::cout << "Test file exists: " << file_path << std::endl;
        return true;
    }

    std::cout << "Test file does not exist: " << file_path << std::endl;
    return false;
}

std::string create_test_copy(const std::string& original_path, const std::string& suffix) {
    std::filesystem::path path(original_path);
    std::filesystem::path copy_stem = path.stem().string() + suffix;
    std::filesystem::path copy_path = path.parent_path() / (copy_stem.string() + path.extension().string());

    std::string test_copy = copy_path.string();

    try {
        std::filesystem::copy_file(
            original_path,
            test_copy,
            std::filesystem::copy_options::overwrite_existing
        );
        std::cout << "Created test copy: " << test_copy << std::endl;
        return test_copy;
    } catch (const std::exception& e) {
        std::cerr << "Failed to create test copy: " << e.what() << std::endl;
        return "";
    }
}

void print_operation_result(const file_handler::operation_result& result) {
    std::cout << "Operation result: " << (result.success ? "Success" : "Failed") << std::endl;
    std::cout << "Message: " << result.message << std::endl;

    if (!result.warnings.empty()) {
        std::cout << "Warnings:" << std::endl;
        for (const auto& warning : result.warnings) {
            std::cout << "  " << warning << std::endl;
        }
    }

    std::cout << "Metadata count: " << result.metadata.size() << std::endl;
    if (!result.metadata.empty()) {
        std::cout << "Metadata contents:" << std::endl;
        for (const auto& [key, value] : result.metadata) {
            std::cout << "  " << key << ": " << value << std::endl;
        }
    }
}

} // namespace test_utils