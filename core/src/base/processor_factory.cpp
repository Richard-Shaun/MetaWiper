/**
* @file processor_factory.cpp
 * @brief Implementation of processor factory
 */
#include "./base/processor_factory.h"
#include <utility>

namespace processor_factory {

    // Initialize static member
    std::unordered_map<
        std::pair<file_properties::type_major, file_properties::type_minor>,
        processor_factory_class::creator_func,
        pair_hash> processor_factory_class::factories;

    void processor_factory_class::register_processor(
        file_properties::type_major major,
        file_properties::type_minor minor,
        creator_func creator)
    {
        factories[{major, minor}] = std::move(creator);
    }

    std::unique_ptr<file_handler::file_handler_class> processor_factory_class::create_processor(
        const std::string& file_path,
        file_handler::operation_type op_type,
        const file_handler::operation_options& options)
    {
        // Create a temporary file_properties object to determine the file type
        file_properties::file_properties_class file_props(file_path);

        // Get the file types
        auto major = file_props.get_file_type_major();
        auto minor = file_props.get_file_type_minor();

        // Look for an exact match first
        auto it = factories.find({major, minor});
        if (it != factories.end()) {
            return it->second(file_path, op_type, options);
        }

        // If no exact match, try with UNKNOWN minor type
        it = factories.find({major, file_properties::type_minor::UNKNOWN});
        if (it != factories.end()) {
            return it->second(file_path, op_type, options);
        }

        // No suitable processor found
        return nullptr;
    }

}