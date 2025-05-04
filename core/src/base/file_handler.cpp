#include <algorithm>
#include <filesystem>
#include "./base/file_handler.h"
#include "./processors/pdf_processor.h"
#include "./processors/jpeg_processor.h"

namespace file_handler {

    file_handler_class::file_handler_class(const std::string &path, operation_type type, const operation_options& opts)
        : file_properties_class(path), type(type), options(opts) {}

    file_handler_class::~file_handler_class() = default;

    operation_result file_handler_class::execute_operation() {

        auto prereq_result = check_prerequisites();
        if (!prereq_result.success) {
            return prereq_result;
        }

        // 根据操作类型分发到不同处理函数
        switch (type) {
            case operation_type::READ:
                return read_metadata();

            case operation_type::CLEAN:
                return clean_metadata();

            case operation_type::OVERWRITE:
                return overwrite_metadata();

            case operation_type::EXPORT:
                return export_metadata();

            case operation_type::RESTORE:
                return restore_metadata();

            default:
                return {false, "Unsupported operation type", {}, {}};
        }
    }

    std::unique_ptr<file_handler_class> create_handler(
        const std::string& file_path,
        operation_type op_type,
        const operation_options& options
    ) {
        std::filesystem::path path(file_path);
        std::string extension = path.extension().string();
        if (!extension.empty() && extension[0] == '.') {
            extension = extension.substr(1);
        }
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        if (extension == "pdf") {
            return std::make_unique<pdf_processor::pdf_processor_class>(file_path, op_type, options);
        }
        else if (extension == "jpeg" || extension == "jpg") {
            return std::make_unique<jpeg_processor::jpeg_processor_class>(file_path, op_type, options);
        }
        return nullptr;
    }

}
