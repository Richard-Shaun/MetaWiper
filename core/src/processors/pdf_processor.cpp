//need to be checked
#include <iostream>
#include <filesystem>
#include <fstream>
#include "./processors/pdf_processor.h"

namespace pdf_processor {

    pdf_processor_class::pdf_processor_class(const std::string& path,
                                             file_handler::operation_type type,
                                             const file_handler::operation_options& opts)
             : file_handler_class(path, type, opts),pdf_loaded(false) {
        try {
            // load pdf file
            pdf_document = std::make_unique<PoDoFo::PdfMemDocument>();
            pdf_document->Load(file_path.c_str());
            pdf_loaded = true;
        } catch (const PoDoFo::PdfError& e) {
            std::cerr << "Failed to load PDF document: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    pdf_processor_class::~pdf_processor_class() = default;

    file_handler::operation_result pdf_processor_class::execute_operation() {
        if (!pdf_loaded) {
            return {false, "Failed to load PDF document", {}, {}};
        }

        // call different operations based on the type
        switch (type) {
            case file_handler::operation_type::READ:
                return read_metadata();

            case file_handler::operation_type::CLEAN:
                return clean_metadata();

            case file_handler::operation_type::OVERWRITE:
                return overwrite_metadata();

            case file_handler::operation_type::EXPORT:
                return export_metadata();

            case file_handler::operation_type::RESTORE:
                return restore_metadata();

            default:
                return {false, "Unsupported operation type", {}, {}};
        }
    }

    file_handler::operation_result pdf_processor_class::read_metadata() {
        file_handler::operation_result result;
        result.success = true;
        result.message = "Metadata successfully read";

        try {
            // 获取文档信息字典
            PoDoFo::PdfObject* infoObj = nullptr;

            // 在 PoDoFo 0.10.4 中获取 Info 对象
            if (pdf_document->GetTrailer().GetDictionary().HasKey(PoDoFo::PdfName("Info"))) {
                infoObj = pdf_document->GetTrailer().GetDictionary().FindKey(PoDoFo::PdfName("Info"));
            }

            if (infoObj != nullptr && infoObj->IsDictionary()) {
                PoDoFo::PdfDictionary& infoDictionary = infoObj->GetDictionary();

                // 读取标准元数据
                auto readDictString = [&result, &infoDictionary](const char* key) {
                    if (infoDictionary.HasKey(PoDoFo::PdfName(key))) {
                        PoDoFo::PdfObject* keyObj = infoDictionary.FindKey(PoDoFo::PdfName(key));
                        if (keyObj != nullptr && keyObj->IsString()) {
                            result.metadata[key] = keyObj->GetString().GetString();
                        }
                    }
                };

                readDictString("Title");
                readDictString("Author");
                readDictString("Subject");
                readDictString("Keywords");
                readDictString("Creator");
                readDictString("Producer");

                // 读取创建和修改日期
                auto readDate = [&result, &infoDictionary](const char* key, const char* resultKey) {
                    if (infoDictionary.HasKey(PoDoFo::PdfName(key))) {
                        PoDoFo::PdfObject* dateObj = infoDictionary.FindKey(PoDoFo::PdfName(key));
                        if (dateObj != nullptr && dateObj->IsString()) {
                            // 尝试解析 PDF 日期格式字符串
                            result.metadata[resultKey] = dateObj->GetString().GetString();
                        }
                    }
                };

                readDate("CreationDate", "CreationDate");
                readDate("ModDate", "ModificationDate");
            }

            // 检查是否有 XMP 元数据
            // 在 PoDoFo 0.10.4 中获取 Catalog
            PoDoFo::PdfDictionary& catalogDict = pdf_document->GetCatalog().GetDictionary();
            if (catalogDict.HasKey(PoDoFo::PdfName("Metadata"))) {
                PoDoFo::PdfObject* metadataObj = catalogDict.FindKey(PoDoFo::PdfName("Metadata"));
                result.metadata["HasXMPMetadata"] = "true";

                // 如果需要，可以提取 XMP 内容
                if (metadataObj != nullptr && metadataObj->HasStream()) {
                    // 获取 XMP 流内容如果需要
                    // auto stream = metadataObj->GetStream();
                    // result.metadata["XMP"] = ...
                }
            }

        } catch (const PoDoFo::PdfError& e) {
            result.success = false;
            result.message = "Failed to read metadata: " + std::string(e.what());
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Exception: " + std::string(e.what());
        }

        return result;
    }

    file_handler::operation_result pdf_processor_class::clean_metadata() {
        file_handler::operation_result result;
        result.success = true;
        result.message = "Metadata successfully cleaned";

        try {
            // 获取 Info 字典对象
            if (pdf_document->GetTrailer().GetDictionary().HasKey(PoDoFo::PdfName("Info"))) {
                // 在 PoDoFo 0.10.4 中移除 Info 键
                pdf_document->GetTrailer().GetDictionary().RemoveKey(PoDoFo::PdfName("Info"));
            }

            // 移除 XMP 元数据
            PoDoFo::PdfDictionary& catalogDict = pdf_document->GetCatalog().GetDictionary();
            if (catalogDict.HasKey(PoDoFo::PdfName("Metadata"))) {
                catalogDict.RemoveKey(PoDoFo::PdfName("Metadata"));
            }

            // 保存文档
            pdf_document->Save(file_path.c_str(), PoDoFo::PdfSaveOptions::Clean);

        } catch (const PoDoFo::PdfError& e) {
            result.success = false;
            result.message = "Failed to clean metadata: " + std::string(e.what());
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Exception: " + std::string(e.what());
        }

        return result;
    }

    file_handler::operation_result pdf_processor_class::overwrite_metadata() {
        file_handler::operation_result result;
        result.success = true;
        result.message = "Metadata successfully overwritten";

        try {
            // 获取或创建 Info 字典
            PoDoFo::PdfObject* infoObj = nullptr;

            // 在 PoDoFo 0.10.4 中获取或创建 Info 对象
            if (pdf_document->GetTrailer().GetDictionary().HasKey(PoDoFo::PdfName("Info"))) {
                infoObj = pdf_document->GetTrailer().GetDictionary().FindKey(PoDoFo::PdfName("Info"));
            } else {
                // 如果 Info 不存在，创建一个新的
                infoObj = &pdf_document->GetObjects().CreateDictionaryObject();
                pdf_document->GetTrailer().GetDictionary().AddKey(PoDoFo::PdfName("Info"), *infoObj);
            }

            // 确保它是一个字典
            if (!infoObj->IsDictionary()) {
                result.success = false;
                result.message = "Info object is not a dictionary";
                return result;
            }

            PoDoFo::PdfDictionary& infoDictionary = infoObj->GetDictionary();

            // 从选项中设置标准元数据
            auto& metadata = options.overwrite_metadata;

            auto setDictString = [&metadata, &infoDictionary](const std::string& key) {
                if (metadata.find(key) != metadata.end()) {
                    infoDictionary.AddKey(PoDoFo::PdfName(key),
                                         PoDoFo::PdfString(metadata[key]));
                }
            };

            setDictString("Title");
            setDictString("Author");
            setDictString("Subject");
            setDictString("Keywords");
            setDictString("Creator");
            setDictString("Producer");

            // 保存文档
            pdf_document->Save(file_path.c_str(), PoDoFo::PdfSaveOptions::None);

        } catch (const PoDoFo::PdfError& e) {
            result.success = false;
            result.message = "Failed  to overwrite metadata: " + std::string(e.what());
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Exception: " + std::string(e.what());
        }

        return result;
    }

    file_handler::operation_result pdf_processor_class::export_metadata() {
        // 先读取元数据
        auto result = read_metadata();
        if (!result.success) {
            return result;
        }

        try {
            // 创建输出目录
            if (!options.output_directory.empty() && !std::filesystem::exists(options.output_directory)) {
                std::filesystem::create_directories(options.output_directory);
            }

            // 创建输出文件路径
            std::filesystem::path output_path;
            if (options.output_directory.empty()) {
                output_path = std::filesystem::path(file_path).parent_path() /
                             (std::filesystem::path(file_path).stem().string() + "_metadata.json");
            } else {
                output_path = options.output_directory /
                             (std::filesystem::path(file_path).stem().string() + "_metadata.json");
            }

            // 写入输出文件
            std::ofstream out(output_path);
            if (!out) {
                result.success = false;
                result.message = "Failed to create output file: " + output_path.string();
                return result;
            }

            // 写入 JSON 格式
            out << "{\n";
            bool first = true;
            for (const auto& [key, value] : result.metadata) {
                if (!first) out << ",\n";
                out << "  \"" << key << "\": \"" << value << "\"";
                first = false;
            }
            out << "\n}";

            out.close();

            result.message = "Metadata successfully exported to: " + output_path.string();

        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Failed to export metadata: " + std::string(e.what());
        }

        return result;
    }

    file_handler::operation_result pdf_processor_class::restore_metadata() {
        // 从备份文件或导出文件恢复元数据
        file_handler::operation_result result;
        result.success = false;
        result.message = "Restore operation not implemented for PDF";

        return result;
    }

}