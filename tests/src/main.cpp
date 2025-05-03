/**
 * @file main.cpp
 * @brief MetaWiper core library test program
 * @details Tests basic functionality of the metadata removal library
 */

#include <iostream>
#include <string>
#include <filesystem>
#include <thread>
#include <chrono>
#include <meta_wiper_core.h>

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @brief Initializes console for proper character display
 */
void init_console() {
#ifdef _WIN32
    // Set console output to UTF-8 encoding
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "Console set to UTF-8 encoding" << std::endl;
#endif
}

/**
 * @brief Tests supported file types
 *
 * @param core Meta wiper core instance
 */
void test_supported_file_types(meta_wiper_core::meta_wiper_core_class& core) {
    std::cout << "=== Testing Supported File Types ===" << std::endl;

    auto file_types = core.get_supported_file_types();
    std::cout << "Number of supported file types: " << file_types.size() << std::endl;

    std::cout << "Supported file types: ";
    for (const auto& type : file_types) {
        std::cout << type << " ";
    }
    std::cout << std::endl;

    // Test if specific file types are supported
    std::cout << "PDF support: " << (core.type_supported("pdf") ? "Supported" : "Not supported") << std::endl;
    std::cout << "JPEG support: " << (core.type_supported("jpg") ? "Supported" : "Not supported") << std::endl;
    std::cout << std::endl;
}

/**
 * @brief Check if file exists
 *
 * @param file_path File path
 * @return True if file exists
 */
bool check_test_file(const std::string& file_path) {
    if (std::filesystem::exists(file_path)) {
        std::cout << "Test file exists: " << file_path << std::endl;
        return true;
    }

    std::cout << "Test file does not exist. Please provide a valid PDF file path for testing." << std::endl;
    return false;
}

/**
 * @brief Test reading file metadata
 *
 * @param core Meta wiper core instance
 * @param file_path PDF file path
 */
void test_read_metadata(meta_wiper_core::meta_wiper_core_class& core, const std::string& file_path) {
    if (!check_test_file(file_path)) {
        return;
    }

    std::cout << "=== Testing Read Metadata ===" << std::endl;
    std::cout << "File path: " << file_path << std::endl;

    auto result = core.process_file(file_path, file_handler::operation_type::READ);

    std::cout << "Operation result: " << (result.success ? "Success" : "Failed") << std::endl;
    std::cout << "Message: " << result.message << std::endl;

    if (result.success) {
        std::cout << "Metadata count: " << result.metadata.size() << std::endl;
        std::cout << "Metadata content:" << std::endl;
        for (const auto& [key, value] : result.metadata) {
            std::cout << "  " << key << ": " << value << std::endl;
        }
    } else if (!result.warnings.empty()) {
        std::cout << "Warnings:" << std::endl;
        for (const auto& warning : result.warnings) {
            std::cout << "  " << warning << std::endl;
        }
    }

    std::cout << std::endl;
}

/**
 * @brief Test cleaning file metadata using a file copy
 *
 * @param core Meta wiper core instance
 * @param file_path PDF file path
 */
void test_clean_metadata(meta_wiper_core::meta_wiper_core_class& core, const std::string& file_path) {
    if (!check_test_file(file_path)) {
        return;
    }

    std::cout << "=== Testing Clean Metadata ===" << std::endl;

    // Create a test copy with proper extension
    std::filesystem::path original_path(file_path);
    std::filesystem::path copy_stem = original_path.stem().string() + "_copy";
    std::filesystem::path copy_path = original_path.parent_path() / (copy_stem.string() + original_path.extension().string());

    std::string test_copy = copy_path.string();

    try {
        std::filesystem::copy_file(
            file_path,
            test_copy,
            std::filesystem::copy_options::overwrite_existing
        );
        std::cout << "Created test copy: " << test_copy << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to create test copy: " << e.what() << std::endl;
        return;
    }

    // First, read the original metadata
    std::cout << "Reading original metadata..." << std::endl;
    auto read_result = core.process_file(test_copy, file_handler::operation_type::READ);

    if (!read_result.success) {
        std::cout << "Failed to read original metadata: " << read_result.message << std::endl;
        // Clean up test copy
        std::filesystem::remove(test_copy);
        return;
    }

    size_t original_metadata_count = read_result.metadata.size();
    std::cout << "Original metadata count: " << original_metadata_count << std::endl;

    // Now, clean the metadata
    std::cout << "Cleaning metadata..." << std::endl;
    auto clean_result = core.process_file(test_copy, file_handler::operation_type::CLEAN);

    std::cout << "Clean operation result: " << (clean_result.success ? "Success" : "Failed") << std::endl;
    std::cout << "Message: " << clean_result.message << std::endl;

    if (!clean_result.success) {
        std::cout << "Cleaning failed, cleaning up test copy..." << std::endl;
        std::filesystem::remove(test_copy);
        return;
    }

    // Add a short delay to ensure file operations are complete
    std::cout << "Waiting for file operations to complete..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Read the metadata again to verify it was cleaned
    std::cout << "Reading metadata after cleaning..." << std::endl;
    auto read_after_clean = core.process_file(test_copy, file_handler::operation_type::READ);

    if (!read_after_clean.success) {
        std::cout << "Failed to read metadata after cleaning: " << read_after_clean.message << std::endl;
        std::filesystem::remove(test_copy);
        return;
    }

    size_t cleaned_metadata_count = read_after_clean.metadata.size();
    std::cout << "Metadata count after cleaning: " << cleaned_metadata_count << std::endl;

    if (cleaned_metadata_count < original_metadata_count) {
        std::cout << "Metadata was successfully reduced/removed!" << std::endl;
    } else {
        std::cout << "Warning: Metadata count did not decrease after cleaning" << std::endl;
    }

    // Display any remaining metadata
    if (!read_after_clean.metadata.empty()) {
        std::cout << "Remaining metadata:" << std::endl;
        for (const auto& [key, value] : read_after_clean.metadata) {
            std::cout << "  " << key << ": " << value << std::endl;
        }
    }

    // Clean up test copy
    try {
        std::filesystem::remove(test_copy);
        std::cout << "Deleted test copy" << std::endl;
    } catch (...) {
        std::cerr << "Failed to delete test copy" << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Main function
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit code
 */
int main(int argc, char* argv[]) {
    // Initialize console for proper character display
    init_console();

    std::cout << "MetaWiper Core Library Test Program" << std::endl;
    std::cout << "=============================" << std::endl;

    // Create core library instance
    meta_wiper_core::meta_wiper_core_class core;

    // Test supported file types
    test_supported_file_types(core);

    // Determine test file path
    std::string test_file_path;
    if (argc > 1) {
        // Use file path provided by command line argument
        test_file_path = argv[1];
    } else {
        // Ask user for file path
        std::cout << "Enter PDF file path for testing (or press Enter to skip file testing): ";
        std::getline(std::cin, test_file_path);

        if (test_file_path.empty()) {
            std::cout << "No file path provided, skipping file testing." << std::endl;
            return 0;
        }
    }

    // Simplify test file path
    try {
        test_file_path = std::filesystem::absolute(test_file_path).string();
        std::cout << "Using file path: " << test_file_path << std::endl;
    } catch (...) {
        // If unable to get absolute path, continue with original path
    }

    // Test reading metadata
    test_read_metadata(core, test_file_path);

    // Test cleaning metadata
    test_clean_metadata(core, test_file_path);

    std::cout << "Testing completed!" << std::endl;
    return 0;
}