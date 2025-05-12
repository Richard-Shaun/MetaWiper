/**
 * @file docx_processor.cpp
 * @brief Implementation of DOCX processor
 */
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <zip.h>
#include "./processors/docx_processor.h"

namespace docx_processor {

    docx_processor_class::docx_processor_class(const std::string& path,
                                              file_handler::operation_type type,
                                              const file_handler::operation_options& opts)
            : file_handler_class(path, type, opts), docx_loaded(false) {
        try {
            // Initialize XML documents
            core_xml = std::make_unique<pugi::xml_document>();
            app_xml = std::make_unique<pugi::xml_document>();

            // Extract core.xml and app.xml from DOCX
            std::string core_content;
            std::string app_content;

            if (extract_xml_file("docProps/core.xml", core_content) &&
                extract_xml_file("docProps/app.xml", app_content)) {

                // Parse XML content
                pugi::xml_parse_result core_result = core_xml->load_string(core_content.c_str());
                pugi::xml_parse_result app_result = app_xml->load_string(app_content.c_str());

                if (core_result && app_result) {
                    docx_loaded = true;
                } else {
                    std::cerr << "Failed to parse XML: "
                              << (core_result ? "" : core_result.description())
                              << (app_result ? "" : app_result.description())
                              << std::endl;
                }
            } else {
                std::cerr << "Failed to extract XML files from DOCX" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in docx_processor constructor: " << e.what() << std::endl;
        }
    }

    docx_processor_class::~docx_processor_class() = default;

    file_handler::operation_result docx_processor_class::check_prerequisites() {
        // Check if file exists and has correct header for ZIP/DOCX
        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            return {false, "Failed to open file", {}, {}};
        }

        // Check if it's a ZIP file (PK signature)
        char header[4];
        file.read(header, 4);
        file.close();

        if (file.good() && header[0] == 'P' && header[1] == 'K' &&
            header[2] == 0x03 && header[3] == 0x04) {

            // Try to extract a DOCX-specific file to validate
            std::string content;
            if (extract_xml_file("docProps/core.xml", content)) {
                return {true, "DOCX file is valid", {}, {}};
            }
            return {false, "File is a ZIP but not a valid DOCX", {}, {}};
        }

        return {false, "File is not a valid DOCX", {}, {}};
    }

    file_handler::operation_result docx_processor_class::read_metadata() {
        file_handler::operation_result result;
        result.success = true;
        result.message = "Metadata successfully read";

        try {
            if (!docx_loaded) {
                result.success = false;
                result.message = "DOCX document not properly loaded";
                return result;
            }

            // Read core properties
            pugi::xml_node core_props = core_xml->document_element();
            if (core_props) {
                for (pugi::xml_node node : core_props.children()) {
                    std::string key = node.name();
                    std::string value = node.text().get();
                    if (!key.empty() && !value.empty()) {
                        result.metadata["Core." + key] = value;
                    }
                }
            }

            // Read app properties
            pugi::xml_node app_props = app_xml->document_element();
            if (app_props) {
                for (pugi::xml_node node : app_props.children()) {
                    std::string key = node.name();
                    std::string value = node.text().get();
                    if (!key.empty() && !value.empty()) {
                        result.metadata["App." + key] = value;
                    }
                }
            }

            // Add summary counts
            if (core_props) {
                int core_count = 0;
                for (pugi::xml_node node : core_props.children()) {
                    if (!std::string(node.name()).empty()) {
                        core_count++;
                    }
                }
                result.metadata["Total.Core"] = std::to_string(core_count);
            }

            if (app_props) {
                int app_count = 0;
                for (pugi::xml_node node : app_props.children()) {
                    if (!std::string(node.name()).empty()) {
                        app_count++;
                    }
                }
                result.metadata["Total.App"] = std::to_string(app_count);
            }

        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Exception while reading metadata: " + std::string(e.what());
        }

        return result;
    }

    file_handler::operation_result docx_processor_class::clean_metadata() {
        file_handler::operation_result result;
        result.success = true;
        result.message = "Metadata successfully cleaned";

        try {
            if (!docx_loaded) {
                result.success = false;
                result.message = "DOCX document not properly loaded";
                return result;
            }

            // Create clean core.xml with minimal required structure
            pugi::xml_document clean_core_xml;
            pugi::xml_node core_decl = clean_core_xml.append_child(pugi::node_declaration);
            core_decl.append_attribute("version") = "1.0";
            core_decl.append_attribute("encoding") = "UTF-8";
            core_decl.append_attribute("standalone") = "yes";

            pugi::xml_node core_props = clean_core_xml.append_child("cp:coreProperties");
            // Add necessary namespaces
            core_props.append_attribute("xmlns:cp") = "http://schemas.openxmlformats.org/package/2006/metadata/core-properties";
            core_props.append_attribute("xmlns:dc") = "http://purl.org/dc/elements/1.1/";
            core_props.append_attribute("xmlns:dcterms") = "http://purl.org/dc/terms/";
            core_props.append_attribute("xmlns:dcmitype") = "http://purl.org/dc/dcmitype/";
            core_props.append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";

            // Create clean app.xml with minimal required structure
            pugi::xml_document clean_app_xml;
            pugi::xml_node app_decl = clean_app_xml.append_child(pugi::node_declaration);
            app_decl.append_attribute("version") = "1.0";
            app_decl.append_attribute("encoding") = "UTF-8";
            app_decl.append_attribute("standalone") = "yes";

            pugi::xml_node app_props = clean_app_xml.append_child("Properties");
            app_props.append_attribute("xmlns") = "http://schemas.openxmlformats.org/officeDocument/2006/extended-properties";
            app_props.append_attribute("xmlns:vt") = "http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes";

            // Save to strings
            std::stringstream core_stream;
            clean_core_xml.save(core_stream);
            std::string core_content = core_stream.str();

            std::stringstream app_stream;
            clean_app_xml.save(app_stream);
            std::string app_content = app_stream.str();

            // Update files in the DOCX
            if (!update_xml_file("docProps/core.xml", core_content) ||
                !update_xml_file("docProps/app.xml", app_content)) {
                result.success = false;
                result.message = "Failed to update XML files in DOCX";
                return result;
            }

            // Reload the documents
            core_xml = std::make_unique<pugi::xml_document>();
            app_xml = std::make_unique<pugi::xml_document>();

            core_xml->load_string(core_content.c_str());
            app_xml->load_string(app_content.c_str());

        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Exception while cleaning metadata: " + std::string(e.what());
        }

        return result;
    }

    file_handler::operation_result docx_processor_class::overwrite_metadata() {
        file_handler::operation_result result;
        result.success = true;
        result.message = "Metadata successfully overwritten";

        try {
            if (!docx_loaded) {
                result.success = false;
                result.message = "DOCX document not properly loaded";
                return result;
            }

            // Create new XML documents with clean structure
            pugi::xml_document new_core_xml;
            pugi::xml_node core_decl = new_core_xml.append_child(pugi::node_declaration);
            core_decl.append_attribute("version") = "1.0";
            core_decl.append_attribute("encoding") = "UTF-8";
            core_decl.append_attribute("standalone") = "yes";

            pugi::xml_node core_props = new_core_xml.append_child("cp:coreProperties");
            core_props.append_attribute("xmlns:cp") = "http://schemas.openxmlformats.org/package/2006/metadata/core-properties";
            core_props.append_attribute("xmlns:dc") = "http://purl.org/dc/elements/1.1/";
            core_props.append_attribute("xmlns:dcterms") = "http://purl.org/dc/terms/";
            core_props.append_attribute("xmlns:dcmitype") = "http://purl.org/dc/dcmitype/";
            core_props.append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";

            pugi::xml_document new_app_xml;
            pugi::xml_node app_decl = new_app_xml.append_child(pugi::node_declaration);
            app_decl.append_attribute("version") = "1.0";
            app_decl.append_attribute("encoding") = "UTF-8";
            app_decl.append_attribute("standalone") = "yes";

            pugi::xml_node app_props = new_app_xml.append_child("Properties");
            app_props.append_attribute("xmlns") = "http://schemas.openxmlformats.org/officeDocument/2006/extended-properties";
            app_props.append_attribute("xmlns:vt") = "http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes";

            // Add metadata from options
            auto& metadata = options.overwrite_metadata;
            for (const auto& [key, value] : metadata) {
                if (key.empty() || value.empty()) {
                    continue;
                }

                // Handle common metadata fields
                if (key == "Title") {
                    core_props.append_child("dc:title").text().set(value.c_str());
                }
                else if (key == "Subject") {
                    core_props.append_child("dc:subject").text().set(value.c_str());
                }
                else if (key == "Author" || key == "Creator") {
                    core_props.append_child("dc:creator").text().set(value.c_str());
                }
                else if (key == "Description") {
                    core_props.append_child("dc:description").text().set(value.c_str());
                }
                else if (key == "Keywords") {
                    core_props.append_child("cp:keywords").text().set(value.c_str());
                }
                else if (key == "Category") {
                    core_props.append_child("cp:category").text().set(value.c_str());
                }
                else if (key == "LastModifiedBy") {
                    core_props.append_child("cp:lastModifiedBy").text().set(value.c_str());
                }
                // Application specific properties
                else if (key == "Application") {
                    app_props.append_child("Application").text().set(value.c_str());
                }
                else if (key == "Company") {
                    app_props.append_child("Company").text().set(value.c_str());
                }
                else if (key == "Manager") {
                    app_props.append_child("Manager").text().set(value.c_str());
                }
                // Handle prefixed properties
                else if (key.substr(0, 5) == "Core.") {
                    std::string node_name = key.substr(5);
                    core_props.append_child(node_name.c_str()).text().set(value.c_str());
                }
                else if (key.substr(0, 4) == "App.") {
                    std::string node_name = key.substr(4);
                    app_props.append_child(node_name.c_str()).text().set(value.c_str());
                }
            }

            // Add creation and modification time
            auto now = std::time(nullptr);
            std::tm tm_now = *std::localtime(&now);
            char time_str[64];
            std::strftime(time_str, sizeof(time_str), "%Y-%m-%dT%H:%M:%SZ", &tm_now);

            // Only add times if not already set
            if (!core_props.child("dcterms:created")) {
                pugi::xml_node created = core_props.append_child("dcterms:created");
                created.text().set(time_str);
                created.append_attribute("xsi:type") = "dcterms:W3CDTF";
            }

            if (!core_props.child("dcterms:modified")) {
                pugi::xml_node modified = core_props.append_child("dcterms:modified");
                modified.text().set(time_str);
                modified.append_attribute("xsi:type") = "dcterms:W3CDTF";
            }

            // Save to strings
            std::stringstream core_stream;
            new_core_xml.save(core_stream);
            std::string core_content = core_stream.str();

            std::stringstream app_stream;
            new_app_xml.save(app_stream);
            std::string app_content = app_stream.str();

            // Update files in DOCX
            if (!update_xml_file("docProps/core.xml", core_content) ||
                !update_xml_file("docProps/app.xml", app_content)) {
                result.success = false;
                result.message = "Failed to update XML files in DOCX";
                return result;
            }

            // Update our in-memory documents
            core_xml = std::make_unique<pugi::xml_document>();
            app_xml = std::make_unique<pugi::xml_document>();

            core_xml->load_string(core_content.c_str());
            app_xml->load_string(app_content.c_str());

        } catch (const std::exception& e) {
            result.success = false;
            result.message = "Exception while overwriting metadata: " + std::string(e.what());
        }

        return result;
    }

    file_handler::operation_result docx_processor_class::export_metadata() {
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

    file_handler::operation_result docx_processor_class::restore_metadata() {
        // Restore operation not implemented
        file_handler::operation_result result;
        result.success = false;
        result.message = "Restore operation not implemented for DOCX";
        return result;
    }

    // Helper methods for ZIP operations
    bool docx_processor_class::extract_xml_file(const std::string& xml_path, std::string& content) {
        int err = 0;
        zip* archive = zip_open(file_path.c_str(), 0, &err);
        if (!archive) {
            std::cerr << "Failed to open ZIP archive, error code: " << err << std::endl;
            return false;
        }

        // Find the file in the archive
        zip_int64_t index = zip_name_locate(archive, xml_path.c_str(), 0);
        if (index < 0) {
            std::cerr << "XML file not found in archive: " << xml_path << std::endl;
            zip_close(archive);
            return false;
        }

        // Open the file in the archive
        zip_file* file = zip_fopen_index(archive, index, 0);
        if (!file) {
            std::cerr << "Failed to open file in archive: " << xml_path << std::endl;
            zip_close(archive);
            return false;
        }

        // Read the file content
        const int buffer_size = 8192;
        char buffer[buffer_size];
        zip_int64_t bytes_read;
        content.clear();

        while ((bytes_read = zip_fread(file, buffer, buffer_size)) > 0) {
            content.append(buffer, bytes_read);
        }

        // Close everything
        zip_fclose(file);
        zip_close(archive);

        return !content.empty();
    }

    bool docx_processor_class::update_xml_file(const std::string& xml_path, const std::string& content) {
        // Create a temporary directory
        std::string temp_dir = create_temp_directory();
        if (temp_dir.empty()) {
            return false;
        }

        try {
            // Extract all files
            int err = 0;
            zip* src_archive = zip_open(file_path.c_str(), 0, &err);
            if (!src_archive) {
                cleanup_temp_directory(temp_dir);
                return false;
            }

            zip_int64_t num_entries = zip_get_num_entries(src_archive, 0);
            for (zip_int64_t i = 0; i < num_entries; i++) {
                const char* name = zip_get_name(src_archive, i, 0);
                if (!name) continue;

                std::string entry_name(name);
                std::filesystem::path entry_path = std::filesystem::path(temp_dir) / entry_name;

                // Create parent directories
                std::filesystem::create_directories(entry_path.parent_path());

                // Skip the file we want to update - we'll create it separately
                if (entry_name == xml_path) {
                    continue;
                }

                // Extract the file
                zip_file* file = zip_fopen_index(src_archive, i, 0);
                if (!file) continue;

                std::ofstream out(entry_path, std::ios::binary);
                if (!out) {
                    zip_fclose(file);
                    continue;
                }

                const int buffer_size = 8192;
                char buffer[buffer_size];
                zip_int64_t bytes_read;

                while ((bytes_read = zip_fread(file, buffer, buffer_size)) > 0) {
                    out.write(buffer, bytes_read);
                }

                out.close();
                zip_fclose(file);
            }

            zip_close(src_archive);

            // Write the updated XML file
            std::filesystem::path xml_full_path = std::filesystem::path(temp_dir) / xml_path;
            std::filesystem::create_directories(xml_full_path.parent_path());

            std::ofstream xml_out(xml_full_path);
            if (!xml_out) {
                cleanup_temp_directory(temp_dir);
                return false;
            }

            xml_out << content;
            xml_out.close();

            // Create a new ZIP file
            std::filesystem::path temp_zip = std::filesystem::temp_directory_path() /
                                           (std::filesystem::path(file_path).filename().string() + ".tmp");

            // Use zip command line tool as a fallback
            std::string command = "cd \"" + temp_dir + "\" && zip -r \"" +
                                 temp_zip.string() + "\" .";

            int result = std::system(command.c_str());
            if (result != 0) {
                std::cerr << "Failed to create new ZIP file, command returned: " << result << std::endl;
                cleanup_temp_directory(temp_dir);
                return false;
            }

            // Replace the original file with the new one
            std::filesystem::rename(temp_zip, file_path);

            // Clean up
            cleanup_temp_directory(temp_dir);

            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Exception in update_xml_file: " << e.what() << std::endl;
            cleanup_temp_directory(temp_dir);
            return false;
        }
    }

    std::string docx_processor_class::create_temp_directory() {
        try {
            std::filesystem::path temp_dir = std::filesystem::temp_directory_path() /
                                           ("metawiper_" + std::to_string(std::hash<std::string>{}(file_path)));
            std::filesystem::create_directories(temp_dir);
            return temp_dir.string();
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to create temp directory: " << e.what() << std::endl;
            return "";
        }
    }

    void docx_processor_class::cleanup_temp_directory(const std::string& temp_dir) {
        try {
            std::filesystem::remove_all(temp_dir);
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to clean up temp directory: " << e.what() << std::endl;
        }
    }

} // namespace docx_processor