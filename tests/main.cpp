/**
 * @file main.cpp
 * @brief MetaWiper测试程序主入口
 */

#include <iostream>
#include <string>
#include <filesystem>
#include <test_utils.h>
#include <meta_wiper_core.h>

// 声明测试命名空间中的函数
namespace pdf_test {
    void run_pdf_tests(const std::string& file_path);
}

namespace jpeg_test {
    void run_jpeg_tests(const std::string& file_path);
}

/**
 * @brief 测试支持的文件类型
 * @param core Meta wiper core实例
 */
void test_supported_file_types(meta_wiper_core::meta_wiper_core_class& core) {
    std::cout << "=== 测试支持的文件类型 ===" << std::endl;

    auto file_types = core.get_supported_file_types();
    std::cout << "支持的文件类型数量: " << file_types.size() << std::endl;

    std::cout << "支持的文件类型: ";
    for (const auto& type : file_types) {
        std::cout << type << " ";
    }
    std::cout << std::endl << std::endl;
}

/**
 * @brief 主函数
 * @param argc 参数数量
 * @param argv 参数数组
 * @return 退出码
 */
int main(int argc, char* argv[]) {
    // 初始化控制台
    test_utils::init_console();

    std::cout << "MetaWiper测试程序" << std::endl;
    std::cout << "================" << std::endl;

    // 创建核心库实例
    meta_wiper_core::meta_wiper_core_class core;

    // 测试支持的文件类型
    test_supported_file_types(core);

    // 获取测试文件路径
    std::string pdf_file_path;
    std::string jpeg_file_path;

    if (argc > 1) {
        pdf_file_path = argv[1];
    }

    if (argc > 2) {
        jpeg_file_path = argv[2];
    }

    // 如果命令行未提供路径，询问用户
    if (pdf_file_path.empty()) {
        std::cout << "输入PDF测试文件路径 (或按Enter跳过): ";
        std::getline(std::cin, pdf_file_path);
    }

    if (jpeg_file_path.empty()) {
        std::cout << "输入JPEG测试文件路径 (或按Enter跳过): ";
        std::getline(std::cin, jpeg_file_path);
    }

    // 简化文件路径
    if (!pdf_file_path.empty()) {
        try {
            pdf_file_path = std::filesystem::absolute(pdf_file_path).string();
        } catch (...) {
            // 如果无法获取绝对路径，继续使用原始路径
        }
    }

    if (!jpeg_file_path.empty()) {
        try {
            jpeg_file_path = std::filesystem::absolute(jpeg_file_path).string();
        } catch (...) {
            // 如果无法获取绝对路径，继续使用原始路径
        }
    }

    // 运行PDF测试
    pdf_test::run_pdf_tests(pdf_file_path);

    // 运行JPEG测试
    jpeg_test::run_jpeg_tests(jpeg_file_path);

    std::cout << "\n所有测试完成！" << std::endl;
    return 0;
}