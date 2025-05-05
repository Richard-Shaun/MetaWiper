add_library(test_utils STATIC
    test_utils.cpp
    test_utils.h
)

target_include_directories(test_utils
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(test_utils
    PRIVATE meta_wiper_core
)