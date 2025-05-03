#pragma once

#include <string>
#include <vector>
#include <memory>
#include "./base/file_handler.h"
#include "meta_wipe_core_export.h"

namespace meta_wiper_core {

    enum class error_code {
        SUCCESS,
        FILE_NOT_FOUND,
        UNSUPPORTED_FILE_TYPE,
        PERMISSION_DENIED,
        PROCESSING_ERROR,
        UNKNOWN_ERROR
    };

    struct meta_item {
        std::string key;
        std::string value;
        bool editable;
        bool removable;
    };

    struct process_result {
        error_code error = error_code::SUCCESS;
        std::string errorMessage;
        std::vector<meta_item> metadata;
    };

    class META_WIPER_CORE_EXPORT_FLAG meta_wiper_core_class {
    public:
        meta_wiper_core_class();
        ~meta_wiper_core_class();
        file_handler::operation_result process_file(
            const std::string& file_path,
            file_handler::operation_type op_type,
            const file_handler::operation_options& options = {}
        );
        std::vector<file_handler::operation_result> process_files(
            const std::vector<std::string>& file_paths,
            file_handler::operation_type op_type,
            const file_handler::operation_options& options = {}
        );
        std::vector<std::string> get_supported_file_types() const;
        bool type_supported(const std::string& file_type) const;
    };

}