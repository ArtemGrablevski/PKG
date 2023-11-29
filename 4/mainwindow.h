#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void drawNextAlgorithm();
    void drawPrevAlgorithm();

private:
    int currentAlgorithm;
    qint64 nanoSeconds;

    QLabel *timeLabel;

    void drawStepByStepLine(QPainter &painter);
    void drawDDALine(QPainter &painter);
    void drawBresenhamLine(QPainter &painter);
    void drawBresenhamCircle(QPainter &painter);
    void drawBresenhamCircle2(QPainter &painter);

    bool isFullScreen;
};

#endif // MAINWINDOW_H
