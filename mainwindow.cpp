#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *label = new QLabel("Привет! Это наше будущее приложение для лабораторной.", this);
    layout->addWidget(label);

    setCentralWidget(centralWidget);
    setWindowTitle("Lab: Qt Graphics & DB");
    resize(800, 600);
}

MainWindow::~MainWindow() {}
