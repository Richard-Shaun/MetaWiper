set(TEST_NAME meta_wiper_test)

add_subdirectory(common)

add_subdirectory(formats/pdf)
add_subdirectory(formats/jpeg)

add_executable(${TEST_NAME} main.cpp)

target_link_libraries(${TEST_NAME} PRIVATE
        meta_wiper_core
        test_utils
        pdf_tests
        jpeg_tests
)

target_compile_features(${TEST_NAME} PRIVATE cxx_std_17)

# Add directory for test files as a resource
add_custom_command(TARGET ${TEST_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${TEST_NAME}>/test_files
    COMMENT "Creating test files directory"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(${TEST_NAME} PRIVATE /W4)
endif()

# find all dll paths
find_package(podofo CONFIG REQUIRED)
get_target_property(PODOFO_DLL_DEBUG podofo_shared IMPORTED_LOCATION_DEBUG)
get_filename_component(PODOFO_DLL_DIR ${PODOFO_DLL_DEBUG} DIRECTORY)

find_package(exiv2 CONFIG REQUIRED)
get_target_property(EXIV2_DLL_DEBUG exiv2lib IMPORTED_LOCATION_DEBUG)
get_filename_component(EXIV2_DLL_DIR ${EXIV2_DLL_DEBUG} DIRECTORY)

# copy all dlls to the test directory
add_custom_command(TARGET ${TEST_NAME} POST_BUILD
    # copy core dll
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:meta_wiper_core>
        $<TARGET_FILE_DIR:${TEST_NAME}>

    # copy podofo dll and its dependencies
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PODOFO_DLL_DIR}
        $<TARGET_FILE_DIR:${TEST_NAME}>

    # copy exiv2 dll and its dependencies
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${EXIV2_DLL_DIR}
        $<TARGET_FILE_DIR:${TEST_NAME}>

    COMMENT "Copying all required DLLs to test directory"
)