#include "mainwindow.h"
#include <QDebug>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentAlgorithm(0)
{
    QPushButton *algorithmButton = new QPushButton("Next Algorithm", this);
    connect(algorithmButton, &QPushButton::clicked, this, &MainWindow::drawNextAlgorithm);

    nanoSeconds = 0;

    timeLabel = new QLabel("Time: ", this);
    timeLabel->setGeometry(10, 25, 450, 30);
    timeLabel->show();

    setGeometry(100, 100, 800, 500);
    algorithmButton->setGeometry(10, 440, 200, 50);
    algorithmButton->show();

    QPushButton *prevAlgorithmButton = new QPushButton("Previous Algorithm", this);
    connect(prevAlgorithmButton, &QPushButton::clicked, this, &MainWindow::drawPrevAlgorithm); // Подключаем кнопку к новому слоту
    prevAlgorithmButton->setGeometry(10, 380, 200, 50);
    prevAlgorithmButton->show();
}


MainWindow::~MainWindow() {}

void MainWindow::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);
    QPainter painter(this);

    int width = 800;
    int height = 500;

    int centerX = width / 2;
    int centerY = height / 2;

    painter.setPen(Qt::black);

    // Оси координат
    painter.setPen(QPen(Qt::white, 4)); // Более толстая линия для осей
    painter.drawLine(0, centerY, width, centerY); // Горизонтальная ось X
    painter.drawLine(centerX, 0, centerX, height); // Вертикальная ось Y

    // Линии сетки
    int gridSize = 50; // Размер сетки
    painter.setPen(QPen(Qt::gray, 1, Qt::DotLine)); // Стиль линии сетки

    for (int i = centerX % gridSize; i <= width; i += gridSize) {
        painter.drawLine(i, 0, i, height); // Вертикальные линии сетки
    }

    for (int i = centerY % gridSize; i <= height; i += gridSize) {
        painter.drawLine(0, i, width, i); // Горизонтальные линии сетки
    }

    // Подписи к осям
    painter.drawText(centerX + 5, 20, "X");
    painter.drawText(width - 20, centerY - 5, "Y");


    // Выбор и вызов метода для отрисовки на основе значения currentAlgorithm
    switch (currentAlgorithm) {
    case 0:
        drawStepByStepLine(painter);
        painter.drawText(300, 390, "Пошаговый алгоритм");
        break;
    case 1:
        drawDDALine(painter);
        painter.drawText(300, 390, "Алгоритм ЦДА");
        break;
    case 2:
        drawBresenhamLine(painter);
        painter.drawText(300, 390, "Алгоритм Брезенхема (линия)");
        break;
    case 3:
        drawBresenhamCircle(painter);
        painter.drawText(300, 390, "Алгоритм Брезенхема (окружность)");
        break;
    case 4:
        drawBresenhamCircle2(painter);
        painter.drawText(300, 390, "Алгоритм Брезенхема 2 (сглаживание)");
        break;
    default:
        break;
    }

    // Вывод времени выполнения алгоритма на экран
    QString timeStr = "Time: " + QString::number(nanoSeconds) + " nanoseconds";
    timeLabel->setText(timeStr);
}

void MainWindow::drawNextAlgorithm() {
    currentAlgorithm = (currentAlgorithm + 1) % 5;

    const int numIterations = 10; // Количество итераций для усреднения времени выполнения
    qint64 totalNanoSeconds = 0;

    for (int i = 0; i < numIterations; ++i) {
        QElapsedTimer timer;
        timer.start();

        update();

        qint64 elapsed = timer.nsecsElapsed();
        totalNanoSeconds += elapsed;
    }

    nanoSeconds = totalNanoSeconds / numIterations;

    update();
}

void MainWindow::drawPrevAlgorithm() {
    currentAlgorithm = (currentAlgorithm - 1 + 5) % 5; // Обратный переход на предыдущий алгоритм

    const int numIterations = 10; // Количество итераций для усреднения времени выполнения
    qint64 totalNanoSeconds = 0;

    for (int i = 0; i < numIterations; ++i) {
        QElapsedTimer timer;
        timer.start();

        update();

        qint64 elapsed = timer.nsecsElapsed();
        totalNanoSeconds += elapsed;
    }

    nanoSeconds = totalNanoSeconds / numIterations;

    update();
}

void MainWindow::drawStepByStepLine(QPainter &painter) {
    int windowWidth = 800;
    int windowHeight = 500;

    // Центр окна
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;

    int x1 = centerX - 150; // Начальная точка смещена на 150 пикселей влево от центра по x
    int y1 = centerY - 100; // Начальная точка смещена на 100 пикселей вверх от центра по y
    int x2 = centerX + 150; // Конечная точка смещена на 150 пикселей вправо от центра по x
    int y2 = centerY + 100; // Конечная точка смещена на 100 пикселей вниз от центра по y

    // Остальной код для рисования линии остается таким же
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = float(dx) / steps;
    float yIncrement = float(dy) / steps;

    float x = x1;
    float y = y1;

    painter.setPen(Qt::green);

    for (int i = 0; i <= steps; ++i) {
        painter.drawPoint(round(x), round(y));

        x += xIncrement;
        y += yIncrement;
    }

    QString values = "X1: " + QString::number(x1) +
                     ", Y1: " + QString::number(y1) +
                     ", X2: " + QString::number(x2) +
                     ", Y2: " + QString::number(y2);
    painter.drawText(10, 20, values); // Расположение текста с координатами
}

void MainWindow::drawDDALine(QPainter &painter) {
    int width = 800;
    int height = 500;

    int centerX = width / 2;
    int centerY = height / 2;

    int x1 = centerX - 100; // Начальная точка смещена на 100 пикселей влево от центра по x
    int y1 = centerY - 50; // Начальная точка смещена на 50 пикселей вверх от центра по y
    int x2 = centerX + 100; // Конечная точка смещена на 100 пикселей вправо от центра по x
    int y2 = centerY + 50; // Конечная точка смещена на 50 пикселей вниз от центра по y

    int dx = x2 - x1;
    int dy = y2 - y1;

    float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = float(dx) / steps;
    float yIncrement = float(dy) / steps;

    float x = x1;
    float y = y1;

    painter.setPen(Qt::green); // Установка цвета линии

    for (int i = 0; i <= steps; ++i) {
        painter.drawPoint(round(x), round(y));
        x += xIncrement;
        y += yIncrement;
    }

    QString values = "X1: " + QString::number(x1) +
                     ", Y1: " + QString::number(y1) +
                     ", X2: " + QString::number(x2) +
                     ", Y2: " + QString::number(y2);
    painter.drawText(10, 20, values); // Расположение текста с координатами
}

void MainWindow::drawBresenhamLine(QPainter &painter) {
    int width = 800;
    int height = 500;

    int centerX = width / 2;
    int centerY = height / 2;

    int x1 = centerX - 100; // Начальная точка смещена на 100 пикселей влево от центра по x
    int y1 = centerY - 50; // Начальная точка смещена на 50 пикселей вверх от центра по y
    int x2 = centerX + 100; // Конечная точка смещена на 100 пикселей вправо от центра по x
    int y2 = centerY + 50; // Конечная точка смещена на 50 пикселей вниз от центра по y

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    painter.setPen(Qt::green); // Установка цвета линии

    while (x1 != x2 || y1 != y2) {
        painter.drawPoint(x1, y1);
        int err2 = 2 * err;

        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    QString values = "X1: " + QString::number(x1) +
                     ", Y1: " + QString::number(y1) +
                     ", X2: " + QString::number(x2) +
                     ", Y2: " + QString::number(y2);
    painter.drawText(10, 20, values); // Расположение текста с координатами
}

void MainWindow::drawBresenhamCircle(QPainter &painter) {
    int width = 800;
    int height = 500;

    int centerX = width / 2;
    int centerY = height / 2;

    int radius = 100; // Радиус окружности

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    painter.setPen(Qt::green); // Установка цвета окружности


    while (x <= y) {
        painter.drawPoint(centerX + x, centerY + y);
        painter.drawPoint(centerX + x, centerY - y);
        painter.drawPoint(centerX - x, centerY + y);
        painter.drawPoint(centerX - x, centerY - y);
        painter.drawPoint(centerX + y, centerY + x);
        painter.drawPoint(centerX + y, centerY - x);
        painter.drawPoint(centerX - y, centerY + x);
        painter.drawPoint(centerX - y, centerY - x);

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    QString values = "Radius: " + QString::number(radius) +
                     ", X: " + QString::number(x) +
                     ", Y: " + QString::number(y) +
                     ", D: " + QString::number(d);
    painter.drawText(10, 20, values); // Расположение текста с координатами

}

void MainWindow::drawBresenhamCircle2(QPainter &painter) {
    int width = 800;
    int height = 500;

    int centerX = width / 2;
    int centerY = height / 2;

    int radius = 100; // Радиус окружности

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    painter.setPen(Qt::green); // Установка цвета окружности

    painter.setRenderHint(QPainter::Antialiasing);

    while (x <= y) {
        painter.drawPoint(centerX + x, centerY + y);
        painter.drawPoint(centerX + x, centerY - y);
        painter.drawPoint(centerX - x, centerY + y);
        painter.drawPoint(centerX - x, centerY - y);
        painter.drawPoint(centerX + y, centerY + x);
        painter.drawPoint(centerX + y, centerY - x);
        painter.drawPoint(centerX - y, centerY + x);
        painter.drawPoint(centerX - y, centerY - x);

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    QString values = "Radius: " + QString::number(radius) +
                     ", X: " + QString::number(x) +
                     ", Y: " + QString::number(y) +
                     ", D: " + QString::number(d);
    painter.drawText(10, 20, values); // Расположение текста с координатами
}

