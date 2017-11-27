#include "PlotPosition.h"

PlotPosition::PlotPosition(const int width, const int height,
                           const QString& image_path, QWidget *parent) :
    QWidget(parent),
    graph_(this),
    radar_image_(image_path),
    scene_(radar_image_, this),
    targets_(nullptr),
    antenna_params_(nullptr)
{
    resize(width, height);
    setContentsMargins(0,0,0,0);

    QLabel* title = new QLabel("Plot Position Indicator", this);
    title->setAlignment(Qt::AlignHCenter);
    title_font_.setBold(true);
    title_font_.setPixelSize(16);
    title_font_.setFamily("Segoe UI Light");
    title->setFont(title_font_);

    axes_font_.setBold(true);
    axes_font_.setPixelSize(10);
    axes_font_.setFamily("Arial");

    graph_.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graph_.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graph_.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    QVBoxLayout* v_layout = new QVBoxLayout(this);
    v_layout->setSpacing(5);
    v_layout->setContentsMargins(0,0,0,0);
    v_layout->addWidget(title);
    v_layout->addWidget(&graph_);

    graph_.setScene(&scene_);
    graph_.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

PlotPosition::~PlotPosition() {}

void PlotPosition::set_model(TargetsModel* model)
{
    assert(model != nullptr);

    targets_ = model;
    scene_.set_model(model);
    connect(targets_, SIGNAL(model_changed(int)), this, SLOT(redraw_targets()));
}

void PlotPosition::set_model(AntennaParamsModel* model)
{
    assert(model != nullptr);

    antenna_params_ = model;
    scene_.set_model(model);
    connect(antenna_params_, SIGNAL(model_changed()), this, SLOT(redraw_all()));
}

void PlotPosition::redraw_all()
{
    scene_.setSceneRect(QRect(0,0,graph_.width(),graph_.height()));
    scene_.invalidate(graph_.sceneRect());
}

void PlotPosition::redraw_targets()
{
    scene_.invalidate(graph_.sceneRect(), QGraphicsScene::ForegroundLayer);
}

void PlotPosition::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    redraw_all();
}
