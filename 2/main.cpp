#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;

    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(30, 30, 30));
    app.setPalette(darkPalette);

    app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");


    mainWindow.setFixedSize(770, 500);
    mainWindow.setWindowTitle("Images viewer");
    mainWindow.show();
    return app.exec();
}
