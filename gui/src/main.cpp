/**
* @file main.cpp
 * @brief MetaWiper GUI应用程序入口点
 */
#include <QApplication>
#include <QtQuickControls2/QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>
#include "application.h"

int main(int argc, char *argv[])
{
    // 设置高DPI支持
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setOrganizationName("MetaWiper");
    app.setOrganizationDomain("metawiper.org");
    app.setApplicationName("MetaWiper");
    app.setApplicationVersion("0.1.0");

    // 设置Material风格
    QQuickStyle::setStyle("Material");

    // 创建应用程序实例
    Application metaWiperApp;

    // 设置QML引擎
    QQmlApplicationEngine engine;

    // 向QML暴露应用程序实例
    engine.rootContext()->setContextProperty("app", &metaWiperApp);

    // 注册视图模型到QML
    metaWiperApp.registerViewModels();

    // 加载主QML文件
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}