#include <iostream>
#include <filesystem>
#include <fstream>
#include "./processors/jpeg_processor.h"

namespace jpeg_processor {

    jpeg_processor_class::jpeg_processor_class(const std::string& path,
                                               file_handler::operation_type type,
                                               const file_handler::operation_options& opts)
            : file_handler_class(path, type, opts), jpeg_loaded(false) {
        try {
            // load jpeg file
            jpeg_image = Exiv2::ImageFactory::open(file_path);
            if (jpeg_image.get() != nullptr) {
                jpeg_image->readMetadata();
                jpeg_loaded = true;
            }
        } catch (const Exiv2::Error& e) {
            std::cerr << "Failed to load JPEG document: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    jpeg_processor_class::~jpeg_processor_class() = default;

    file_handler::operation_result jpeg_processor_class::check_prerequisites() {
        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            return {false, "Failed to open file", {}, {}};
        }

        unsigned char header[2];
        file.read(reinterpret_cast<char*>(header), 2);

        // JPEG files start with the magic bytes FF D8
        if (file.good() && header[0] == 0xFF && header[1] == 0xD8) {
            return {true, "JPEG file is valid", {}, {}};
        }
        else {
            return {false, "File is not a valid JPEG", {}, {}};
        }
    }

    /**
     * @brief THis fuction needs to be further checked
     */
    file_handler::operation_result jpeg_processor_class::read_metadata() {
        file_handler::operation_result result;
        result.success = true;
        result.message = "Metadata successfully read";

        try {
            // Read EXIF metadata
            if (jpeg_image->exifData().count() > 0) {
                for (const auto& exif : jpeg_image->exifData()) {
                    std::string key = "EXIF." + exif.key();
                    std::string value = exif.toString();
                    result.metadata[key] = value;
                }
            }

            // Read IPTC metadata
            if (jpeg_image->iptcData().count() > 0) {
                for (const auto& iptc : jpeg_image->iptcData()) {
                    std::string key = "IPTC." + iptc.key();
                    std::string value = iptc.toString();
                    result.metadata[key] = value;
                }
            }

            // Read XMP metadata
            if (jpeg_image->xmpData().count() > 0) {
                for (const auto& xmp : jpeg_image->xmpData()) {
                    std::string key = "XMP." + xmp.key();
                    std::string value = xmp.toString();
                    result.metadata[key] = value;
                }
            }

            // Add summary information
            result.metadata["Total.EXIF"] = std::to_string(jpeg_image->exifData().count());
            result.metadata["Total.IPTC"] = std::to_string(jpeg_image->iptcData().count());
            result.metadata["Total.XMP"] = std::to_string(jpeg_image->xmpData().count());

        } catch (const Exiv2::Error& e) {
            result.success = false;
            result.message = "Failed to read metadata: " + std::string(e.what());
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Exception: " + std::string(e.what());
        }

        return result;
    }

    /**
     * @brief THis fuction needs to be further checked
     */
    file_handler::operation_result jpeg_processor_class::clean_metadata() {
        file_handler::operation_result result;
        result.success = true;
        result.message = "Metadata successfully cleaned";

        try {
            // Clear all metadata
            jpeg_image->clearExifData();
            jpeg_image->clearIptcData();
            jpeg_image->clearXmpData();
            jpeg_image->clearComment();

            // Write changes back to file
            jpeg_image->writeMetadata();

        } catch (const Exiv2::Error& e) {
            result.success = false;
            result.message = "Failed to clean metadata: " + std::string(e.what());
            std::cerr << "Exiv2 error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Exception: " + std::string(e.what());
            std::cerr << "Standard exception: " << e.what() << std::endl;
        }

        return result;
    }

    file_handler::operation_result jpeg_processor_class::overwrite_metadata() {
        file_handler::operation_result result;
        result.success = true;
        result.message = "Metadata successfully overwritten";

        try {
            // First clear all existing metadata
            jpeg_image->clearExifData();
            jpeg_image->clearIptcData();
            jpeg_image->clearXmpData();
            jpeg_image->clearComment();

            // Create new Exif data container
            Exiv2::ExifData exif_data;
            Exiv2::IptcData iptc_data;
            Exiv2::XmpData xmp_data;

            // Set metadata values from options
            auto& metadata = options.overwrite_metadata;

            // Process metadata entries and add to appropriate containers
            for (const auto& [key, value] : metadata) {
                if (key.empty() || value.empty()) {
                    continue;
                }

                // 确定元数据类型并添加到相应容器
                if (key.substr(0, 5) == "EXIF.") {
                    std::string exif_key = key.substr(5);
                    // 使用[]操作符添加或修改元数据
                    exif_data[exif_key] = value;
                }
                else if (key.substr(0, 5) == "IPTC.") {
                    std::string iptc_key = key.substr(5);
                    iptc_data[iptc_key] = value;
                }
                else if (key.substr(0, 4) == "XMP.") {
                    std::string xmp_key = key.substr(4);
                    xmp_data[xmp_key] = value;
                }
                // 处理常见元数据字段
                else if (key == "Title" || key == "Description" || key == "Author" ||
                         key == "Copyright" || key == "DateCreated" || key == "Software") {
                    // 将常见字段映射到相应的EXIF/XMP标签
                    if (key == "Title") {
                        exif_data["Exif.Image.ImageDescription"] = value;
                        xmp_data["Xmp.dc.title"] = value;
                    }
                    else if (key == "Author") {
                        exif_data["Exif.Image.Artist"] = value;
                        xmp_data["Xmp.dc.creator"] = value;
                    }
                    else if (key == "Copyright") {
                        exif_data["Exif.Image.Copyright"] = value;
                        xmp_data["Xmp.dc.rights"] = value;
                    }
                    else if (key == "DateCreated") {
                        exif_data["Exif.Image.DateTime"] = value;
                    }
                    else if (key == "Software") {
                        exif_data["Exif.Image.Software"] = value;
                    }
                }
            }

            // Set the metadata to the image
            jpeg_image->setExifData(exif_data);
            jpeg_image->setIptcData(iptc_data);
            jpeg_image->setXmpData(xmp_data);

            // Write changes back to file
            jpeg_image->writeMetadata();

        } catch (const Exiv2::Error& e) {
            result.success = false;
            result.message = "Failed to overwrite metadata: " + std::string(e.what());
            std::cerr << "Exiv2 error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Exception: " + std::string(e.what());
            std::cerr << "Standard exception: " << e.what() << std::endl;
        }

        return result;
    }
    file_handler::operation_result jpeg_processor_class::export_metadata() {
        // First read the metadata
        auto result = read_metadata();
        if (!result.success) {
            return result;
        }

        try {
            // Create output directory if needed
            if (!options.output_directory.empty() && !std::filesystem::exists(options.output_directory)) {
                std::filesystem::create_directories(options.output_directory);
            }

            // Create output file path
            std::filesystem::path output_path;
            if (options.output_directory.empty()) {
                output_path = std::filesystem::path(file_path).parent_path() /
                             (std::filesystem::path(file_path).stem().string() + "_metadata.json");
            } else {
                output_path = options.output_directory /
                             (std::filesystem::path(file_path).stem().string() + "_metadata.json");
            }

            // Write to output file
            std::ofstream out(output_path);
            if (!out) {
                result.success = false;
                result.message = "Failed to create output file: " + output_path.string();
                return result;
            }

            // Write in JSON format
            out << "{\n";
            bool first = true;
            for (const auto& [key, value] : result.metadata) {
                if (!first) out << ",\n";
                // Escape JSON special characters in the value
                std::string escaped_value = value;
                size_t pos = 0;
                while ((pos = escaped_value.find("\"", pos)) != std::string::npos) {
                    escaped_value.replace(pos, 1, "\\\"");
                    pos += 2;
                }
                out << "  \"" << key << "\": \"" << escaped_value << "\"";
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
    file_handler::operation_result jpeg_processor_class::restore_metadata() {
        // Functionality to restore metadata from backup or export file
        file_handler::operation_result result;
        result.success = false;
        result.message = "Restore operation not implemented for JPEG";

        return result;
    }


}