#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include "./base/file_properties.h"

namespace file_handler {

    enum class operation_type {
        READ,
        CLEAN,
        OVERWRITE,
        EXPORT,
        RESTORE
    };

    struct operation_options {
        std::vector<std::string> selected_properties;
        std::filesystem::path output_directory;
        std::unordered_map<std::string, std::string> overwrite_metadata;
    };

    struct operation_result {
        bool success;
        std::string message;
        std::vector<std::string> warnings;
        std::unordered_map<std::string, std::string> metadata;
    };


    class file_handler_class : public file_properties::file_properties_class {
    protected:
        operation_type type {operation_type::READ};
        operation_options options;
        virtual operation_result check_prerequisites() {
            return {true, "",{}, {}};
        }
        virtual operation_result read_metadata() {
            return {false, "Operation not supported", {}, {}};
        }
        virtual operation_result clean_metadata() {
            return {false, "Operation not supported", {}, {}};
        }
        virtual operation_result overwrite_metadata() {
            return {false, "Operation not supported", {}, {}};
        }
        virtual operation_result export_metadata() {
            return {false, "Operation not supported", {}, {}};
        }
        virtual operation_result restore_metadata() {
            return {false, "Operation not supported", {}, {}};
        }

    public:
        file_handler_class(const std::string& path, operation_type type, const operation_options& opts);
        ~file_handler_class() override;
        operation_result execute_operation();

    };

    /**
     * @brief Create appropriate file handler for the given file
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
    );

}
