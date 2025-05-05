/**
 * @file jpeg_test.cpp
 * @brief JPEG metadata processor test
 */
#include <meta_wiper_core.h>
#include <test_utils.h>
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>

namespace jpeg_test {

/**
 * @brief Test JPEG support status
 * @param core Metadata processor core instance
 */
void test_jpeg_support(meta_wiper_core::meta_wiper_core_class& core) {
    std::cout << "\n=== Test JPEG Support ===" << std::endl;

    bool jpeg_supported = core.type_supported("jpg");
    bool jpeg2_supported = core.type_supported("jpeg");

    std::cout << "JPEG support (.jpg): " << (jpeg_supported ? "Supported" : "Not supported") << std::endl;
    std::cout << "JPEG support (.jpeg): " << (jpeg2_supported ? "Supported" : "Not supported") << std::endl;
}

/**
 * @brief Test reading JPEG metadata
 * @param core Metadata processor core instance
 * @param file_path JPEG file path
 */
void test_read_jpeg_metadata(meta_wiper_core::meta_wiper_core_class& core, const std::string& file_path) {
    if (!test_utils::check_test_file(file_path)) {
        return;
    }

    std::cout << "\n=== Test Reading JPEG Metadata ===" << std::endl;
    std::cout << "File path: " << file_path << std::endl;

    auto result = core.process_file(file_path, file_handler::operation_type::READ);
    test_utils::print_operation_result(result);

    // Check for common JPEG metadata tags
    std::vector<std::string> common_tags = {
        "EXIF.Exif.Image.Make",
        "EXIF.Exif.Image.Model",
        "EXIF.Exif.Photo.DateTimeOriginal",
        "EXIF.Exif.Image.Software",
        "EXIF.Exif.Image.Copyright"
    };

    std::cout << "\nCommon JPEG metadata tag check:" << std::endl;
    for (const auto& tag : common_tags) {
        auto it = result.metadata.find(tag);
        if (it != result.metadata.end()) {
            std::cout << "  Found: " << tag << " = " << it->second << std::endl;
        } else {
            std::cout << "  Not found: " << tag << std::endl;
        }
    }
}

/**
 * @brief Test cleaning JPEG metadata
 * @param core Metadata processor core instance
 * @param file_path JPEG file path
 */
void test_clean_jpeg_metadata(meta_wiper_core::meta_wiper_core_class& core, const std::string& file_path) {
    if (!test_utils::check_test_file(file_path)) {
        return;
    }

    std::cout << "\n=== Test Cleaning JPEG Metadata ===" << std::endl;

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

        double reduction_percent = 100.0 * (original_metadata_count - cleaned_metadata_count) / original_metadata_count;
        std::cout << "  Reduction percentage: " << reduction_percent << "%" << std::endl;
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
 * @brief Test overwriting JPEG metadata
 * @param core Metadata processor core instance
 * @param file_path JPEG file path
 */
void test_overwrite_jpeg_metadata(meta_wiper_core::meta_wiper_core_class& core, const std::string& file_path) {
    if (!test_utils::check_test_file(file_path)) {
        return;
    }

    std::cout << "\n=== Test Overwriting JPEG Metadata ===" << std::endl;

    // Create test copy
    std::string test_copy = test_utils::create_test_copy(file_path, "_overwrite");
    if (test_copy.empty()) {
        return;
    }

    // Prepare overwrite metadata
    file_handler::operation_options options;
    options.overwrite_metadata = {
        {"EXIF.Exif.Image.Software", "MetaWiper Test Program"},
        {"EXIF.Exif.Image.Copyright", "Copyright 2025"},
        {"EXIF.Exif.Image.ImageDescription", "This is a test image"},
        {"XMP.Xmp.dc.creator", "MetaWiper Development Team"},
        {"XMP.Xmp.dc.rights", "All rights reserved"}
    };

    // Execute overwrite operation
    std::cout << "Overwriting metadata..." << std::endl;
    auto overwrite_result = core.process_file(
        test_copy,
        file_handler::operation_type::OVERWRITE,
        options
    );
    test_utils::print_operation_result(overwrite_result);

    if (!overwrite_result.success) {
        std::cout << "Failed to overwrite metadata, deleting test copy..." << std::endl;
        std::filesystem::remove(test_copy);
        return;
    }

    // Wait for file operations to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Verify overwrite results
    std::cout << "\nVerifying overwrite results..." << std::endl;
    auto read_result = core.process_file(test_copy, file_handler::operation_type::READ);
    test_utils::print_operation_result(read_result);

    // Check overwritten fields
    std::cout << "\nOverwritten field verification:" << std::endl;
    for (const auto& [key, value] : options.overwrite_metadata) {
        auto it = read_result.metadata.find(key);
        if (it != read_result.metadata.end()) {
            bool match = (it->second == value);
            std::cout << "  Field " << key << ": "
                      << (match ? "Overwrite successful" : "Overwrite mismatch")
                      << " (Value: " << it->second << ")" << std::endl;
        } else {
            std::cout << "  Field " << key << ": Not found, overwrite may have failed" << std::endl;
        }
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
 * @brief Run all JPEG tests
 * @param file_path JPEG test file path
 */
void run_jpeg_tests(const std::string& file_path) {
    std::cout << "\n======== JPEG Metadata Tests ========" << std::endl;

    meta_wiper_core::meta_wiper_core_class core;

    // Test JPEG support
    test_jpeg_support(core);

    // Skip file-related tests if no file path provided
    if (file_path.empty()) {
        std::cout << "\nNo JPEG file path provided, skipping file tests" << std::endl;
        return;
    }

    // Test metadata reading
    test_read_jpeg_metadata(core, file_path);

    // Test metadata cleaning
    test_clean_jpeg_metadata(core, file_path);

    // Test metadata overwriting
    test_overwrite_jpeg_metadata(core, file_path);

    std::cout << "\nJPEG tests completed!" << std::endl;
}

}