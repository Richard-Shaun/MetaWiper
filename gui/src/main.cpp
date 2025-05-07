/**
* @file main.cpp
 * @brief MetaWiper GUI application entry point
 */
#include <QApplication>  // Changed from QGuiApplication to QApplication
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>
#include "application.h"

int main(int argc, char *argv[])
{
    // Enable high DPI support
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Use QApplication instead of QGuiApplication to support widgets like QFileDialog
    QApplication app(argc, argv);

    // Set application information
    app.setOrganizationName("MetaWiper");
    app.setOrganizationDomain("metawiper.org");
    app.setApplicationName("MetaWiper");
    app.setApplicationVersion("0.1.0");

    // Set Material style
    QQuickStyle::setStyle("Material");

    // Create application instance
    Application metaWiperApp;

    // Set up QML engine
    QQmlApplicationEngine engine;

    // Expose application instance to QML
    engine.rootContext()->setContextProperty("app", &metaWiperApp);

    // Also expose models directly to QML context to avoid calling getters
    engine.rootContext()->setContextProperty("fileListModel", metaWiperApp.getFileListModel());
    engine.rootContext()->setContextProperty("metadataModel", metaWiperApp.getMetadataModel());
    engine.rootContext()->setContextProperty("mainViewModel", metaWiperApp.getMainViewModel());

    // Register view models to QML - removed parameter
    metaWiperApp.registerViewModels();

    // Load main QML file
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}