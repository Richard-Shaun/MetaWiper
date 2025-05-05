set(LIB_NAME meta_wiper_core)

find_package(PoDoFo REQUIRED)

set(CORE_SOURCES
    # api codes
    src/meta_wiper_core.cpp
    # base codes
    src/base/file_handler.cpp
    src/base/file_properties.cpp
    src/base/processor_factory.cpp
    # processors codes
    src/processors/pdf_processor.cpp
    src/processors/jpeg_processor.cpp
)
set (CORE_HEADERS
    # api headers
    include/meta_wiper_core.h
    # base headers
    include/base/file_handler.h
    include/base/file_properties.h
    include/base/processor_factory.h
    # processors headers
    include/processors/pdf_processor.h
    include/processors/jpeg_processor.h
)

add_library(${LIB_NAME} SHARED ${CORE_SOURCES} ${CORE_HEADERS})

target_compile_definitions(${LIB_NAME} PRIVATE META_WIPER_CORE_EXPORTS)

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)

target_include_directories(${LIB_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


find_package(podofo CONFIG REQUIRED)
target_link_libraries(${LIB_NAME} PRIVATE $<IF:$<TARGET_EXISTS:podofo_shared>,podofo_shared,podofo_static>)

find_package(exiv2 CONFIG REQUIRED)
target_link_libraries(${LIB_NAME} PRIVATE Exiv2::exiv2lib)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra)
elseif(MSVC)
    target_compile_options(${LIB_NAME} PRIVATE /W4)
endif()

# 创建别名
add_library(MetaWiper::Core ALIAS ${LIB_NAME})

#[==[
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MetaWiperCoreConfigVersion.cmake"
    VERSION 0.1.0
    COMPATIBILITY SameMajorVersion
)
#]==]

#[==[
# 设置安装规则
include(GNUInstallDirs)

# 安装库文件
install(TARGETS ${LIB_NAME}
    EXPORT MetaWiperCoreTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 安装头文件
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 导出目标
install(EXPORT MetaWiperCoreTargets
    FILE MetaWiperCoreTargets.cmake
    NAMESPACE MetaWiper::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MetaWiperCore
)

# 生成配置文件
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MetaWiperCoreConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MetaWiperCoreConfig.cmake
    @ONLY
)

# 安装配置文件
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MetaWiperCoreConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MetaWiperCoreConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MetaWiperCore
)
#]==]