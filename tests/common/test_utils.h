/**
* @file test_utils.h
 * @brief Test utility functions
 */
#pragma once

#include <string>
#include <meta_wiper_core.h>

namespace test_utils {

 /**
  * @brief Initialize console output
  */
 void init_console();

 /**
  * @brief Check if test file exists
  * @param file_path File path
  * @return Whether the file exists
  */
 bool check_test_file(const std::string& file_path);

 /**
  * @brief Create test file copy
  * @param original_path Original file path
  * @param suffix Copy suffix
  * @return Copy file path
  */
 std::string create_test_copy(const std::string& original_path, const std::string& suffix = "_copy");

 /**
  * @brief Print operation result summary
  * @param result Operation result
  */
 void print_operation_result(const file_handler::operation_result& result);

}