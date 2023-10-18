#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void openFolder();
    void processImages(const QString &folderPath);
    void clearTable();
private:
    QTableWidget *tableWidget;
};

#endif // MAINWINDOW_H
