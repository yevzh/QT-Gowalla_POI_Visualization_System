#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QMainWindow>
#include"dataset.h"
namespace Ui {
class DisplayWindow;
}

class DisplayWindow : public QMainWindow
{
    Q_OBJECT
private:
    DataSet*database;
    range latest;
    range latest_2;
    userComparison latest_3;
    locationComparison latest_4;
    pred latest_5;
public:
    explicit DisplayWindow(QWidget *parent = 0,DataSet*dt=NULL);
    ~DisplayWindow();
signals:
    void startdrawing(DataSet*dt,range r);
    void startdrawing_2(DataSet*dt,range r);
    void startdrawing_3(DataSet*dt,userComparison u);
    void startdrawing_4(DataSet*dt,locationComparison lo);
    void startpainting(DataSet*dt,locationComparison lo);
    void startpredicting(DataSet*dt,pred pre);
private:
    Ui::DisplayWindow *ui;
};

#endif // DISPLAYWINDOW_H
