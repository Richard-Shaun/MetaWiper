add_library(jpeg_tests STATIC
    jpeg_test.cpp
)

target_link_libraries(jpeg_tests
    PRIVATE
    meta_wiper_core
    test_utils
)