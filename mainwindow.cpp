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
#include <QStyledItemDelegate>
#include <QPainter>

// ==========================================
// КАСТОМНЫЙ ДЕЛЕГАТ ДЛЯ ОТРИСОВКИ ИКОНОК
// ==========================================
class IconDelegate : public QStyledItemDelegate {
public:
    IconDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyledItemDelegate::paint(painter, option, index);
        
        int count = index.data().toInt();
        if (count <= 0) return;

        int iconCount = 0;
        if (count >= 1 && count <= 3) iconCount = 1;
        else if (count >= 4 && count <= 10) iconCount = 2;
        else if (count > 10) iconCount = 3;

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QRect rect = option.rect;
        int startX = rect.right() - 20;
        
        for (int i = 0; i < iconCount; ++i) {
            QRect iconRect(startX - (i * 20), rect.top() + 5, 15, 15);
            painter->setBrush(Qt::blue);
            painter->setPen(Qt::black);
            painter->drawEllipse(iconRect);
        }
        painter->restore();
    }
};

// ==========================================
// MAIN WINDOW
// ==========================================
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
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("shapes.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка БД", "Не удалось открыть базу данных: " + db.lastError().text());
        return;
    }

    QSqlQuery query;
    // ВНИМАНИЕ: Для лабораторной мы каждый раз создаем таблицу заново (DROP), 
    // чтобы не было старых дублей при отладке. 
    query.exec("DROP TABLE IF EXISTS shapes");
    query.exec("CREATE TABLE shapes ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "type TEXT UNIQUE, "
               "connections TEXT, "
               "count INTEGER)");

    // Сразу добавляем три базовые строки
    query.exec("INSERT INTO shapes (type, connections, count) VALUES ('Прямоугольник', '', 0)");
    query.exec("INSERT INTO shapes (type, connections, count) VALUES ('Эллипс', '', 0)");
    query.exec("INSERT INTO shapes (type, connections, count) VALUES ('Многоугольник', '', 0)");

    model = new QSqlTableModel(this, db);
    model->setTable("shapes");
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Тип фигуры");
    model->setHeaderData(2, Qt::Horizontal, "Связи");
    model->setHeaderData(3, Qt::Horizontal, "Кол-во");

    tableView->setModel(model);
    tableView->setItemDelegateForColumn(3, new IconDelegate(this));
    tableView->setColumnWidth(0, 50);
    tableView->setColumnWidth(1, 150);
    tableView->setColumnWidth(2, 150);
}

void MainWindow::addShapeToDb(const QString &type)
{
    QSqlQuery query;
    query.prepare("UPDATE shapes SET count = count + 1 WHERE type = :type");
    query.bindValue(":type", type);
    
    if (query.exec()) {
        model->select();
    } else {
        qDebug() << "Ошибка обновления БД:" << query.lastError().text();
    }
}

void MainWindow::addRectangle()
{
    // Располагаем фигуры со сдвигом, чтобы они не накладывались друг на друга
    int offset = scene->items().size() * 10;
    scene->addRect(50 + offset, 50 + offset, 100, 60, QPen(Qt::black), QBrush(Qt::cyan));
    addShapeToDb("Прямоугольник");
}

void MainWindow::addEllipse()
{
    int offset = scene->items().size() * 10;
    scene->addEllipse(100 + offset, 100 + offset, 80, 80, QPen(Qt::black), QBrush(Qt::yellow));
    addShapeToDb("Эллипс");
}

void MainWindow::addPolygon()
{
    int offset = scene->items().size() * 10;
    QPolygonF triangle;
    triangle << QPointF(200 + offset, 50 + offset) << QPointF(250 + offset, 150 + offset) << QPointF(150 + offset, 150 + offset);
    scene->addPolygon(triangle, QPen(Qt::black), QBrush(Qt::magenta));
    addShapeToDb("Многоугольник");
}
