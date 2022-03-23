#include "graphmodifier.h"
#include <QtDataVisualization/qcategory3daxis.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qbar3dseries.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtCore/QTime>
#include <QtWidgets/QComboBox>
#include <QtCore/qmath.h>

using namespace QtDataVisualization;
GraphModifier::GraphModifier(Q3DBars *bargraph,QVector<int>nm)
    : m_graph(bargraph),
    m_xRotation(0.0f),
    m_yRotation(0.0f),
    m_fontSize(30),
    m_segments(4),
    m_subSegments(3),
    m_numAxis(new QValue3DAxis),
    m_latitudeAxis(new QCategory3DAxis),
    m_longtitudeAxis(new QCategory3DAxis),
    m_primarySeries(new QBar3DSeries),
    m_barMesh(QAbstract3DSeries::MeshBevelBar),
    m_smooth(false)
{
    m_longtitude << "-180~-170" << "-170~-160" << "-160~-150" << "-150~-140" << "-140~-130" << "-130~-120" << "-120~-110" << "-110~-100" << "-100~-90" << "-90~-80" << "-80~-70" << "-70~-60"<<"-60~-50"<<"-50~-40"<<"-40~-30"<<"-30~-20"<<"-20~-10"<<"-10~0"<<"0~10"<<"10~20"<<"20~30"<<"30~40"<<"40~50"<<"50~60"<<"60~70"<<"70~80"<<"80~90"<<"90~100"<<"100~110"<<"110~120"<<"120~130"<<"130~140"<<"140~150"<<"150~160"<<"160~170"<<"170~180";
    m_latitude<< "-90~-80" << "-80~-70" << "-70~-60" << "-60~-50" << "-50~-40" << "-40~-30" << "-30~-20" << "-20~-10"<<"-10~0"<<"0~10"<<"10~20"<<"20~30"<<"30~40"<<"40~50"<<"50~60"<<"60~70"<<"70~80"<<"80~90";

    m_numAxis->setTitle("Check-in Num");


    int maxNum=0;
    for(int i=0;i<nm.size();i++)
    {
        if(maxNum<nm[i])maxNum=nm[i];
    }
    m_numAxis->setSegmentCount(10);

    m_numAxis->setSubSegmentCount(5);
    m_numAxis->setRange(0, maxNum);
    m_numAxis->setLabelFormat(QString(QStringLiteral("%.1d ")));

    m_numAxis->setLabelAutoRotation(30.0f);
    m_numAxis->setTitleVisible(true);

    m_latitudeAxis->setTitle("latitude");
    m_latitudeAxis->setLabelAutoRotation(30.0f);
    m_latitudeAxis->setTitleVisible(true);
    m_longtitudeAxis->setTitle("longtitude");
    m_longtitudeAxis->setLabelAutoRotation(30.0f);
    m_longtitudeAxis->setTitleVisible(true);

    m_graph->setValueAxis(m_numAxis);
    m_graph->setRowAxis(m_latitudeAxis);
    m_graph->setColumnAxis(m_longtitudeAxis);

    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftMedium);
    m_graph->activeTheme()->setBackgroundEnabled(false);
    m_graph->activeTheme()->setFont(QFont("Times New Roman", m_fontSize));
    m_graph->activeTheme()->setLabelBackgroundEnabled(true);

    m_graph->setMultiSeriesUniform(true);

    m_primarySeries->setItemLabelFormat(QStringLiteral("Gowalla - lo:@colLabel, la:@rowLabel: @valueLabel"));
    m_primarySeries->setMesh(QAbstract3DSeries::MeshBevelBar);
    m_primarySeries->setMeshSmooth(false);



    m_graph->addSeries(m_primarySeries);

    changePresetCamera();

    resetNumberData(nm);

    //! [动画设置]
    Q3DCamera *camera = m_graph->scene()->activeCamera();
    m_defaultAngleX = camera->xRotation();
    m_defaultAngleY = camera->yRotation();
    m_defaultZoom = camera->zoomLevel();
    m_defaultTarget = camera->target();

    m_animationCameraX.setTargetObject(camera);
    m_animationCameraY.setTargetObject(camera);
    m_animationCameraZoom.setTargetObject(camera);
    m_animationCameraTarget.setTargetObject(camera);

    m_animationCameraX.setPropertyName("xRotation");
    m_animationCameraY.setPropertyName("yRotation");
    m_animationCameraZoom.setPropertyName("zoomLevel");
    m_animationCameraTarget.setPropertyName("target");

    int duration = 1700;
    m_animationCameraX.setDuration(duration);
    m_animationCameraY.setDuration(duration);
    m_animationCameraZoom.setDuration(duration);
    m_animationCameraTarget.setDuration(duration);


    qreal zoomOutFraction = 0.3;
    m_animationCameraX.setKeyValueAt(zoomOutFraction, QVariant::fromValue(0.0f));
    m_animationCameraY.setKeyValueAt(zoomOutFraction, QVariant::fromValue(90.0f));
    m_animationCameraZoom.setKeyValueAt(zoomOutFraction, QVariant::fromValue(50.0f));
    m_animationCameraTarget.setKeyValueAt(zoomOutFraction,
    QVariant::fromValue(QVector3D(0.0f, 0.0f, 0.0f)));

}


void GraphModifier::resetNumberData(QVector<int>nm)
{

    for(int i=0;i<18;i++)
    {
        for(int j=0;j<36;j++)
        {
            tempOulu[i][j]=nm[i*36+j];
        }
    }

    QBarDataArray *dataSet = new QBarDataArray;
    QBarDataArray *dataSet2 = new QBarDataArray;
    QBarDataRow *dataRow;
    QBarDataRow *dataRow2;

    dataSet->reserve(m_latitude.size());
    for (int y = 0; y < m_latitude.size(); y++) {

        dataRow = new QBarDataRow(m_longtitude.size());
        dataRow2 = new QBarDataRow(m_longtitude.size());
        for (int m = 0; m < m_longtitude.size(); m++) {

            (*dataRow)[m].setValue(tempOulu[y][m]);
        }

        dataSet->append(dataRow);
        dataSet2->append(dataRow2);
    }


    m_primarySeries->dataProxy()->resetArray(dataSet, m_latitude, m_longtitude);
}



void GraphModifier::changePresetCamera()
{

    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();


    m_graph->scene()->activeCamera()->setTarget(QVector3D(0.0f, 0.0f, 0.0f));

    static int preset = Q3DCamera::CameraPresetFront;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
}



void GraphModifier::changeLabelBackground()
{
    m_graph->activeTheme()->setLabelBackgroundEnabled(
        !m_graph->activeTheme()->isLabelBackgroundEnabled());
}


void GraphModifier::zoomToSelectedBar()
{
    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();

    Q3DCamera *camera = m_graph->scene()->activeCamera();
    float currentX = camera->xRotation();
    float currentY = camera->yRotation();
    float currentZoom = camera->zoomLevel();
    QVector3D currentTarget = camera->target();

    m_animationCameraX.setStartValue(QVariant::fromValue(currentX));
    m_animationCameraY.setStartValue(QVariant::fromValue(currentY));
    m_animationCameraZoom.setStartValue(QVariant::fromValue(currentZoom));
    m_animationCameraTarget.setStartValue(QVariant::fromValue(currentTarget));


    QPoint selectedBar = m_graph->selectedSeries()
                             ? m_graph->selectedSeries()->selectedBar()
                             : QBar3DSeries::invalidSelectionPosition();

    if (selectedBar != QBar3DSeries::invalidSelectionPosition()) {

        QVector3D endTarget;
        float xMin = m_graph->columnAxis()->min();
        float xRange = m_graph->columnAxis()->max() - xMin;
        float zMin = m_graph->rowAxis()->min();
        float zRange = m_graph->rowAxis()->max() - zMin;

        endTarget.setX((selectedBar.y() - xMin) / xRange * 2.0f - 1.0f);
        endTarget.setZ((selectedBar.x() - zMin) / zRange * 2.0f - 1.0f);

        qreal endAngleX = 90.0 - qRadiansToDegrees(qAtan(qreal(endTarget.z() / endTarget.x())));

        if (endTarget.x() > 0.0f)
            endAngleX -= 180.0f;
        float barValue = m_graph->selectedSeries()->dataProxy()->itemAt(selectedBar.x(),selectedBar.y())->value();

        float endAngleY = barValue >= 0.0f ? 30.0f : -30.0f;
        if (m_graph->valueAxis()->reversed())
            endAngleY *= -1.0f;


        m_animationCameraX.setEndValue(QVariant::fromValue(float(endAngleX)));
        m_animationCameraY.setEndValue(QVariant::fromValue(endAngleY));

        m_animationCameraZoom.setEndValue(QVariant::fromValue(250));
        m_animationCameraTarget.setEndValue(QVariant::fromValue(endTarget));
    } else {

        m_animationCameraX.setEndValue(QVariant::fromValue(m_defaultAngleX));
        m_animationCameraY.setEndValue(QVariant::fromValue(m_defaultAngleY));
        m_animationCameraZoom.setEndValue(QVariant::fromValue(m_defaultZoom));
        m_animationCameraTarget.setEndValue(QVariant::fromValue(m_defaultTarget));
    }

    m_animationCameraX.start();
    m_animationCameraY.start();
    m_animationCameraZoom.start();
    m_animationCameraTarget.start();
}





//show year
void GraphModifier::changeRange(int range)
{
    if (range >= m_latitude.count())
        m_latitudeAxis->setRange(0, m_latitude.count() - 1);
    else
        m_latitudeAxis->setRange(range, range);
}
//change bar style
void GraphModifier::changeStyle(int style)
{

    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {

        m_barMesh = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        m_primarySeries->setMesh(m_barMesh);
    }
}

void GraphModifier::changeSelectionMode(int selectionMode)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        int flags = comboBox->itemData(selectionMode).toInt();
        m_graph->setSelectionMode(QAbstract3DGraph::SelectionFlags(flags));
    }
}

void GraphModifier::changeTheme(int theme)
{
    Q3DTheme *currentTheme = m_graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
    emit fontSizeChanged(currentTheme->font().pointSize());
}


void GraphModifier::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    m_graph->setShadowQuality(sq);
    emit shadowQualityChanged(quality);
}

void GraphModifier::changeFont(const QFont &font)
{
    QFont newFont = font;
    m_graph->activeTheme()->setFont(newFont);
}

void GraphModifier::changeFontSize(int fontsize)
{
    m_fontSize = fontsize;
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
}

void GraphModifier::changeLabelRotation(int rotation)
{
    m_numAxis->setLabelAutoRotation(float(rotation));
    m_longtitudeAxis->setLabelAutoRotation(float(rotation));
    m_latitudeAxis->setLabelAutoRotation(float(rotation));
}



void GraphModifier::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
{
    int quality = int(sq);

    emit shadowQualityChanged(quality);
}


void GraphModifier::rotateX(int rotation)
{
    m_xRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}

void GraphModifier::rotateY(int rotation)
{
    m_yRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}


void GraphModifier::setBackgroundEnabled(int enabled)
{
    m_graph->activeTheme()->setBackgroundEnabled(bool(enabled));
}

void GraphModifier::setGridEnabled(int enabled)
{
    m_graph->activeTheme()->setGridEnabled(bool(enabled));
}

void GraphModifier::setSmoothBars(int smooth)
{
    m_smooth = bool(smooth);
    m_primarySeries->setMeshSmooth(m_smooth);
}


void GraphModifier::setReverseValueAxis(int enabled)
{
    m_graph->valueAxis()->setReversed(enabled);
}

void GraphModifier::setReflection(bool enabled)
{
    m_graph->setReflection(enabled);
}

void GraphModifier::setAxisTitleVisibility(bool enabled)
{
    m_numAxis->setTitleVisible(enabled);
    m_longtitudeAxis->setTitleVisible(enabled);
    m_latitudeAxis->setTitleVisible(enabled);
}

void GraphModifier::setAxisTitleFixed(bool enabled)
{
    m_numAxis->setTitleFixed(enabled);
    m_longtitudeAxis->setTitleFixed(enabled);
    m_latitudeAxis->setTitleFixed(enabled);
}

