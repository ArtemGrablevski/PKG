#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDir>
#include <QImageWriter>
#include <QImage>
#include <QFileInfo>

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);
    tableWidget->setHorizontalHeaderLabels({"File Name", "Size (px)", "Resolution (dpi)", "Color Depth", "Compression"});

    for (int i = 0; i < 5; i++) {
        tableWidget->setColumnWidth(i, 150);
    }

    QPushButton *openButton = new QPushButton("Open Folder", this);
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openFolder);

    QPushButton *clearButton = new QPushButton("Clear Table", this); // Create the clear button
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearTable); // Connect it to the slot

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(openButton);
    layout->addWidget(clearButton); // Add the clear button to the layout
    layout->addWidget(tableWidget);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void MainWindow::clearTable()
{
    tableWidget->clearContents(); // Clear the cell contents
    tableWidget->setRowCount(0); // Reset the row count
}

void MainWindow::openFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder");

    if (!folderPath.isEmpty()) {
        processImages(folderPath);
    }
}

void MainWindow::processImages(const QString &folderPath)
{
    QDir directory(folderPath);
    QStringList nameFilters = {"*.jpg", "*.gif", "*.tif", "*.bmp", "*.png", "*.pcx"};
    QStringList imageFiles = directory.entryList(nameFilters, QDir::Files);

    tableWidget->setRowCount(imageFiles.size());

    for (int i = 0; i < imageFiles.size(); ++i) {
        QString filePath = directory.filePath(imageFiles[i]);
        QImage image(filePath);
        QImageWriter imageWriter(filePath);

        QTableWidgetItem *fileNameItem = new QTableWidgetItem(imageFiles[i]);
        QTableWidgetItem *sizeItem = new QTableWidgetItem(QString("%1 x %2").arg(image.width()).arg(image.height()));
        QTableWidgetItem *resolutionItem = new QTableWidgetItem(QString("%1 x %2").arg(image.dotsPerMeterX() * 0.0254).arg(image.dotsPerMeterY() * 0.0254));
        QTableWidgetItem *colorDepthItem = new QTableWidgetItem(QString::number(image.depth()));
        QTableWidgetItem *compressionItem = new QTableWidgetItem(QString::number(imageWriter.compression()));

        tableWidget->setItem(i, 0, fileNameItem);
        tableWidget->setItem(i, 1, sizeItem);
        tableWidget->setItem(i, 2, resolutionItem);
        tableWidget->setItem(i, 3, colorDepthItem);
        tableWidget->setItem(i, 4, compressionItem);
    }

}
