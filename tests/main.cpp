/**
 * @file main.cpp
 * @brief Main entry point for MetaWiper test program
 */

#include <iostream>
#include <string>
#include <filesystem>
#include <test_utils.h>
#include <meta_wiper_core.h>

// Declare functions from test namespaces
namespace pdf_test {
    void run_pdf_tests(const std::string& file_path);
}

namespace jpeg_test {
    void run_jpeg_tests(const std::string& file_path);
}

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
    std::cout << std::endl << std::endl;
}

/**
 * @brief Main function
 * @param argc Argument count
 * @param argv Argument array
 * @return Exit code
 */
int main(int argc, char* argv[]) {
    // Initialize console
    test_utils::init_console();

    std::cout << "MetaWiper Test Program" << std::endl;
    std::cout << "======================" << std::endl;

    // Create core library instance
    meta_wiper_core::meta_wiper_core_class core;

    // Test supported file types
    test_supported_file_types(core);

    // Get test file paths
    std::string pdf_file_path;
    std::string jpeg_file_path;

    if (argc > 1) {
        pdf_file_path = argv[1];
    }

    if (argc > 2) {
        jpeg_file_path = argv[2];
    }

    // If paths not provided via command line, ask user
    if (pdf_file_path.empty()) {
        std::cout << "Enter PDF test file path (or press Enter to skip): ";
        std::getline(std::cin, pdf_file_path);
    }

    if (jpeg_file_path.empty()) {
        std::cout << "Enter JPEG test file path (or press Enter to skip): ";
        std::getline(std::cin, jpeg_file_path);
    }

    // Simplify file paths
    if (!pdf_file_path.empty()) {
        try {
            pdf_file_path = std::filesystem::absolute(pdf_file_path).string();
        } catch (...) {
            // Continue with original path if unable to get absolute path
        }
    }

    if (!jpeg_file_path.empty()) {
        try {
            jpeg_file_path = std::filesystem::absolute(jpeg_file_path).string();
        } catch (...) {
            // Continue with original path if unable to get absolute path
        }
    }

    // Run PDF tests
    pdf_test::run_pdf_tests(pdf_file_path);

    // Run JPEG tests
    jpeg_test::run_jpeg_tests(jpeg_file_path);

    std::cout << "\nAll tests completed!" << std::endl;
    return 0;
}