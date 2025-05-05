/**
 * @file pdf_test.cpp
 * @brief PDF metadata processor test
 */

#include <meta_wiper_core.h>
#include <test_utils.h>
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>

namespace pdf_test {

/**
 * @brief Test PDF support status
 * @param core Metadata processor core instance
 */
void test_pdf_support(meta_wiper_core::meta_wiper_core_class& core) {
    std::cout << "\n=== Test PDF Support ===" << std::endl;

    bool pdf_supported = core.type_supported("pdf");
    std::cout << "PDF support: " << (pdf_supported ? "Supported" : "Not supported") << std::endl;
}

/**
 * @brief Test reading PDF metadata
 * @param core Metadata processor core instance
 * @param file_path PDF file path
 */
void test_read_pdf_metadata(meta_wiper_core::meta_wiper_core_class& core, const std::string& file_path) {
    if (!test_utils::check_test_file(file_path)) {
        return;
    }

    std::cout << "\n=== Test Reading PDF Metadata ===" << std::endl;
    std::cout << "File path: " << file_path << std::endl;

    auto result = core.process_file(file_path, file_handler::operation_type::READ);
    test_utils::print_operation_result(result);
}

/**
 * @brief Test cleaning PDF metadata
 * @param core Metadata processor core instance
 * @param file_path PDF file path
 */
void test_clean_pdf_metadata(meta_wiper_core::meta_wiper_core_class& core, const std::string& file_path) {
    if (!test_utils::check_test_file(file_path)) {
        return;
    }

    std::cout << "\n=== Test Cleaning PDF Metadata ===" << std::endl;

    // Create test copy
    std::string test_copy = test_utils::create_test_copy(file_path);
    if (test_copy.empty()) {
        return;
    }

    // Read original metadata
    std::cout << "Reading original metadata..." << std::endl;
    auto read_result = core.process_file(test_copy, file_handler::operation_type::READ);

    if (!read_result.success) {
        std::cout << "Failed to read original metadata: " << read_result.message << std::endl;
        std::filesystem::remove(test_copy);
        return;
    }

    size_t original_metadata_count = read_result.metadata.size();
    std::cout << "Original metadata count: " << original_metadata_count << std::endl;

    // Clean metadata
    std::cout << "\nCleaning metadata..." << std::endl;
    auto clean_result = core.process_file(test_copy, file_handler::operation_type::CLEAN);
    test_utils::print_operation_result(clean_result);

    if (!clean_result.success) {
        std::cout << "Failed to clean metadata, deleting test copy..." << std::endl;
        std::filesystem::remove(test_copy);
        return;
    }

    // Wait for file operations to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Read metadata again
    std::cout << "\nReading metadata after cleaning..." << std::endl;
    auto read_after_clean = core.process_file(test_copy, file_handler::operation_type::READ);
    test_utils::print_operation_result(read_after_clean);

    // Evaluate cleaning effect
    size_t cleaned_metadata_count = read_after_clean.metadata.size();
    std::cout << "\nCleaning effect evaluation:" << std::endl;
    std::cout << "  Original metadata count: " << original_metadata_count << std::endl;
    std::cout << "  Cleaned metadata count: " << cleaned_metadata_count << std::endl;

    if (cleaned_metadata_count < original_metadata_count) {
        std::cout << "  Conclusion: Metadata cleaning successful!" << std::endl;
    } else {
        std::cout << "  Conclusion: Metadata not reduced, cleaning may not be successful" << std::endl;
    }

    // Clean up test copy
    try {
        std::filesystem::remove(test_copy);
        std::cout << "Test copy deleted" << std::endl;
    } catch (...) {
        std::cerr << "Failed to delete test copy" << std::endl;
    }
}

/**
 * @brief Run all PDF tests
 * @param file_path PDF test file path
 */
void run_pdf_tests(const std::string& file_path) {
    std::cout << "\n======== PDF Metadata Tests ========" << std::endl;

    meta_wiper_core::meta_wiper_core_class core;

    // Test PDF support
    test_pdf_support(core);

    // Skip file-related tests if no file path provided
    if (file_path.empty()) {
        std::cout << "\nNo PDF file path provided, skipping file tests" << std::endl;
        return;
    }

    // Test metadata reading
    test_read_pdf_metadata(core, file_path);

    // Test metadata cleaning
    test_clean_pdf_metadata(core, file_path);

    std::cout << "\nPDF tests completed!" << std::endl;
}

} // namespace pdf_test