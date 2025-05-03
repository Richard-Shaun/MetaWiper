//need to be checked
#include "meta_wiper_core.h"
#include <algorithm>
#include <filesystem>

namespace meta_wiper_core {

    meta_wiper_core_class::meta_wiper_core_class() = default;
    meta_wiper_core_class::~meta_wiper_core_class() = default;

    file_handler::operation_result meta_wiper_core_class::process_file(
        const std::string& file_path,
        const file_handler::operation_type op_type,
        const file_handler::operation_options& options) {

        // 检查文件是否存在
        if (!std::filesystem::exists(file_path)) {
            return {false, "File does not exist: " + file_path, {}};
        }

        // 创建合适的处理器
        auto handler = file_handler::create_handler(file_path, op_type, options);
        if (!handler) {
            return {false, "Unsupported file format", {}};
        }

        return handler->execute_operation();
    }

    std::vector<file_handler::operation_result> meta_wiper_core_class::process_files(
        const std::vector<std::string>& file_paths,
        file_handler::operation_type op_type,
        const file_handler::operation_options& options) {

        std::vector<file_handler::operation_result> results;
        results.reserve(file_paths.size());

        for (const auto& path : file_paths) {
            results.push_back(process_file(path, op_type, options));
        }

        return results;
    }

    std::vector<std::string> meta_wiper_core_class::get_supported_file_types() const {
        return {".pdf"}; // 目前仅支持PDF
    }

    bool meta_wiper_core_class::type_supported(const std::string& file_extension) const {
        std::string ext = file_extension;

        // 确保扩展名以点开头
        if (!ext.empty() && ext[0] != '.') {
            ext = "." + ext;
        }

        // 转换为小写
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        // 检查扩展名是否在支持的格式中
        const auto formats = get_supported_file_types();
        return std::find(formats.begin(), formats.end(), ext) != formats.end();
    }

}