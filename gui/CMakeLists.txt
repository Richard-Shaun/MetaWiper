set(GUI_NAME meta_wiper_gui)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_PREFIX_PATH "C:/Softwares/Qt/6.9.0/msvc2022_64;${CMAKE_PREFIX_PATH}")

# 确保项目包含Qt自动MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置Qt模块
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick QuickControls2)

# 添加QML资源
qt_add_resources(QML_RESOURCES resources/resources.qrc)

# 设置应用程序源文件
set(SOURCE_FILES
    src/main.cpp
    src/application.cpp
    src/application.h
    src/viewmodels/filelistmodel.cpp
    src/viewmodels/metadatamodel.cpp
    src/viewmodels/mainviewmodel.cpp
    include/viewmodels/filelistmodel.h
    include/viewmodels/metadatamodel.h
    include/viewmodels/mainviewmodel.h
)

# 添加可执行文件
add_executable(${GUI_NAME} ${SOURCE_FILES} ${QML_RESOURCES})

# 包含目录
target_include_directories(${GUI_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 链接库
target_link_libraries(${GUI_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    meta_wiper_core
)

# 设置C++标准
set_target_properties(${GUI_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# find all dll paths
find_package(podofo CONFIG REQUIRED)
get_target_property(PODOFO_DLL_DEBUG podofo_shared IMPORTED_LOCATION_DEBUG)
get_filename_component(PODOFO_DLL_DIR ${PODOFO_DLL_DEBUG} DIRECTORY)

find_package(exiv2 CONFIG REQUIRED)
get_target_property(EXIV2_DLL_DEBUG exiv2lib IMPORTED_LOCATION_DEBUG)
get_filename_component(EXIV2_DLL_DIR ${EXIV2_DLL_DEBUG} DIRECTORY)

# copy all dlls to the test directory
add_custom_command(TARGET ${GUI_NAME} POST_BUILD
    # copy core dll
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:meta_wiper_core>
        $<TARGET_FILE_DIR:${GUI_NAME}>

    # copy podofo dll and its dependencies
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PODOFO_DLL_DIR}
        $<TARGET_FILE_DIR:${GUI_NAME}>

    # copy exiv2 dll and its dependencies
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${EXIV2_DLL_DIR}
        $<TARGET_FILE_DIR:${GUI_NAME}>

    COMMENT "Copying all required DLLs to test directory"
)


# 构建后复制QML文件 - 使用正确的路径
add_custom_command(TARGET meta_wiper_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/qml
            ${CMAKE_CURRENT_BINARY_DIR}/qml
)

# Qt部署 - 使用正确的QML目录路径
if(WIN32)
    find_program(QT_DEPLOY_EXECUTABLE windeployqt HINTS "${Qt6_DIR}/../../../bin")
    add_custom_command(TARGET meta_wiper_gui POST_BUILD
        COMMAND ${QT_DEPLOY_EXECUTABLE}
                --qmldir ${CMAKE_CURRENT_SOURCE_DIR}/resources/qml
                --no-translations
                --no-system-d3d-compiler
                $<TARGET_FILE:meta_wiper_gui>
    )
endif()

# 复制核心库DLL
add_custom_command(TARGET meta_wiper_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:meta_wiper_core>
            $<TARGET_FILE_DIR:meta_wiper_gui>
)

