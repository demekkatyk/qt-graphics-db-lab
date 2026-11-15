#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSqlTableModel>

class QGraphicsView;
class QPushButton;
class QTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addRectangle();
    void addEllipse();
    void addPolygon();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTableView *tableView;
    QSqlTableModel *model;

    void setupUI();
    void setupDatabase();
    void addShapeToDb(const QString &type);
};

#endif // MAINWINDOW_H
