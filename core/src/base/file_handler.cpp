#include <algorithm>
#include <filesystem>
#include "./base/file_handler.h"
#include "./processors/pdf_processor.h"

namespace file_handler {

    file_handler_class::file_handler_class(const std::string &path, operation_type type, const operation_options& opts)
        : file_properties_class(path), type(type), options(opts) {}

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
        return nullptr;
    }

}
