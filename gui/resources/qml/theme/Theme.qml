pragma Singleton
import QtQuick
import "."

QtObject {
    // 导出Colors和Typography的所有属性
    readonly property QtObject colors: Colors
    readonly property QtObject typography: Typography

    // 间距和大小
    readonly property int spacing: 8
    readonly property int spacingSmall: 4
    readonly property int spacingMedium: 8
    readonly property int spacingLarge: 16
    readonly property int spacingXLarge: 24

    // 圆角
    readonly property int radiusSmall: 2
    readonly property int radiusMedium: 4
    readonly property int radiusLarge: 8
    readonly property int radiusXLarge: 16

    // 阴影
    readonly property list<QtObject> elevation: [
        QtObject {
            // 无阴影
            property string color: "transparent"
            property int radius: 0
            property int offsetX: 0
            property int offsetY: 0
        },
        QtObject {
            // 轻微阴影 (1dp)
            property string color: "#1A000000"
            property int radius: 3
            property int offsetX: 0
            property int offsetY: 1
        },
        QtObject {
            // 中等阴影 (2dp)
            property string color: "#1F000000"
            property int radius: 5
            property int offsetX: 0
            property int offsetY: 2
        },
        QtObject {
            // 较重阴影 (4dp)
            property string color: "#24000000"
            property int radius: 8
            property int offsetX: 0
            property int offsetY: 4
        },
        QtObject {
            // 重阴影 (8dp)
            property string color: "#29000000"
            property int radius: 12
            property int offsetX: 0
            property int offsetY: 8
        }
    ]

    // 动画时长
    readonly property int durationFast: 150
    readonly property int durationNormal: 250
    readonly property int durationSlow: 350

    // 应用尺寸
    readonly property int appBarHeight: 56
    readonly property int tabBarHeight: 48
    readonly property int buttonHeight: 36
    readonly property int iconSize: 24
    readonly property int cardWidth: 300

    // 响应式布局断点
    readonly property int breakpointXS: 0
    readonly property int breakpointSM: 600
    readonly property int breakpointMD: 960
    readonly property int breakpointLG: 1280
    readonly property int breakpointXL: 1920

    // 辅助函数：基于屏幕宽度获取当前断点
    function getCurrentBreakpoint(width) {
        if (width >= breakpointXL) return "xl";
        if (width >= breakpointLG) return "lg";
        if (width >= breakpointMD) return "md";
        if (width >= breakpointSM) return "sm";
        return "xs";
    }

    // 辅助函数：为组件应用阴影效果
    function applyShadow(target, level) {
        if (level < 0 || level >= elevation.length) {
            level = 0;
        }

        target.layer.enabled = level > 0;
        if (level > 0) {
            target.layer.effect = shadowEffect;
            target.layer.samplerName = "source";

            shadowEffect.color = elevation[level].color;
            shadowEffect.horizontalOffset = elevation[level].offsetX;
            shadowEffect.verticalOffset = elevation[level].offsetY;
            shadowEffect.radius = elevation[level].radius;
        }
    }

    // 阴影效果组件
    property Component shadowEffect: Component {
        DropShadow {
            transparentBorder: true
            samples: 17
        }
    }
}