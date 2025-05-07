pragma Singleton
import QtQuick

QtObject {
    // 主色调
    readonly property color primary: "#3F51B5"
    readonly property color primaryLight: "#7986CB"
    readonly property color primaryDark: "#303F9F"

    // 强调色
    readonly property color accent: "#00BCD4"
    readonly property color accentLight: "#4DD0E1"
    readonly property color accentDark: "#0097A7"

    // 背景色
    readonly property color background: "#FAFAFA"
    readonly property color surface: "#FFFFFF"
    readonly property color card: "#FFFFFF"

    // 文本颜色
    readonly property color textPrimary: "#212121"
    readonly property color textSecondary: "#757575"
    readonly property color textHint: "#9E9E9E"
    readonly property color textDisabled: "#BDBDBD"
    readonly property color textOnPrimary: "#FFFFFF"
    readonly property color textOnAccent: "#FFFFFF"

    // 边框和分隔线
    readonly property color divider: "#E0E0E0"
    readonly property color border: "#BDBDBD"

    // 状态颜色
    readonly property color success: "#4CAF50"
    readonly property color warning: "#FFC107"
    readonly property color error: "#F44336"
    readonly property color info: "#2196F3"

    // 其他颜色
    readonly property color shadow: "#33000000"
    readonly property color overlay: "#33000000"
    readonly property color hoverHighlight: "#0A000000"
    readonly property color selectedHighlight: "#1A000000"
}