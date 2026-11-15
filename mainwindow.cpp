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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupDatabase();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout *leftPanel = new QVBoxLayout();
    QPushButton *btnRect = new QPushButton("Прямоугольник");
    QPushButton *btnEllipse = new QPushButton("Эллипс");
    QPushButton *btnPolygon = new QPushButton("Многоугольник");

    leftPanel->addWidget(btnRect);
    leftPanel->addWidget(btnEllipse);
    leftPanel->addWidget(btnPolygon);
    leftPanel->addStretch();

    QVBoxLayout *rightPanel = new QVBoxLayout();
    
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setMinimumHeight(400);

    tableView = new QTableView();
    tableView->horizontalHeader()->setStretchLastSection(true);

    rightPanel->addWidget(view, 2);
    rightPanel->addWidget(tableView, 1);

    mainLayout->addLayout(leftPanel, 1);
    mainLayout->addLayout(rightPanel, 4);

    setCentralWidget(centralWidget);
    setWindowTitle("Lab: Graphics & DB");
    resize(1000, 700);

    connect(btnRect, &QPushButton::clicked, this, &MainWindow::addRectangle);
    connect(btnEllipse, &QPushButton::clicked, this, &MainWindow::addEllipse);
    connect(btnPolygon, &QPushButton::clicked, this, &MainWindow::addPolygon);
}

void MainWindow::setupDatabase()
{
    // Подключаемся к SQLite (файл создастся в папке проекта)
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("shapes.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка БД", "Не удалось открыть базу данных: " + db.lastError().text());
        return;
    }

    // Создаем таблицу, если её нет
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS shapes ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "type TEXT, "
               "connections TEXT, "
               "count INTEGER)");

    // Настраиваем модель для отображения таблицы
    model = new QSqlTableModel(this, db);
    model->setTable("shapes");
    model->select(); // Загружаем данные из БД

    // Настраиваем заголовки колонок
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Тип фигуры");
    model->setHeaderData(2, Qt::Horizontal, "Связи");
    model->setHeaderData(3, Qt::Horizontal, "Кол-во");

    tableView->setModel(model);
}

void MainWindow::addShapeToDb(const QString &type)
{
    QSqlQuery query;
    query.prepare("INSERT INTO shapes (type, connections, count) VALUES (:type, :conn, :count)");
    query.bindValue(":type", type);
    query.bindValue(":conn", ""); // пока нет связей
    query.bindValue(":count", 1);
    
    if (!query.exec()) {
        qDebug() << "Ошибка добавления в БД:" << query.lastError().text();
    } else {
        model->select(); // Обновляем таблицу на экране
    }
}

void MainWindow::addRectangle()
{
    scene->addRect(50, 50, 100, 60, QPen(Qt::black), QBrush(Qt::cyan));
    addShapeToDb("Прямоугольник");
}

void MainWindow::addEllipse()
{
    scene->addEllipse(100, 100, 80, 80, QPen(Qt::black), QBrush(Qt::yellow));
    addShapeToDb("Эллипс");
}

void MainWindow::addPolygon()
{
    QPolygonF triangle;
    triangle << QPointF(200, 50) << QPointF(250, 150) << QPointF(150, 150);
    scene->addPolygon(triangle, QPen(Qt::black), QBrush(Qt::magenta));
    addShapeToDb("Многоугольник");
}
