#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSqlTableModel>

class QGraphicsView;
class QPushButton;
class QTableView;
class QComboBox;
class QLineEdit;
class QSpinBox;

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
    void applyFilter();
    void deleteSelected(); // Новый слот для удаления

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTableView *tableView;
    QSqlTableModel *model;

    QComboBox *typeFilter;
    QSpinBox *countFilter;
    QLineEdit *idFilter;
    QPushButton *btnDelete; // Кнопка удаления

    void setupUI();
    void setupDatabase();
    void addShapeToDb(const QString &type);
};

#endif // MAINWINDOW_H
