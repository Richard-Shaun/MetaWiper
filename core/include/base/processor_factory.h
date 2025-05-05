/**
 * @file processor_factory.h
 * @brief Factory for creating file handlers based on file types
 */
#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include "./base/file_handler.h"
#include "./base/file_properties.h"

namespace processor_factory {

    /**
     * @brief Hash function for std::pair to use in unordered_map
     */
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };

    /**
     * @brief Factory for creating file handlers based on file types
     */
    class processor_factory_class {
    public:
        /**
         * @brief Creator function type for file handlers
         */
        using creator_func = std::function<std::unique_ptr<file_handler::file_handler_class>(
            const std::string&,
            file_handler::operation_type,
            const file_handler::operation_options&)>;

        /**
         * @brief Register a processor factory for a specific file type
         * @param major Major file type
         * @param minor Minor file type
         * @param creator Creator function for the processor
         */
        static void register_processor(
            file_properties::type_major major,
            file_properties::type_minor minor,
            creator_func creator);

        /**
         * @brief Create a processor for the given file type
         * @param file_path Path to the file
         * @param op_type Operation type
         * @param options Operation options
         * @return Unique pointer to file handler
         */
        static std::unique_ptr<file_handler::file_handler_class> create_processor(
            const std::string& file_path,
            file_handler::operation_type op_type,
            const file_handler::operation_options& options);

    private:
        /**
         * @brief Map of registered processor factories
         * Key: pair of (major_type, minor_type)
         * Value: creator function
         */
        static std::unordered_map<
            std::pair<file_properties::type_major, file_properties::type_minor>,
            creator_func,
            pair_hash> factories;
    };

    /**
     * @brief Template for automatic processor registration
     * @tparam ProcessorType The processor class type
     * @tparam Major The major file type
     * @tparam Minor The minor file type
     */
    template<typename ProcessorType, file_properties::type_major Major, file_properties::type_minor Minor>
    class processor_registrar {
    public:
        /**
         * @brief Constructor registers the processor at compile time
         */
        processor_registrar() {
            processor_factory_class::register_processor(
                Major, Minor,
                [](const std::string& path, file_handler::operation_type type, const file_handler::operation_options& opts) {
                    return std::make_unique<ProcessorType>(path, type, opts);
                }
            );
        }
    };

}