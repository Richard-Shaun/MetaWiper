#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include "./base/file_properties.h"

namespace file_properties {

file_properties_class::file_properties_class(std::string  path) : file_path(std::move(path)) {
    init_file_name();
    init_file_extension();
    init_file_type_major();
    init_file_category();
    init_file_type_minor();
    init_file_hash();
}

void file_properties_class::init_file_hash() {
    const std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        file_hash = "error"; // default hash value
        return;
    }

    std::ostringstream oss;
    oss << file.rdbuf(); // read the file content into a string stream
    const std::string file_content = oss.str();

    // use std::hash to generate a hash value
    constexpr std::hash<std::string> hasher;
    file_hash = std::to_string(hasher(file_content));
}

void file_properties_class::init_file_name()  {
    if (const size_t last_slash = file_path.find_last_of("/\\"); last_slash != std::string::npos) {
        file_name = file_path.substr(last_slash + 1);
    } else {
        file_name = file_path;
    }
}

void file_properties_class::init_file_extension() {
    if (const size_t last_dot = file_name.find_last_of('.'); last_dot != std::string::npos) {
        file_extension = file_name.substr(last_dot + 1);
        std::transform(file_extension.begin(), file_extension.end(), file_extension.begin(), ::tolower);
    } else {
        file_extension = "null";
    }
}

void file_properties_class::init_file_category() {
    if (file_extension == "pdf" || file_extension == "doc" || file_extension == "docx" ||
        file_extension == "xls" || file_extension == "xlsx" || file_extension == "ppt" ||
        file_extension == "pptx" || file_extension == "txt") {
        file_category = category::DOCUMENT;
    } else if (file_extension == "jpg" || file_extension == "jpeg" || file_extension == "png" ||
               file_extension == "gif" || file_extension == "bmp" || file_extension == "tiff") {
        file_category = category::IMAGE;
    } else if (file_extension == "mp3" || file_extension == "wav" || file_extension == "ogg" ||
               file_extension == "flac") {
        file_category = category::AUDIO;
    } else if (file_extension == "mp4" || file_extension == "avi" || file_extension == "mkv" ||
               file_extension == "mov") {
        file_category = category::VIDEO;
    } else if (file_extension == "zip" || file_extension == "rar" || file_extension == "7z" ||
               file_extension == "tar" || file_extension == "gz") {
        file_category = category::ARCHIVE;
    } else {
        file_category = category::UNKNOWN;
    }
}

void file_properties_class::init_file_type_major() {
    if (file_extension == "pdf") {
        file_type_major = type_major::PDF;
    } else if (file_extension == "doc" || file_extension == "docx") {
        file_type_major = type_major::WORD;
    } else if (file_extension == "xls" || file_extension == "xlsx") {
        file_type_major = type_major::EXCEL;
    } else if (file_extension == "ppt" || file_extension == "pptx") {
        file_type_major = type_major::POWERPOINT;
    } else if (file_extension == "jpg" || file_extension == "jpeg") {
        file_type_major = type_major::JPEG;
    } else if (file_extension == "png") {
        file_type_major = type_major::PNG;
    } else if (file_extension == "mp3") {
        file_type_major = type_major::MP3;
    } else if (file_extension == "mp4") {
        file_type_major = type_major::MP4;
    } else {
        file_type_major = type_major::UNKNOWN;
    }
}
void file_properties_class::init_file_type_minor() {
    if (file_extension == "pdf") {
        // 需要实际分析PDF内容来确定是否是PDF/A
        // to be fixed
        file_type_minor = type_minor::PDFA;
    } else if (file_extension == "docx") {
        file_type_minor = type_minor::DOCX;
    } else if (file_extension == "xlsx") {
        file_type_minor = type_minor::XLSX;
    } else if (file_extension == "pptx") {
        file_type_minor = type_minor::PPTX;
    } else {
        file_type_minor = type_minor::UNKNOWN;
    }
}

}