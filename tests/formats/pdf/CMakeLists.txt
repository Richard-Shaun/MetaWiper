add_library(pdf_tests STATIC
    pdf_test.cpp
)

target_link_libraries(pdf_tests
    PRIVATE
    meta_wiper_core
    test_utils
)