#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include.h"
#include "dataset.h"
#include "loadthread.h"
#include"displaywindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Gowalla Dataset Visualization System");
    DataSet*data=new DataSet;
    dt=data;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0,1502535);
    QThread*t=new QThread;
    LoadThread*dataLoad=new LoadThread;
    dataLoad->moveToThread(t);

    QString*path=new QString;
    QObject::connect(ui->open,&QPushButton::clicked,[=]()
    {
        *path=QFileDialog::getOpenFileName(this,"Please select a file:","/home");
        ui->lineEdit->setText(*path);
    });
    QObject::connect(this,&MainWindow::starting,dataLoad,&LoadThread::working);
    QObject::connect(ui->load,&QPushButton::clicked,this,[=](){
        emit starting(path,data);
        t->start();
    });
    QObject::connect(dataLoad,&LoadThread::progress,this,[=](int i){
        ui->progressBar->setValue(i);
    });
    QObject::connect(dataLoad,&LoadThread::finished,this,[=](){
        this->startDisplay(data);
    });



    

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startDisplay(DataSet*data)
{
    DisplayWindow*dw=new DisplayWindow(0,data);
    dw->showMaximized();
    dw->show();
}
