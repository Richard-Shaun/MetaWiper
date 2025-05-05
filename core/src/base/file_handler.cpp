#include <algorithm>
#include <filesystem>
#include "./base/file_handler.h"
#include "./base/processor_factory.h"

namespace file_handler {

    file_handler_class::file_handler_class(const std::string &path, operation_type type, const operation_options& opts)
        : file_properties_class(path), type(type), options(opts) {}

    file_handler_class::~file_handler_class() = default;

    operation_result file_handler_class::execute_operation() {
        auto prereq_result = check_prerequisites();
        if (!prereq_result.success) {
            return prereq_result;
        }

        // Dispatch to appropriate handler based on operation type
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

    /**
     * @brief Create appropriate file handler for the given file
     *
     * This function delegates to processor_factory to create the appropriate
     * handler based on file type determined by file_properties.
     *
     * @param file_path Path to the file
     * @param op_type Operation type
     * @param options Operation options
     * @return Unique pointer to file handler
     */
    std::unique_ptr<file_handler_class> create_handler(
        const std::string& file_path,
        operation_type op_type,
        const operation_options& options
    ) {
        return processor_factory::processor_factory_class::create_processor(
            file_path, op_type, options);
    }

}