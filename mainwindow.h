#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

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

    void setupUI();
};

#endif // MAINWINDOW_H
