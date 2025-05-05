/**
 * @file test_utils.cpp
 * @brief 测试工具函数实现
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
    // 设置控制台输出为UTF-8编码
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::cout << "控制台初始化完成" << std::endl;
}

bool check_test_file(const std::string& file_path) {
    if (std::filesystem::exists(file_path)) {
        std::cout << "测试文件存在: " << file_path << std::endl;
        return true;
    }

    std::cout << "测试文件不存在: " << file_path << std::endl;
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
        std::cout << "创建测试副本: " << test_copy << std::endl;
        return test_copy;
    } catch (const std::exception& e) {
        std::cerr << "创建测试副本失败: " << e.what() << std::endl;
        return "";
    }
}

void print_operation_result(const file_handler::operation_result& result) {
    std::cout << "操作结果: " << (result.success ? "成功" : "失败") << std::endl;
    std::cout << "消息: " << result.message << std::endl;

    if (!result.warnings.empty()) {
        std::cout << "警告:" << std::endl;
        for (const auto& warning : result.warnings) {
            std::cout << "  " << warning << std::endl;
        }
    }

    std::cout << "元数据数量: " << result.metadata.size() << std::endl;
    if (!result.metadata.empty()) {
        std::cout << "元数据内容:" << std::endl;
        for (const auto& [key, value] : result.metadata) {
            std::cout << "  " << key << ": " << value << std::endl;
        }
    }
}

} // namespace test_utils