#include "displaywindow.h"
#include "ui_displaywindow.h"
#include"include.h"
#include "timethread.h"
#include"mapthread.h"
#include"graphmodifier.h"
#include"comparethread.h"
#include "dauthread.h"
#include "heatmap.h"
#include "ui_heatmap.h"
#include "heatthread.h"
#include "predictthread.h"

using namespace QtDataVisualization;
DisplayWindow::DisplayWindow(QWidget *parent,DataSet*dt) :
    QMainWindow(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Gowalla Dataset Visualization System");

    //
    //the first function
    //first we create another thread
    QThread*t1=new QThread;

    //initialize
    ui->user_start->setValue(0);
    ui->user_end->setValue(22324);
    ui->time_start->setDateTime(QDateTime::fromString("2009-02-01 00:00:00","yyyy-MM-dd hh:mm:ss"));
    ui->time_end->setDateTime(QDateTime::fromString("2010-10-31 23:59:59","yyyy-MM-dd hh:mm:ss"));
    ui->location_start->setValue(0);
    ui->location_end->setValue(50384);
    ui->latitude_start->setValue(-90.00);
    ui->longtitude_start->setValue(-180.00);
    ui->latitude_end->setValue(90.00);
    ui->longtitude_end->setValue(180.00);
    latest.timeMin=ui->time_start->dateTime().toTime_t();
    latest.timeMax=ui->time_end->dateTime().toTime_t();

    latest.userMin=ui->user_start->value();
    latest.userMax=ui->user_end->value();
    latest.locationMin=ui->location_start->value();
    latest.locationMax=ui->location_end->value();
    latest.latitudeMin=ui->latitude_start->value();
    latest.latitudeMax=ui->latitude_end->value();
    latest.longtitudeMin=ui->longtitude_start->value();
    latest.longtitudeMax=ui->longtitude_end->value();
    latest.comboBox=ui->comboBox->currentIndex();

    QObject::connect(ui->time_start,&QDateTimeEdit::dateTimeChanged,this,[=]()
    {
//        start_time=begin_time.secsTo(ui->time_start->dateTime());
            latest.timeMin=ui->time_start->dateTime().toTime_t();
    });
    QObject::connect(ui->time_end,&QDateTimeEdit::dateTimeChanged,this,[=]()
    {
//        end_time=begin_time.secsTo(ui->time_end->dateTime());
            latest.timeMax=ui->time_end->dateTime().toTime_t();
    });

    QObject::connect(ui->user_start,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
//        start_user=ui->user_start->value();
        latest.userMin=ui->user_start->value();
    });
    QObject::connect(ui->user_end,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
//        end_user=ui->user_end->value();
        latest.userMax=ui->user_end->value();
    });
    QObject::connect(ui->location_start,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
//        start_location=ui->location_start->value();
        latest.locationMin=ui->location_start->value();
    });
    QObject::connect(ui->location_end,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
//        end_location=ui->location_end->value();
        latest.locationMax=ui->location_end->value();
    });
    QObject::connect(ui->latitude_start,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[=](){
//        start_latitude=ui->latitude_start->value();
        latest.latitudeMin=ui->latitude_start->value();
    });
    QObject::connect(ui->latitude_end,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[=](){
//        end_latitude=ui->latitude_end->value();
        latest.latitudeMax=ui->latitude_end->value();
    });
    QObject::connect(ui->longtitude_start,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[=](){
//        start_longtitude=ui->longtitude_start->value();
        latest.longtitudeMin=ui->longtitude_start->value();
    });
    QObject::connect(ui->longtitude_end,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[=](){
//        end_longtitude=ui->longtitude_end->value();
        latest.longtitudeMax=ui->longtitude_end->value();
    });
    QObject::connect(ui->comboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,[=]()
    {
        latest.comboBox=ui->comboBox->currentIndex();
    });

    TimeThread*depict=new TimeThread;
    depict->moveToThread(t1);

    QObject::connect(this,&DisplayWindow::startdrawing,depict,&TimeThread::filter);
    QObject::connect(ui->draw,&QPushButton::clicked,this,[=](){
        emit startdrawing(dt,latest);
        t1->start();
    });


    QObject::connect(depict,&TimeThread::finish,this,[=](QVector<int>nt){
        int numofVector=nt.size();
        QSplineSeries*line=new QSplineSeries();
        line->setName(QString("Num-time"));
        line->setVisible(true);
        line->setPointLabelsVisible(false);
        line->setPointsVisible(true);
        QDateTime xValue;
        for(int i=0;i<numofVector;i++)
        {
            int ran[3]={604800,86400,3600};
            xValue=QDateTime::fromTime_t(latest.timeMin+i*ran[latest.comboBox]);
            line->append(xValue.toMSecsSinceEpoch(),nt[i]);
            //qDebug()<<nt[i];
        }
        QDateTimeAxis *axisX = new QDateTimeAxis;
        QValueAxis*axisY=new QValueAxis;
        axisX->setFormat("yyyy-MM-dd hh:mm:ss");
        QChart*chart=new QChart();
        chart->setAnimationOptions(QChart::AllAnimations);
        chart->setLocalizeNumbers(true);
        chart->addSeries(line);
        chart->setAxisX(axisX,line);
        chart->setAxisY(axisY,line);

        axisX->setLabelsAngle(-45);
        axisX->setTickCount(10);
        chart->setTitle("POI checking-in data");
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
        chart->legend()->setBackgroundVisible(true);
        chart->legend()->setLabelColor(QColor(255,128,255));
        chart->legend()->setVisible(true);
        ui->draw_widget->setChart(chart);
    });



    //





    //
    //the second function
    QThread*t2=new QThread;
    ui->user_start_2->setValue(0);
    ui->user_end_2->setValue(22324);
    ui->time_start_2->setDateTime(QDateTime::fromString("2009-02-01 00:00:00","yyyy-MM-dd hh:mm:ss"));
    ui->time_end_2->setDateTime(QDateTime::fromString("2010-10-31 23:59:59","yyyy-MM-dd hh:mm:ss"));
    ui->location_start_2->setValue(0);
    ui->location_end_2->setValue(50384);
    ui->latitude_start_2->setValue(-90.00);
    ui->longtitude_start_2->setValue(-180.00);
    ui->latitude_end_2->setValue(90.00);
    ui->longtitude_end_2->setValue(180.00);
    latest_2.timeMin=ui->time_start_2->dateTime().toTime_t();
    latest_2.timeMax=ui->time_end_2->dateTime().toTime_t();
    latest_2.userMin=ui->user_start_2->value();
    latest_2.userMax=ui->user_end_2->value();
    latest_2.locationMin=ui->location_start_2->value();
    latest_2.locationMax=ui->location_end_2->value();
    latest_2.latitudeMin=ui->latitude_start_2->value();
    latest_2.latitudeMax=ui->latitude_end_2->value();
    latest_2.longtitudeMin=ui->longtitude_start_2->value();
    latest_2.longtitudeMax=ui->longtitude_end_2->value();
    QObject::connect(ui->time_start_2,&QDateTimeEdit::dateTimeChanged,this,[=]()
    {
        latest_2.timeMin=ui->time_start_2->dateTime().toTime_t();
    });
    QObject::connect(ui->time_end_2,&QDateTimeEdit::dateTimeChanged,this,[=]()
    {
        latest_2.timeMax=ui->time_end_2->dateTime().toTime_t();
    });

    QObject::connect(ui->user_start_2,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_2.userMin=ui->user_start_2->value();
    });
    QObject::connect(ui->user_end_2,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_2.userMax=ui->user_end_2->value();
    });
    QObject::connect(ui->location_start_2,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_2.locationMin=ui->location_start_2->value();
    });
    QObject::connect(ui->location_end_2,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_2.locationMax=ui->location_end_2->value();
    });
    QObject::connect(ui->latitude_start_2,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[=](){
        latest_2.latitudeMin=ui->latitude_start_2->value();
    });
    QObject::connect(ui->latitude_end_2,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[=](){
        latest_2.latitudeMax=ui->latitude_end_2->value();
    });
    QObject::connect(ui->longtitude_start_2,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[=](){
        latest_2.longtitudeMin=ui->longtitude_start_2->value();
    });
    QObject::connect(ui->longtitude_end_2,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[=](){
        latest_2.longtitudeMax=ui->longtitude_end_2->value();
    });
    MapThread*depict_2=new MapThread;
    depict_2->moveToThread(t2);


    ui->scrollArea->setVisible(false);
    QObject::connect(this,&DisplayWindow::startdrawing_2,depict_2,&MapThread::filter_2);
    QObject::connect(ui->draw_2,&QPushButton::clicked,this,[=](){
        emit startdrawing_2(dt,latest_2);
        t2->start();
    });


    QObject::connect(depict_2,&MapThread::finish_2,this,[=](QVector<int>nm){
        ui->scrollArea->setVisible(true);
        Q3DBars *widgetgraph=new Q3DBars;
        QWidget *container = QWidget::createWindowContainer(widgetgraph);
        ui->hLayout->addWidget(container,1);


//        QSlider *rotationSliderX = new QSlider(Qt::Horizontal, ui->map);
        ui->rotationSliderX->setTickInterval(30);
        ui->rotationSliderX->setTickPosition(QSlider::TicksBelow);
        ui->rotationSliderX->setMinimum(-180);
        ui->rotationSliderX->setValue(0);
        ui->rotationSliderX->setMaximum(180);

        ui->rotationSliderY->setTickInterval(15);
        ui->rotationSliderY->setTickPosition(QSlider::TicksAbove);
        ui->rotationSliderY->setMinimum(-90);
        ui->rotationSliderY->setValue(0);
        ui->rotationSliderY->setMaximum(90);

       ui->themeList->addItem(QStringLiteral("Qt"));
       ui->themeList->addItem(QStringLiteral("Primary Colors"));
       ui->themeList->addItem(QStringLiteral("Digia"));
       ui->themeList->addItem(QStringLiteral("Stone Moss"));
       ui->themeList->addItem(QStringLiteral("Army Blue"));
       ui->themeList->addItem(QStringLiteral("Retro"));
       ui->themeList->addItem(QStringLiteral("Ebony"));
       ui->themeList->addItem(QStringLiteral("Isabelle"));
       ui->themeList->setCurrentIndex(0);

//       QComboBox *barStyleList = new QComboBox(ui->map);
       ui->barStyleList->addItem(QStringLiteral("Bar"), int(QAbstract3DSeries::MeshBar));
       ui->barStyleList->addItem(QStringLiteral("Pyramid"), int(QAbstract3DSeries::MeshPyramid));
       ui->barStyleList->addItem(QStringLiteral("Cone"), int(QAbstract3DSeries::MeshCone));
       ui->barStyleList->addItem(QStringLiteral("Cylinder"), int(QAbstract3DSeries::MeshCylinder));
       ui->barStyleList->addItem(QStringLiteral("Bevel bar"), int(QAbstract3DSeries::MeshBevelBar));
       ui->barStyleList->addItem(QStringLiteral("Sphere"), int(QAbstract3DSeries::MeshSphere));
       ui->barStyleList->setCurrentIndex(4);

//       QComboBox *selectionModeList = new QComboBox(ui->map);
       ui->selectionModeList->addItem(QStringLiteral("None"),int(QAbstract3DGraph::SelectionNone));
       ui->selectionModeList->addItem(QStringLiteral("Bar"),int(QAbstract3DGraph::SelectionItem));
       ui->selectionModeList->addItem(QStringLiteral("Row"),int(QAbstract3DGraph::SelectionRow));
       ui->selectionModeList->addItem(QStringLiteral("Bar and Row"),int(QAbstract3DGraph::SelectionItemAndRow));
       ui->selectionModeList->addItem(QStringLiteral("Column"),int(QAbstract3DGraph::SelectionColumn));
       ui->selectionModeList->addItem(QStringLiteral("Bar and Column"),int(QAbstract3DGraph::SelectionItemAndColumn));
       ui->selectionModeList->addItem(QStringLiteral("Row and Column"),int(QAbstract3DGraph::SelectionRowAndColumn));
       ui->selectionModeList->addItem(QStringLiteral("Bar, Row and Column"),int(QAbstract3DGraph::SelectionItemRowAndColumn));
       ui->selectionModeList->addItem(QStringLiteral("Slice into Row"),int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionRow));
       ui->selectionModeList->addItem(QStringLiteral("Slice into Row and Item"),int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndRow));
       ui->selectionModeList->addItem(QStringLiteral("Slice into Column"),int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionColumn));
       ui->selectionModeList->addItem(QStringLiteral("Slice into Column and Item"),int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndColumn));
       ui->selectionModeList->addItem(QStringLiteral("Multi: Bar, Row, Col"),int(QAbstract3DGraph::SelectionItemRowAndColumn | QAbstract3DGraph::SelectionMultiSeries));
       ui->selectionModeList->addItem(QStringLiteral("Multi, Slice: Row, Item"),int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndRow | QAbstract3DGraph::SelectionMultiSeries));
       ui->selectionModeList->addItem(QStringLiteral("Multi, Slice: Col, Item"),int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndColumn  | QAbstract3DGraph::SelectionMultiSeries));
       ui->selectionModeList->setCurrentIndex(1);

//       QFontComboBox *fontList = new QFontComboBox(ui->map);
       ui->fontList->setCurrentFont(QFont("Times New Roman"));

//       QComboBox *shadowQuality = new QComboBox(ui->map);
       ui->shadowQuality->addItem(QStringLiteral("None"));
       ui->shadowQuality->addItem(QStringLiteral("Low"));
       ui->shadowQuality->addItem(QStringLiteral("Medium"));
       ui->shadowQuality->addItem(QStringLiteral("High"));
       ui->shadowQuality->addItem(QStringLiteral("Low Soft"));
       ui->shadowQuality->addItem(QStringLiteral("Medium Soft"));
       ui->shadowQuality->addItem(QStringLiteral("High Soft"));
       ui->shadowQuality->setCurrentIndex(5);

//       QComboBox *rangeList = new QComboBox(ui->map);
       ui->rangeList->addItem(QStringLiteral("-90~-80"));
       ui->rangeList->addItem(QStringLiteral("-80~-70"));
       ui->rangeList->addItem(QStringLiteral("-70~-60"));
       ui->rangeList->addItem(QStringLiteral("-60~-50"));
       ui->rangeList->addItem(QStringLiteral("-50~-40"));
       ui->rangeList->addItem(QStringLiteral("-40~-30"));
       ui->rangeList->addItem(QStringLiteral("-30~-20"));
       ui->rangeList->addItem(QStringLiteral("-20~-10"));
       ui->rangeList->addItem(QStringLiteral("-10~0"));
       ui->rangeList->addItem(QStringLiteral("0~10"));
       ui->rangeList->addItem(QStringLiteral("10~20"));
       ui->rangeList->addItem(QStringLiteral("20~30"));
       ui->rangeList->addItem(QStringLiteral("30~40"));
       ui->rangeList->addItem(QStringLiteral("40~50"));
       ui->rangeList->addItem(QStringLiteral("50~60"));
       ui->rangeList->addItem(QStringLiteral("60~70"));
       ui->rangeList->addItem(QStringLiteral("70~80"));
       ui->rangeList->addItem(QStringLiteral("80~90"));
       ui->rangeList->addItem(QStringLiteral("All"));
       ui->rangeList->setCurrentIndex(18);


       ui->fontSizeSlider->setTickInterval(10);
       ui->fontSizeSlider->setTickPosition(QSlider::TicksBelow);
       ui->fontSizeSlider->setMinimum(1);
       ui->fontSizeSlider->setValue(30);
       ui->fontSizeSlider->setMaximum(100);

//       QSlider *axisLabelRotationSlider = new QSlider(Qt::Horizontal, ui->map);
       ui->axisLabelRotationSlider->setTickInterval(10);
       ui->axisLabelRotationSlider->setTickPosition(QSlider::TicksBelow);
       ui->axisLabelRotationSlider->setMinimum(0);
       ui->axisLabelRotationSlider->setValue(30);
       ui->axisLabelRotationSlider->setMaximum(90);




       GraphModifier *modifier = new GraphModifier(widgetgraph,nm);
       QObject::connect(ui->rotationSliderX, &QSlider::valueChanged, modifier, &GraphModifier::rotateX);
       QObject::connect(ui->rotationSliderY, &QSlider::valueChanged, modifier, &GraphModifier::rotateY);

       QObject::connect(ui->labelButton, &QPushButton::clicked, modifier,&GraphModifier::changeLabelBackground);
       QObject::connect(ui->cameraButton, &QPushButton::clicked, modifier,&GraphModifier::changePresetCamera);
       QObject::connect(ui->zoomToSelectedButton, &QPushButton::clicked, modifier,&GraphModifier::zoomToSelectedBar);

       QObject::connect(ui->backgroundCheckBox, &QCheckBox::stateChanged, modifier,&GraphModifier::setBackgroundEnabled);
       QObject::connect(ui->gridCheckBox, &QCheckBox::stateChanged, modifier,&GraphModifier::setGridEnabled);
       QObject::connect(ui->smoothCheckBox, &QCheckBox::stateChanged, modifier,&GraphModifier::setSmoothBars);

       QObject::connect(ui->reverseValueAxisCheckBox, &QCheckBox::stateChanged, modifier,&GraphModifier::setReverseValueAxis);
       QObject::connect(ui->reflectionCheckBox, &QCheckBox::stateChanged, modifier,&GraphModifier::setReflection);

       QObject::connect(modifier, &GraphModifier::backgroundEnabledChanged,ui->backgroundCheckBox, &QCheckBox::setChecked);
       QObject::connect(modifier, &GraphModifier::gridEnabledChanged,ui->gridCheckBox, &QCheckBox::setChecked);

       QObject::connect(ui->rangeList, SIGNAL(currentIndexChanged(int)), modifier,SLOT(changeRange(int)));

       QObject::connect(ui->barStyleList, SIGNAL(currentIndexChanged(int)), modifier,SLOT(changeStyle(int)));

       QObject::connect(ui->selectionModeList, SIGNAL(currentIndexChanged(int)), modifier,SLOT(changeSelectionMode(int)));

       QObject::connect(ui->themeList, SIGNAL(currentIndexChanged(int)), modifier,SLOT(changeTheme(int)));

       QObject::connect(ui->shadowQuality, SIGNAL(currentIndexChanged(int)), modifier,SLOT(changeShadowQuality(int)));

       QObject::connect(modifier, &GraphModifier::shadowQualityChanged, ui->shadowQuality,&QComboBox::setCurrentIndex);
       QObject::connect(widgetgraph, &Q3DBars::shadowQualityChanged, modifier,&GraphModifier::shadowQualityUpdatedByVisual);

       QObject::connect(ui->fontSizeSlider, &QSlider::valueChanged, modifier,&GraphModifier::changeFontSize);
       QObject::connect(ui->fontList, &QFontComboBox::currentFontChanged, modifier,&GraphModifier::changeFont);

       QObject::connect(modifier, &GraphModifier::fontSizeChanged, ui->fontSizeSlider,&QSlider::setValue);
       QObject::connect(modifier, &GraphModifier::fontChanged, ui->fontList,&QFontComboBox::setCurrentFont);

       QObject::connect(ui->axisTitlesVisibleCB, &QCheckBox::stateChanged, modifier,&GraphModifier::setAxisTitleVisibility);
       QObject::connect(ui->axisTitlesFixedCB, &QCheckBox::stateChanged, modifier,&GraphModifier::setAxisTitleFixed);
       QObject::connect(ui->axisLabelRotationSlider, &QSlider::valueChanged, modifier,&GraphModifier::changeLabelRotation);

       QObject::connect(widgetgraph->scene()->activeCamera(), &Q3DCamera::xRotationChanged,modifier, &GraphModifier::xRotateChanged);
       QObject::connect(modifier, &GraphModifier::xRotateChanged,ui->rotationSliderX,&QSlider::setValue);
       QObject::connect(widgetgraph->scene()->activeCamera(), &Q3DCamera::xRotationChanged,modifier, &GraphModifier::xRotateChanged);
       QObject::connect(modifier, &GraphModifier::xRotateChanged,ui->rotationSliderX,&QSlider::setValue);

       QObject::connect(ui->clear,&QPushButton::clicked,[=]()
       {
           ui->hLayout->removeWidget(container);
           ui->themeList->clear();
           ui->barStyleList->clear();
           ui->selectionModeList->clear();
           ui->fontList->clear();
           ui->shadowQuality->clear();
           ui->rangeList->clear();
           ui->scrollArea->setVisible(false);
       });
    });










    //the third function
    QThread*t3=new QThread;
    ui->time_start_3->setDateTime(QDateTime::fromString("2009-02-01 00:00:00","yyyy-MM-dd hh:mm:ss"));
    ui->time_end_3->setDateTime(QDateTime::fromString("2010-10-31 23:59:59","yyyy-MM-dd hh:mm:ss"));
    ui->user_1->setValue(0);
    ui->user_2->setValue(0);
    latest_3.timeMin=ui->time_start_3->dateTime().toTime_t();
    latest_3.timeMax=ui->time_end_3->dateTime().toTime_t();
    latest_3.comboBox=ui->comboBox_3->currentIndex();

    QObject::connect(ui->time_start_3,&QDateTimeEdit::dateTimeChanged,this,[=]()
    {
        latest_3.timeMin=ui->time_start_3->dateTime().toTime_t();
    });
    QObject::connect(ui->time_end_3,&QDateTimeEdit::dateTimeChanged,this,[=]()
    {
        latest_3.timeMax=ui->time_end_3->dateTime().toTime_t();
    });

    QObject::connect(ui->user_1,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_3.user1=ui->user_1->value();
    });
    QObject::connect(ui->user_2,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_3.user2=ui->user_2->value();
    });
    QObject::connect(ui->comboBox_3,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,[=]()
    {
        latest_3.comboBox=ui->comboBox_3->currentIndex();
    });

    CompareThread*compare=new CompareThread;
    compare->moveToThread(t3);
    QObject::connect(this,&DisplayWindow::startdrawing_3,compare,&CompareThread::filter_3);
    QObject::connect(ui->compare,&QPushButton::clicked,this,[=](){
        emit startdrawing_3(dt,latest_3);
        t3->start();
    });


    int*up1=new int[10];
    int*up1_num=new int[10];
    int*up2=new int[10];
    int*up2_num=new int[10];
    for(int i=0;i<10;i++)
    {
        up1[i]=0;
        up1_num[i]=0;
        up2[i]=0;
        up2_num[i]=0;
    }
    QObject::connect(compare,&CompareThread::finish_3,this,[=](QVector<int>user_nt1,QVector<int>user_nt2,QVector<int>c1,QVector<int>c2){
        QSplineSeries*line1=new QSplineSeries;
        int max=0;
        for(int i=0;i<user_nt1.size();i++)
        {
            if(user_nt1[i]>max)max=user_nt1[i];
            if(user_nt2[i]>max)max=user_nt2[i];
        }
        QSplineSeries*line2=new QSplineSeries;
        line1->setName(QString("user1"));
        line1->setVisible(true);
        line1->setPointLabelsVisible(false);
        line1->setPointsVisible(true);
        line2->setName(QString("user2"));
        line2->setVisible(true);
        line2->setPointLabelsVisible(false);
        line2->setPointsVisible(true);
        QDateTime xValue1,xValue2;
        for(int i=0;i<user_nt1.size();i++)
        {
            int ran[3]={604800,86400,3600};
            xValue1=QDateTime::fromTime_t(latest_3.timeMin+i*ran[latest_3.comboBox]);
            xValue2=QDateTime::fromTime_t(latest_3.timeMin+i*ran[latest_3.comboBox]);
            line1->append(xValue1.toMSecsSinceEpoch(),user_nt1[i]);
            line2->append(xValue2.toMSecsSinceEpoch(),user_nt2[i]);
            //qDebug()<<nt[i];
        }
        QDateTimeAxis *axisX = new QDateTimeAxis;
        QValueAxis*axisY=new QValueAxis;
        axisY->setRange(0,max);
        axisX->setFormat("yyyy-MM-dd hh:mm:ss");
        QChart*chart_3=new QChart();
        chart_3->setAnimationOptions(QChart::AllAnimations);
        chart_3->setLocalizeNumbers(true);
        chart_3->addSeries(line1);
        chart_3->addSeries(line2);
        chart_3->setAxisX(axisX,line1);
        chart_3->setAxisX(axisX,line2);
        chart_3->setAxisY(axisY,line1);
        chart_3->setAxisY(axisY,line2);
        axisX->setLabelsAngle(-60);
        axisX->setTickCount(6);
        chart_3->setTitle("POI checking-in data");
        chart_3->legend()->setVisible(true);
        chart_3->legend()->setAlignment(Qt::AlignBottom);
        chart_3->legend()->setBackgroundVisible(true);
        chart_3->legend()->setLabelColor(QColor(255,128,255));
        chart_3->legend()->setVisible(true);
        ui->userline->setChart(chart_3);


        int*cp1=new int[50835];
        int*cp2=new int[50835];
        for(int i=0;i<50835;i++)
        {
            cp1[i]=c1[i];
            cp2[i]=c2[i];
        }
        QBarSeries*bar1=new QBarSeries;
//        for(int i=0;i<50385;i++)
//        {
//            if(dd<cp1[i])dd=cp1[i];
//        }
//        qDebug()<<dd;
        for(int i=0;i<10;i++)
        {
            int pointMax_1=0,max_1=0;
            for(int j=0;j<50835;j++)
            {
                if(cp1[j]>max_1)
                {
                    max_1=cp1[j];
                    pointMax_1=j;
//                                qDebug()<<max_1;
                }
            }
            cp1[pointMax_1]=0;
            up1[i]=pointMax_1;
            up1_num[i]=max_1;

//                                          qDebug()<<max_1;
        }
        QBarSet*barRank_1=new QBarSet("user1");
        barRank_1->setColor(QColor(50,100,200));
        *barRank_1<<up1_num[0]<<up1_num[1]<<up1_num[2]<<up1_num[3]<<up1_num[4]<<up1_num[5]<<up1_num[6]<<up1_num[7]<<up1_num[8]<<up1_num[9];
        bar1->append(barRank_1);
        QChart *pieChart_1 = new QChart();
        pieChart_1->addSeries(bar1);
        pieChart_1->setTitle("the top 10 POIs of user1");
        pieChart_1->setAnimationOptions(QChart::SeriesAnimations);
        QBarCategoryAxis*axis_1=new QBarCategoryAxis;
        axis_1->setTitleText("POI ID");
        for(int i=0;i<10;i++)
        {
            axis_1->append(QString::number(up1[i]));
        }
        pieChart_1->createDefaultAxes();
        pieChart_1->setAxisX(axis_1,bar1);
        pieChart_1->legend()->setVisible(true);
        pieChart_1->legend()->setAlignment(Qt::AlignBottom);

        ui->pie_1->setChart(pieChart_1);

        QBarSeries*bar2=new QBarSeries;
        for(int i=0;i<10;i++)
        {
            int pointMax_2=0,max_2=0;
            for(int j=0;j<50835;j++)
            {
                if(cp2[j]>max_2)
                {
                    max_2=cp2[j];
                    pointMax_2=j;
                }
            }
            cp2[pointMax_2]=0;
            up2[i]=pointMax_2;
            up2_num[i]=max_2;

        }
        QBarSet*barRank_2=new QBarSet("user2");
        barRank_2->setColor(QColor(55,200,100));
        *barRank_2<<up2_num[0]<<up2_num[1]<<up2_num[2]<<up2_num[3]<<up2_num[4]<<up2_num[5]<<up2_num[6]<<up2_num[7]<<up2_num[8]<<up2_num[9];
//         *barRank_2<<0<<1<<2<<3<<4<<5<<6<<7<<8<<9;
        bar2->append(barRank_2);
        QChart *pieChart_2 = new QChart();
        pieChart_2->addSeries(bar2);
        pieChart_2->setTitle("the top 10 POIs of user2");
        pieChart_2->setAnimationOptions(QChart::SeriesAnimations);
        QBarCategoryAxis*axis_2=new QBarCategoryAxis;
        axis_2->setTitleText("POI ID");
        for(int i=0;i<10;i++)
        {
            axis_2->append(QString::number(up2[i]));
        }
        pieChart_2->createDefaultAxes();
        pieChart_2->setAxisX(axis_2,bar2);
        pieChart_2->legend()->setVisible(true);
        pieChart_2->legend()->setAlignment(Qt::AlignBottom);

        ui->pie_2->setChart(pieChart_2);

        delete []cp1;
        delete []cp2;
    });








    //the fourth function

    QThread*t4=new QThread;
    ui->location_1->setValue(0);
    ui->location_2->setValue(0);
    ui->dateEdit->setDate(QDate::fromString("2009-10-01","yyyy-MM-dd"));
    latest_4.location1=ui->location_1->value();
    latest_4.location2=ui->location_2->value();
//    latest_4.time=QDateTime::fromString(ui->dateEdit->date().toString("yyyy-MM-dd hh:mm:ss"), "yyyy-MM-dd hh:mm:ss").toTime_t();

    latest_4.time=ui->dateEdit->dateTime().toTime_t();
    QObject::connect(ui->dateEdit,&QDateEdit::dateChanged,this,[=](){
        latest_4.time=ui->dateEdit->dateTime().toTime_t();
    });
    QObject::connect(ui->location_1,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_4.location1=ui->location_1->value();
    });
    QObject::connect(ui->location_2,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_4.location2=ui->location_2->value();
    });
    DAUThread*dau=new DAUThread;
    dau->moveToThread(t4);
    QObject::connect(this,&DisplayWindow::startdrawing_4,dau,&DAUThread::filter_4);
    QObject::connect(ui->depictdau,&QPushButton::clicked,this,[=](){
        emit startdrawing_4(dt,latest_4);
        t4->start();
    });


    QThread*t4_h=new QThread;
    HeatThread*heatSelector=new HeatThread;
    heatSelector->moveToThread(t4_h);
    QObject::connect(this,&DisplayWindow::startpainting,heatSelector,&HeatThread::filter_h);
    QObject::connect(ui->heatmap,&QPushButton::clicked,this,[=](){
        emit startpainting(dt,latest_4);
        t4_h->start();
    });

    QObject::connect(heatSelector,&HeatThread::finish_h,this,[=](QVector<int>xx,QVector<int>yy){
        HeatMap*hm=new HeatMap(0,xx,yy);
        for(int i=0;i<xx.size();i++)
        {
            hm->paintMap(i);
        }
        hm->show();
    });


    int*upup1=new int[10];
    int*upup1_num=new int[10];
    int*upup2=new int[10];
    int*upup2_num=new int[10];
    int*up=new int[10];
    int*up_num=new int[10];
    for(int i=0;i<10;i++)
    {
        upup1[i]=0;
        upup1_num[i]=0;
        upup2[i]=0;
        upup2_num[i]=0;
        up[i]=0;
        up_num[i]=0;
    }
    QObject::connect(dau,&DAUThread::finish_4,this,[=](QVector<int>lc_1,QVector<int>lc_2,QVector<int>cc){

        int*l=new int[50835];
        for(int i=0;i<50835;i++)
        {
            l[i]=cc[i];
        }
        QBarSeries*lbar=new QBarSeries;
        for(int i=0;i<10;i++)
        {
            int pointMax=0,max=0;
            for(int j=0;j<50835;j++)
            {
                if(l[j]>max)
                {
                    max=l[j];
                    pointMax=j;
                }
            }
            l[pointMax]=0;
            up[i]=pointMax;
            up_num[i]=max;
        }
        QBarSet*lbarRank=new QBarSet("daily top locations");
        *lbarRank<<up_num[0]<<up_num[1]<<up_num[2]<<up_num[3]<<up_num[4]<<up_num[5]<<up_num[6]<<up_num[7]<<up_num[8]<<up_num[9];
        lbar->append(lbarRank);
        QChart *lpieChart = new QChart();
        lpieChart->addSeries(lbar);
        lpieChart->setTitle("Daily Active Locations");
        lpieChart->setAnimationOptions(QChart::SeriesAnimations);
        QBarCategoryAxis*laxis=new QBarCategoryAxis;
        laxis->setTitleText("POI ID");
        for(int i=0;i<10;i++)
        {
            laxis->append(QString::number(up[i]));
        }
        lpieChart->createDefaultAxes();
        lpieChart->setAxisX(laxis,lbar);
        lpieChart->legend()->setVisible(true);
        lpieChart->legend()->setAlignment(Qt::AlignBottom);

        ui->location_distribution->setChart(lpieChart);



        int *lp1=new int[22325];
        int *lp2=new int[22325];
        for(int i=0;i<22325;i++)
        {
            lp1[i]=lc_1[i];
            lp2[i]=lc_2[i];
        }
//        qDebug()<<m;
        QBarSeries*lbar1=new QBarSeries;
        for(int i=0;i<10;i++)
        {
            int pointMax_1=0,max_1=0;
            for(int j=0;j<22325;j++)
            {
                if(lp1[j]>max_1)
                {
                    max_1=lp1[j];
                    pointMax_1=j;
                }
            }
            lp1[pointMax_1]=0;
            upup1[i]=pointMax_1;
            upup1_num[i]=max_1;
//            qDebug()<<pointMax_1;
        }
        QBarSet*lbarRank_1=new QBarSet("location1");
        lbarRank_1->setColor(QColor(50, 100, 255));
        *lbarRank_1<<upup1_num[0]<<upup1_num[1]<<upup1_num[2]<<upup1_num[3]<<upup1_num[4]<<upup1_num[5]<<upup1_num[6]<<upup1_num[7]<<upup1_num[8]<<upup1_num[9];
//        *lbarRank_1<<0<<1<<2<<3<<4<<5<<6<<7<<8<<9;
        lbar1->append(lbarRank_1);
        QChart *lpieChart_1 = new QChart();
        lpieChart_1->addSeries(lbar1);
        lpieChart_1->setTitle("Daily Active Users");
        lpieChart_1->setAnimationOptions(QChart::SeriesAnimations);
        QBarCategoryAxis*laxis_1=new QBarCategoryAxis;
        laxis_1->setTitleText("User ID");
        for(int i=0;i<10;i++)
        {
            laxis_1->append(QString::number(upup1[i]));
        }
        lpieChart_1->createDefaultAxes();
        lpieChart_1->setAxisX(laxis_1,lbar1);
        lpieChart_1->legend()->setVisible(true);
        lpieChart_1->legend()->setAlignment(Qt::AlignBottom);

        ui->pos1->setChart(lpieChart_1);


        QBarSeries*lbar2=new QBarSeries;
        for(int i=0;i<10;i++)
        {
            int pointMax_2=0,max_2=0;
            for(int j=0;j<22325;j++)
            {
                if(lp2[j]>max_2)
                {
                    max_2=lp2[j];
                    pointMax_2=j;
                }
            }
            lp2[pointMax_2]=0;
            upup2[i]=pointMax_2;
            upup2_num[i]=max_2;
//            qDebug()<<pointMax_1;
        }
        QBarSet*lbarRank_2=new QBarSet("location2");
        lbarRank_2->setColor(QColor(50, 255, 100));
        *lbarRank_2<<upup2_num[0]<<upup2_num[1]<<upup2_num[2]<<upup2_num[3]<<upup2_num[4]<<upup2_num[5]<<upup2_num[6]<<upup2_num[7]<<upup2_num[8]<<upup2_num[9];
        lbar2->append(lbarRank_2);
        QChart *lpieChart_2 = new QChart();
        lpieChart_2->addSeries(lbar2);
        lpieChart_2->setTitle("Daily Active Users");
        lpieChart_2->setAnimationOptions(QChart::SeriesAnimations);
        QBarCategoryAxis*laxis_2=new QBarCategoryAxis;
        laxis_2->setTitleText("User ID");
        for(int i=0;i<10;i++)
        {
            laxis_2->append(QString::number(upup2[i]));
        }
        lpieChart_2->createDefaultAxes();
        lpieChart_2->setAxisX(laxis_2,lbar2);
        lpieChart_2->legend()->setVisible(true);
        lpieChart_2->legend()->setAlignment(Qt::AlignBottom);
        ui->pos2->setChart(lpieChart_2);
    });



    QThread*t5_1=new QThread;
    QThread*t5_2=new QThread;
    ui->poi->setValue(0);
    ui->user_predict->setValue(0);
    latest_5.pointofinterest=ui->poi->value();
    latest_5.interestinguser=ui->user_predict->value();
    QObject::connect(ui->poi,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_5.pointofinterest=ui->poi->value();
    });
    QObject::connect(ui->user_predict,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](){
        latest_5.interestinguser=ui->user_predict->value();
    });
    PredictThread*pl=new PredictThread;
    PredictThread*pu=new PredictThread;
    pl->moveToThread(t5_1);
    pu->moveToThread(t5_2);
    QObject::connect(this,&DisplayWindow::startpredicting,pl,&PredictThread::poiPrediction);
    QObject::connect(ui->predict_1,&QPushButton::clicked,this,[=]()
    {
        emit startpredicting(dt,latest_5);
        t5_1->start();
    });
    QObject::connect(this,&DisplayWindow::startpredicting,pu,&PredictThread::userPrediction);
    QObject::connect(ui->predict_2,&QPushButton::clicked,this,[=]()
    {
        emit startpredicting(dt,latest_5);
        t5_2->start();
    });
    QObject::connect(pu,&PredictThread::finish_up,this,[=](int j)
    {
        ui->value_2->setText(QString::number(j));
    });
    QObject::connect(pl,&PredictThread::finish_lp,this,[=](int a,int b,int c){
       ui->value_11->setText(QString::number(a));
       ui->value_12->setText(QString::number(b));
       ui->value_13->setText(QString::number(c));

    });



}

DisplayWindow::~DisplayWindow()
{
    delete ui;
}
