#pragma once

#include <memory>
#include <exiv2/exiv2.hpp>
#include "./base/file_handler.h"

namespace jpeg_processor {

    class jpeg_processor_class : public file_handler::file_handler_class {
    public:
        jpeg_processor_class(const std::string& path,
                             file_handler::operation_type type,
                             const file_handler::operation_options& opts);
        ~jpeg_processor_class() override;;
    protected:
        file_handler::operation_result check_prerequisites() override;
        file_handler::operation_result read_metadata() override;
        file_handler::operation_result clean_metadata() override;
        file_handler::operation_result overwrite_metadata() override;
        file_handler::operation_result export_metadata() override;
        file_handler::operation_result restore_metadata() override;
    private:
        std::unique_ptr<Exiv2::Image> jpeg_image;
        bool jpeg_loaded;
    };

}

#include "./base/processor_factory.h"

namespace {
    /**
     * @brief Static registrar for JPEG files
     */
    processor_factory::processor_registrar<
        jpeg_processor::jpeg_processor_class,
        file_properties::type_major::JPEG,
        file_properties::type_minor::UNKNOWN
    > register_jpeg_processor;
}