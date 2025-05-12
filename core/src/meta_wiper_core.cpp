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

        // Check if file exists
        if (!std::filesystem::exists(file_path)) {
            return {false, "File does not exist: " + file_path, {}, {}};
        }

        // Create appropriate handler
        auto handler = file_handler::create_handler(file_path, op_type, options);
        if (!handler) {
            return {false, "Unsupported file format", {}, {}};
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

    std::vector<std::string> meta_wiper_core_class::get_supported_file_types() {
        // Return all supported extensions
        // In a more advanced implementation, this could query the processor_factory
        return {".pdf", ".jpg", ".jpeg", ".docx"};
    }

    bool meta_wiper_core_class::type_supported(const std::string& file_extension) const {
        std::string ext = file_extension;

        // Ensure extension starts with dot
        if (!ext.empty() && ext[0] != '.') {
            ext = "." + ext;
        }

        // Convert to lowercase
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        // Check if extension is in supported formats
        const auto formats = get_supported_file_types();
        return std::find(formats.begin(), formats.end(), ext) != formats.end();
    }

}