/**
* @file main.cpp
 * @brief MetaWiper GUI application entry point
 */
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "application.h"

int main(int argc, char *argv[]){

    // Enable high DPI support
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Use QApplication instead of QGuiApplication to support widgets like QFileDialog
    QApplication app(argc, argv);

    // Set application information
    app.setOrganizationName("Richard Shaun");
    app.setOrganizationDomain("https://github.com/Richard-Shaun/");
    app.setApplicationName("MetaWiper");
    app.setApplicationVersion("0.1.0");

    // Set Material style
    QQuickStyle::setStyle("Material");

    // Create application instance
    Application meta_wiper_gui_app;

    // Set up QML engine
    QQmlApplicationEngine qml_engine;

    // Expose application instance to QML
    qml_engine.rootContext()->setContextProperty("app", &meta_wiper_gui_app);

    // Also expose models directly to QML context to avoid calling getters
    qml_engine.rootContext()->setContextProperty("fileListModel", meta_wiper_gui_app.getFileListModel());
    qml_engine.rootContext()->setContextProperty("metadataModel", meta_wiper_gui_app.getMetadataModel());
    qml_engine.rootContext()->setContextProperty("mainViewModel", meta_wiper_gui_app.getMainViewModel());

    // Register view models to QML
    meta_wiper_gui_app.registerViewModels();

    // Load main QML file
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    QObject::connect(&qml_engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qCritical() << "Failed to load QML file:" << url.toString();
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    qml_engine.load(url);

    return app.exec();
}