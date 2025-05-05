#pragma once
#include <string>

namespace file_properties {

    enum class category {
        test_category,
        UNKNOWN,
        DOCUMENT,
        IMAGE,
        AUDIO,
        VIDEO,
        ARCHIVE
    };

    enum class type_major {
        test_type_major,
        UNKNOWN,
        PDF,
        WORD,
        EXCEL,
        POWERPOINT,
        JPEG,
        PNG,
        MP3,
        MP4
    };

    enum class type_minor {
        test_type_minor,
        UNKNOWN,
        PDFA,
        DOCX,
        XLSX,
        PPTX,
        JPEG
    };

    class file_properties_class {
    protected:
        std::string file_hash;
        std::string file_path;
        std::string file_name;
        std::string file_extension;
        category file_category {category::UNKNOWN};
        type_major file_type_major {type_major::UNKNOWN};
        type_minor file_type_minor {type_minor::UNKNOWN};
    public:
        explicit file_properties_class(std::string path);
        virtual ~file_properties_class();
        [[nodiscard]] const std::string& get_file_hash() const { return file_hash; }
        [[nodiscard]] const std::string& get_file_path() const { return file_path; }
        [[nodiscard]] const std::string& get_file_name() const { return file_name; }
        [[nodiscard]] const std::string& get_file_extension() const { return file_extension; }
        [[nodiscard]] category get_file_category() const { return file_category; }
        [[nodiscard]] type_major get_file_type_major() const { return file_type_major; }
        [[nodiscard]] type_minor get_file_type_minor() const { return file_type_minor; }
    protected:
        void init_file_hash();
        void init_file_name();
        void init_file_extension();
        void init_file_category();
        void init_file_type_major();
        void init_file_type_minor();
    };

}


