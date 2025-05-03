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
        file_handler::operation_result execute_operation() override;
    private:
        file_handler::operation_result read_metadata();
        file_handler::operation_result clean_metadata();
        file_handler::operation_result overwrite_metadata();
        file_handler::operation_result export_metadata();
        file_handler::operation_result restore_metadata();

        std::unique_ptr<PoDoFo::PdfMemDocument> pdf_document;
        bool pdf_loaded;
    };

}