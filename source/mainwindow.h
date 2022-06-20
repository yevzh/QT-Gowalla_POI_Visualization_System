#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include.h"
#include"dataset.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    DataSet*dt;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startDisplay(DataSet*data);
private:
    Ui::MainWindow *ui;

signals:
    void starting(QString *pt,DataSet*dt);
};

#endif // MAINWINDOW_H
