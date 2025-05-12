cmake_minimum_required(VERSION 3.15)
project(meta_wiper VERSION 0.1.0)

# set variables for version, unify the version number in the whole project
set(META_WIPER_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(META_WIPER_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(META_WIPER_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(META_WIPER_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(core)
add_subdirectory(tests)
add_subdirectory(gui)


# 添加安装规则
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)

# 只在构建GUI时添加打包配置
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # 设置基本CPack变量
    set(CPACK_PACKAGE_NAME "MetaWiper")
    set(CPACK_PACKAGE_VENDOR "Your Company")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Meta Wiper - A tool to clean metadata from files")
    set(CPACK_PACKAGE_VERSION ${META_WIPER_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${META_WIPER_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${META_WIPER_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${META_WIPER_VERSION_PATCH})
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "MetaWiper")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    # 设置NSIS特定选项
    set(CPACK_GENERATOR "NSIS")
    # can change to custom logos latter on
    # set(CPACK_NSIS_MUI_ICON "${NSISDIR}/Contrib/Graphics/Icons/modern-install.ico")
    # set(CPACK_NSIS_MUI_UNIICON "${NSISDIR}/Contrib/Graphics/Icons/modern-uninstall.ico")
    set(CPACK_NSIS_PACKAGE_NAME "MetaWiper ${META_WIPER_VERSION}")
    set(CPACK_NSIS_DISPLAY_NAME "MetaWiper ${META_WIPER_VERSION}")
    set(CPACK_NSIS_HELP_LINK "https://github.com/yourusername/meta_wiper")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/yourusername/meta_wiper")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")

    # 包含独立的CPack配置
    if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/CPackConfig.cmake")
        include("${CMAKE_SOURCE_DIR}/cmake/CPackConfig.cmake")
    endif()

    # 包含CPack模块
    include(CPack)
endif()