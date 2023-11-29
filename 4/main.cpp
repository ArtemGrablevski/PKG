#include <QApplication>
#include <QStyleFactory>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;

    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(30, 30, 30));

    app.setPalette(darkPalette);
    mainWindow.setWindowTitle("Algorithms");
    mainWindow.setFixedSize(800, 500);

    mainWindow.show();
    return app.exec();
}

