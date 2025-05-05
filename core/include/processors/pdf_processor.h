#pragma once

#include <memory>
#include <podofo/podofo.h>
#include "./base/file_handler.h"

namespace pdf_processor {

    /**
     * @brief PDF metadata processor
     *
     * This class handles PDF files metadata operations using PoDoFo library
     */
    class pdf_processor_class : public file_handler::file_handler_class {
    public:
        pdf_processor_class(const std::string& path,
                      file_handler::operation_type type,
                      const file_handler::operation_options& opts);
        ~pdf_processor_class() override;
    protected:
        file_handler::operation_result check_prerequisites() override;
        file_handler::operation_result read_metadata() override;
        file_handler::operation_result clean_metadata() override;
        file_handler::operation_result overwrite_metadata() override;
        file_handler::operation_result export_metadata() override;
        file_handler::operation_result restore_metadata() override;
    private:
        std::unique_ptr<PoDoFo::PdfMemDocument> pdf_document;
        bool pdf_loaded;
    };

}


#include "./base/processor_factory.h"

namespace {
    /**
     * @brief Static registrar for regular PDF files
     */
    processor_factory::processor_registrar<
        pdf_processor::pdf_processor_class,
        file_properties::type_major::PDF,
        file_properties::type_minor::UNKNOWN
    > register_pdf_processor;

    /**
     * @brief Static registrar for PDF/A files
     */
    processor_factory::processor_registrar<
        pdf_processor::pdf_processor_class,
        file_properties::type_major::PDF,
        file_properties::type_minor::PDFA
    > register_pdfa_processor;
}