/**
 * @file docx_processor.h
 * @brief Processor for DOCX file format metadata
 */
#pragma once

#include <memory>
#include <pugixml.hpp>
#include "./base/file_handler.h"

namespace docx_processor {

    /**
     * @brief DOCX metadata processor
     *
     * This class handles DOCX files metadata operations using pugixml library
     */
    class docx_processor_class : public file_handler::file_handler_class {
    public:
        /**
         * @brief Constructor
         * @param path Path to the DOCX file
         * @param type Operation type
         * @param opts Operation options
         */
        docx_processor_class(const std::string& path,
                             file_handler::operation_type type,
                             const file_handler::operation_options& opts);

        /**
         * @brief Destructor
         */
        ~docx_processor_class() override;

    protected:
        /**
         * @brief Check if the file is a valid DOCX
         * @return Operation result
         */
        file_handler::operation_result check_prerequisites() override;

        /**
         * @brief Read metadata from DOCX file
         * @return Operation result with metadata
         */
        file_handler::operation_result read_metadata() override;

        /**
         * @brief Clean metadata from DOCX file
         * @return Operation result
         */
        file_handler::operation_result clean_metadata() override;

        /**
         * @brief Overwrite metadata in DOCX file
         * @return Operation result
         */
        file_handler::operation_result overwrite_metadata() override;

        /**
         * @brief Export metadata to external file
         * @return Operation result
         */
        file_handler::operation_result export_metadata() override;

        /**
         * @brief Restore metadata from backup
         * @return Operation result
         */
        file_handler::operation_result restore_metadata() override;

    private:
        /**
         * @brief Extract XML file from DOCX archive
         * @param xml_path Path to XML file within archive
         * @param content Output string to store content
         * @return True if successful, false otherwise
         */
        bool extract_xml_file(const std::string& xml_path, std::string& content);

        /**
         * @brief Update XML file in DOCX archive
         * @param xml_path Path to XML file within archive
         * @param content New content for XML file
         * @return True if successful, false otherwise
         */
        bool update_xml_file(const std::string& xml_path, const std::string& content);

        /**
         * @brief Create temporary directory for processing
         * @return Path to temporary directory
         */
        std::string create_temp_directory();

        /**
         * @brief Clean up temporary directory
         * @param temp_dir Path to temporary directory
         */
        void cleanup_temp_directory(const std::string& temp_dir);

        // Private member variables
        std::unique_ptr<pugi::xml_document> core_xml;
        std::unique_ptr<pugi::xml_document> app_xml;
        bool docx_loaded;
    };

}

#include "./base/processor_factory.h"

namespace {
    /**
     * @brief Static registrar for DOCX files
     */
    processor_factory::processor_registrar<
        docx_processor::docx_processor_class,
        file_properties::type_major::WORD,
        file_properties::type_minor::DOCX
    > register_docx_processor;
}