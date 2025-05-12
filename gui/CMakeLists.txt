set(GUI_NAME meta_wiper_gui)


set(CMAKE_PREFIX_PATH "C:/Softwares/Qt/6.9.0/msvc2022_64;${CMAKE_PREFIX_PATH}")

# ensure the project contains Qt auto MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# set up Qt modules
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick QuickControls2)

# add all qml resources
qt_add_resources(QML_RESOURCES resources/resources.qrc)

# set up the version header file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include_autogen/version.h
)

# set application source files
set(SOURCE_FILES
    src/main.cpp
    src/application.cpp
    src/viewmodels/filelistmodel.cpp
    src/viewmodels/metadatamodel.cpp
    src/viewmodels/mainviewmodel.cpp
    include/application.h
    include/viewmodels/filelistmodel.h
    include/viewmodels/metadatamodel.h
    include/viewmodels/mainviewmodel.h
)

add_executable(${GUI_NAME} ${SOURCE_FILES} ${QML_RESOURCES})

target_include_directories(${GUI_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include_autogen
)

target_link_libraries(${GUI_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    meta_wiper_core
)

# find all dll paths
find_package(podofo CONFIG REQUIRED)
get_target_property(PODOFO_DLL_DEBUG podofo_shared IMPORTED_LOCATION_DEBUG)
get_filename_component(PODOFO_DLL_DIR ${PODOFO_DLL_DEBUG} DIRECTORY)

find_package(exiv2 CONFIG REQUIRED)
get_target_property(EXIV2_DLL_DEBUG exiv2lib IMPORTED_LOCATION_DEBUG)
get_filename_component(EXIV2_DLL_DIR ${EXIV2_DLL_DEBUG} DIRECTORY)

find_package(pugixml CONFIG REQUIRED)
get_target_property(PUGIXML_DLL_DEBUG pugixml::shared IMPORTED_LOCATION_DEBUG)
get_filename_component(PUGIXML_DLL_DIR ${PUGIXML_DLL_DEBUG} DIRECTORY)

find_package(libzip CONFIG REQUIRED)
get_target_property(LIBZIP_DLL_DEBUG libzip::zip IMPORTED_LOCATION_DEBUG)
get_filename_component(LIBZIP_DLL_DIR ${LIBZIP_DLL_DEBUG} DIRECTORY)

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

    # copy pugixml dll and its dependencies
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PUGIXML_DLL_DIR}
        $<TARGET_FILE_DIR:${GUI_NAME}>

    # copy libzip dll and its dependencies
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${LIBZIP_DLL_DIR}
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

# 添加安装规则
install(TARGETS ${GUI_NAME}
    RUNTIME DESTINATION bin
    COMPONENT applications
)

# 安装QML文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/qml
    DESTINATION bin/qml
    COMPONENT applications
)

# 安装图标文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons
    DESTINATION bin/icons
    COMPONENT applications
)

# 安装依赖的core库
install(TARGETS meta_wiper_core
    RUNTIME DESTINATION bin
    COMPONENT libraries
)

# 为Windows设置安装依赖库的规则
if(WIN32)
    # 创建一个脚本在构建后收集所有需要的DLL
    set(COLLECT_DEPENDENCIES_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/collect_dependencies.cmake")
    file(WRITE ${COLLECT_DEPENDENCIES_SCRIPT} "
        # 收集所有需要安装的DLL
        file(GLOB QT_DLLS \"$<TARGET_FILE_DIR:${GUI_NAME}>/*.dll\")
        file(GLOB QT_PLATFORM_DLLS \"$<TARGET_FILE_DIR:${GUI_NAME}>/platforms/*.dll\")
        file(GLOB QT_STYLES_DLLS \"$<TARGET_FILE_DIR:${GUI_NAME}>/styles/*.dll\")
        file(GLOB QT_IMAGEFORMATS_DLLS \"$<TARGET_FILE_DIR:${GUI_NAME}>/imageformats/*.dll\")
        file(GLOB QT_QML_DLLS \"$<TARGET_FILE_DIR:${GUI_NAME}>/QtQml/*.dll\")
        file(GLOB QT_QUICK_DLLS \"$<TARGET_FILE_DIR:${GUI_NAME}>/QtQuick/*.dll\")
        file(GLOB QT_QUICK_CONTROLS_DLLS \"$<TARGET_FILE_DIR:${GUI_NAME}>/QtQuick/Controls/*.dll\")

        # 将所有DLL写入文件列表
        file(WRITE \"${CMAKE_CURRENT_BINARY_DIR}/dll_list.txt\" \"\")
        foreach(DLL_FILE \${QT_DLLS})
            file(APPEND \"${CMAKE_CURRENT_BINARY_DIR}/dll_list.txt\" \"\${DLL_FILE}\\n\")
        endforeach()

        # 创建子目录列表
        set(SUBDIRS platforms styles imageformats QtQml QtQuick \"QtQuick/Controls\")
        foreach(SUBDIR \${SUBDIRS})
            file(MAKE_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}/collected_dlls/\${SUBDIR}\")
        endforeach()

        # 复制所有DLL到临时目录
        foreach(DLL_FILE \${QT_DLLS})
            file(COPY \${DLL_FILE} DESTINATION \"${CMAKE_CURRENT_BINARY_DIR}/collected_dlls\")
        endforeach()

        # 复制平台DLL
        foreach(DLL_FILE \${QT_PLATFORM_DLLS})
            file(COPY \${DLL_FILE} DESTINATION \"${CMAKE_CURRENT_BINARY_DIR}/collected_dlls/platforms\")
        endforeach()

        # 复制样式DLL
        foreach(DLL_FILE \${QT_STYLES_DLLS})
            file(COPY \${DLL_FILE} DESTINATION \"${CMAKE_CURRENT_BINARY_DIR}/collected_dlls/styles\")
        endforeach()

        # 复制图像格式DLL
        foreach(DLL_FILE \${QT_IMAGEFORMATS_DLLS})
            file(COPY \${DLL_FILE} DESTINATION \"${CMAKE_CURRENT_BINARY_DIR}/collected_dlls/imageformats\")
        endforeach()

        # 复制QML DLL
        foreach(DLL_FILE \${QT_QML_DLLS})
            file(COPY \${DLL_FILE} DESTINATION \"${CMAKE_CURRENT_BINARY_DIR}/collected_dlls/QtQml\")
        endforeach()

        # 复制Quick DLL
        foreach(DLL_FILE \${QT_QUICK_DLLS})
            file(COPY \${DLL_FILE} DESTINATION \"${CMAKE_CURRENT_BINARY_DIR}/collected_dlls/QtQuick\")
        endforeach()

        # 复制Quick Controls DLL
        foreach(DLL_FILE \${QT_QUICK_CONTROLS_DLLS})
            file(COPY \${DLL_FILE} DESTINATION \"${CMAKE_CURRENT_BINARY_DIR}/collected_dlls/QtQuick/Controls\")
        endforeach()
    ")

    # 在构建后执行收集依赖的脚本
    add_custom_command(TARGET ${GUI_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -P ${COLLECT_DEPENDENCIES_SCRIPT}
        COMMENT "Collecting dependencies for installation package"
    )

    # 安装收集到的DLL
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/collected_dlls/
        DESTINATION bin
        COMPONENT libraries
    )

    # 安装第三方库
    install(FILES
        ${PODOFO_DLL_DEBUG}
        ${EXIV2_DLL_DEBUG}
        ${PUGIXML_DLL_DEBUG}
        ${LIBZIP_DLL_DEBUG}
        DESTINATION bin
        COMPONENT libraries
    )
endif()