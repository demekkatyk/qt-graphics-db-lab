#include "mainwindow.h"
#include <QGraphicsView>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    // Главный виджет и основной слой
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Левая панель с кнопками
    QVBoxLayout *leftPanel = new QVBoxLayout();
    QPushButton *btnRect = new QPushButton("Прямоугольник");
    QPushButton *btnEllipse = new QPushButton("Эллипс");
    QPushButton *btnPolygon = new QPushButton("Многоугольник");

    leftPanel->addWidget(btnRect);
    leftPanel->addWidget(btnEllipse);
    leftPanel->addWidget(btnPolygon);
    leftPanel->addStretch(); // прижимает кнопки к верху

    // Правая часть: сверху сцена, снизу таблица
    QVBoxLayout *rightPanel = new QVBoxLayout();
    
    // Наша графическая сцена (холст)
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing); // сглаживание
    view->setMinimumHeight(400);

    // Таблица для базы данных (пока пустая)
    tableView = new QTableView();
    tableView->horizontalHeader()->setStretchLastSection(true);

    rightPanel->addWidget(view, 2);    // сцена занимает 2/3
    rightPanel->addWidget(tableView, 1); // таблица занимает 1/3

    // Собираем всё вместе
    mainLayout->addLayout(leftPanel, 1);
    mainLayout->addLayout(rightPanel, 4);

    setCentralWidget(centralWidget);
    setWindowTitle("Lab: Graphics & DB");
    resize(1000, 700);

    // Подключаем кнопки к слотам
    connect(btnRect, &QPushButton::clicked, this, &MainWindow::addRectangle);
    connect(btnEllipse, &QPushButton::clicked, this, &MainWindow::addEllipse);
    connect(btnPolygon, &QPushButton::clicked, this, &MainWindow::addPolygon);
}

// Простые обработчики для добавления фигур на сцену
void MainWindow::addRectangle()
{
    scene->addRect(50, 50, 100, 60, QPen(Qt::black), QBrush(Qt::cyan));
}

void MainWindow::addEllipse()
{
    scene->addEllipse(100, 100, 80, 80, QPen(Qt::black), QBrush(Qt::yellow));
}

void MainWindow::addPolygon()
{
    QPolygonF triangle;
    triangle << QPointF(200, 50) << QPointF(250, 150) << QPointF(150, 150);
    scene->addPolygon(triangle, QPen(Qt::black), QBrush(Qt::magenta));
}
