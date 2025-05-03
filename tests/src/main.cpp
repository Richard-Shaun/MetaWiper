#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <meta_wiper_core.h>

/**
 * @brief Test supported file types
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
 * @brief Create a backup of the test file
 * @param original_path Original file path
 * @return Path to the backup file, empty if failed
 */
std::string create_test_file_backup(const std::string& original_path) {
    try {
        std::string backup_path = original_path + ".backup";
        std::filesystem::copy_file(
            original_path,
            backup_path,
            std::filesystem::copy_options::overwrite_existing
        );
        std::cout << "Created backup at: " << backup_path << std::endl;
        return backup_path;
    } catch (const std::exception& e) {
        std::cerr << "Failed to create backup: " << e.what() << std::endl;
        return "";
    }
}

/**
 * @brief Restore the original file from backup
 * @param original_path Original file path
 * @param backup_path Backup file path
 * @return True if restore succeeded
 */
bool restore_from_backup(const std::string& original_path, const std::string& backup_path) {
    try {
        if (std::filesystem::exists(backup_path)) {
            std::filesystem::copy_file(
                backup_path,
                original_path,
                std::filesystem::copy_options::overwrite_existing
            );
            std::cout << "Restored original file from backup" << std::endl;
            // Optionally remove the backup
            std::filesystem::remove(backup_path);
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Failed to restore from backup: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Check if file exists, create simple test file if necessary
 * @param file_path File path
 * @return True if file exists or was created successfully
 */
bool check_or_create_test_file(const std::string& file_path) {
    if (std::filesystem::exists(file_path)) {
        std::cout << "Test file exists: " << file_path << std::endl;
        return true;
    }

    std::cout << "Test file does not exist. Please provide a valid PDF file path for testing." << std::endl;
    return false;
}

/**
 * @brief Test reading file metadata
 * @param core Meta wiper core instance
 * @param file_path PDF file path
 */
void test_read_metadata(meta_wiper_core::meta_wiper_core_class& core, const std::string& file_path) {
    if (!check_or_create_test_file(file_path)) {
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
    }

    std::cout << std::endl;
}

/**
 * @brief Test cleaning file metadata
 * @param core Meta wiper core instance
 * @param file_path PDF file path
 */
void test_clean_metadata(meta_wiper_core::meta_wiper_core_class& core, const std::string& file_path) {
    if (!check_or_create_test_file(file_path)) {
        return;
    }

    std::cout << "=== Testing Clean Metadata ===" << std::endl;
    std::cout << "File path: " << file_path << std::endl;

    // Create a backup of the original file
    std::string backup_path = create_test_file_backup(file_path);
    if (backup_path.empty()) {
        std::cout << "Skipping clean test due to backup failure" << std::endl;
        return;
    }

    // First, read the original metadata
    std::cout << "Reading original metadata..." << std::endl;
    auto read_result = core.process_file(file_path, file_handler::operation_type::READ);

    if (!read_result.success) {
        std::cout << "Failed to read original metadata: " << read_result.message << std::endl;
        restore_from_backup(file_path, backup_path);
        return;
    }

    size_t original_metadata_count = read_result.metadata.size();
    std::cout << "Original metadata count: " << original_metadata_count << std::endl;

    // Now, clean the metadata
    std::cout << "Cleaning metadata..." << std::endl;
    auto clean_result = core.process_file(file_path, file_handler::operation_type::CLEAN);

    std::cout << "Clean operation result: " << (clean_result.success ? "Success" : "Failed") << std::endl;
    std::cout << "Message: " << clean_result.message << std::endl;

    if (!clean_result.success) {
        std::cout << "Cleaning failed, restoring from backup..." << std::endl;
        restore_from_backup(file_path, backup_path);
        return;
    }

    // Read the metadata again to verify it was cleaned
    std::cout << "Reading metadata after cleaning..." << std::endl;
    auto read_after_clean = core.process_file(file_path, file_handler::operation_type::READ);

    if (!read_after_clean.success) {
        std::cout << "Failed to read metadata after cleaning: " << read_after_clean.message << std::endl;
        restore_from_backup(file_path, backup_path);
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

    // Restore the original file from backup
    std::cout << "Test completed, restoring original file..." << std::endl;
    restore_from_backup(file_path, backup_path);

    std::cout << std::endl;
}

/**
 * @brief Main function
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit code
 */
int main(int argc, char* argv[]) {
    #ifdef _WIN32
        // set output to utf-8 encoding, ensure console can properly display
        SetConsoleOutputCP(CP_UTF8);
    #endif

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

    // Test reading metadata
    test_read_metadata(core, test_file_path);

    // Test cleaning metadata
    test_clean_metadata(core, test_file_path);

    std::cout << "Testing completed!" << std::endl;
    return 0;
}