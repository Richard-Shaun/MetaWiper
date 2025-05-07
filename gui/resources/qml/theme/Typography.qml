pragma Singleton
import QtQuick

QtObject {
    // 字体系列
    readonly property string fontFamily: "Roboto, Segoe UI, Arial, sans-serif"

    // 字体尺寸
    readonly property int h1: 24
    readonly property int h2: 20
    readonly property int h3: 18
    readonly property int h4: 16
    readonly property int body1: 14
    readonly property int body2: 13
    readonly property int caption: 12
    readonly property int button: 14
    readonly property int overline: 10

    // 字体权重
    readonly property int weightLight: Font.Light       // 300
    readonly property int weightRegular: Font.Normal    // 400
    readonly property int weightMedium: Font.Medium     // 500
    readonly property int weightBold: Font.Bold         // 700

    // 行高倍数
    readonly property real lineHeightH1: 1.4
    readonly property real lineHeightH2: 1.4
    readonly property real lineHeightH3: 1.4
    readonly property real lineHeightH4: 1.4
    readonly property real lineHeightBody: 1.5
    readonly property real lineHeightCaption: 1.4
    readonly property real lineHeightButton: 1.4

    // 字间距（百分比）
    readonly property real letterSpacingH1: -0.01
    readonly property real letterSpacingH2: -0.005
    readonly property real letterSpacingH3: 0
    readonly property real letterSpacingH4: 0.005
    readonly property real letterSpacingBody: 0.01
    readonly property real letterSpacingCaption: 0.02
    readonly property real letterSpacingButton: 0.02

    // 创建字体对象
    function createFont(size, weight, spacing, lineHeight) {
        return {
            family: fontFamily,
            pixelSize: size,
            weight: weight,
            letterSpacing: size * spacing,
            lineHeight: size * lineHeight
        };
    }

    // 预定义字体样式
    readonly property var h1Style: createFont(h1, weightLight, letterSpacingH1, lineHeightH1)
    readonly property var h2Style: createFont(h2, weightLight, letterSpacingH2, lineHeightH2)
    readonly property var h3Style: createFont(h3, weightRegular, letterSpacingH3, lineHeightH3)
    readonly property var h4Style: createFont(h4, weightRegular, letterSpacingH4, lineHeightH4)
    readonly property var body1Style: createFont(body1, weightRegular, letterSpacingBody, lineHeightBody)
    readonly property var body2Style: createFont(body2, weightRegular, letterSpacingBody, lineHeightBody)
    readonly property var captionStyle: createFont(caption, weightRegular, letterSpacingCaption, lineHeightCaption)
    readonly property var buttonStyle: createFont(button, weightMedium, letterSpacingButton, lineHeightButton)
}