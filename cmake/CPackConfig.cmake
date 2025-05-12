# 这个文件包含特定于CPack的配置

# 设置组件
set(CPACK_COMPONENTS_ALL applications libraries)

# 定义组件组
set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set(CPACK_COMPONENT_LIBRARIES_GROUP "Runtime")

# 组信息
set(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION "Meta Wiper应用程序及其所需的所有文件")

# 组件描述
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Meta Wiper应用程序")
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "Meta Wiper图形用户界面应用程序")
set(CPACK_COMPONENT_APPLICATIONS_DEPENDS libraries)

set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "运行时库")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "应用程序需要的库文件")
set(CPACK_COMPONENT_LIBRARIES_REQUIRED ON)

# NSIS特定配置
set(CPACK_NSIS_CREATE_ICONS_EXTRA
    "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Meta Wiper.lnk' '$INSTDIR\\\\bin\\\\meta_wiper_gui.exe'"
)
set(CPACK_NSIS_DELETE_ICONS_EXTRA
    "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Meta Wiper.lnk'"
)

# 桌面快捷方式
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
    "CreateShortCut '$DESKTOP\\\\Meta Wiper.lnk' '$INSTDIR\\\\bin\\\\meta_wiper_gui.exe'"
)
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
    "Delete '$DESKTOP\\\\Meta Wiper.lnk'"
)

# 安装程序自定义消息
set(CPACK_NSIS_WELCOME_TITLE "欢迎安装Meta Wiper")
set(CPACK_NSIS_FINISH_TITLE "Meta Wiper安装完成")