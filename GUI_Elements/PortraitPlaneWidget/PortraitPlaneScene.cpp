#include "PortraitPlaneScene.h"


PortraitPlaneScene::PortraitPlaneScene(QObject *parent) :
    QGraphicsScene(parent),
    targets_(nullptr),
    antenna_params_(nullptr)
{}

void PortraitPlaneScene::drawBackground(QPainter *p, const QRectF &rect)
{
    QRectF scene_size(rect);
    scene_size.setWidth(scene_size.width() - margin_ * 2);
    scene_size.setHeight(scene_size.height() - margin_ * 2);

    if (antenna_params_ == nullptr)
        return;

    auto params = antenna_params_->readGridParams();
    // -------------------------- Draw Axes -------------------------
    QPen axes_pen(Qt::black);
    axes_pen.setWidth(2);

    p->setPen(axes_pen);

    const int w_center = scene_size.width() / 2 + margin_;
    const int h_center = scene_size.height() / 2 + margin_;

    // Vertical line
    p->drawLine(w_center, scene_size.height() + margin_, w_center, 0 + margin_);
    // Horizontal line
    p->drawLine(0 + margin_, h_center, scene_size.width() + margin_, h_center);

    // ------------------------- Draw Grid --------------------------
    QPen grid_pen(Qt::black);
    grid_pen.setStyle(Qt::DashDotLine);

    p->setPen(grid_pen);

    for(int grid_index = 1; grid_index < std::max(params.width, params.height) / params.step + 1; ++grid_index)
    {
        // Vertical lines
        int grid_vertical_offset = grid_index * (scene_size.width() * params.step / params.width);
        if (grid_vertical_offset + w_center - margin_ < scene_size.width())
        {
            p->drawLine(w_center + grid_vertical_offset,
                        scene_size.height() + margin_,
                        w_center + grid_vertical_offset,
                        0 + margin_);
            p->drawLine(w_center - grid_vertical_offset,
                        scene_size.height() + margin_,
                        w_center - grid_vertical_offset,
                        0 + margin_);
        }

        // Horizontal line
        int grid_horizontal_offset = grid_index * (scene_size.height() * params.step / params.height);
        if (grid_horizontal_offset + h_center - margin_ < scene_size.height())
        {
            p->drawLine(0 + margin_,
                        h_center + grid_horizontal_offset,
                        scene_size.width() + margin_,
                        h_center + grid_horizontal_offset);
            p->drawLine(0 + margin_,
                        h_center - grid_horizontal_offset,
                        scene_size.width() + margin_,
                        h_center - grid_horizontal_offset);
        }
    }

    // --------------------------- Draw Text ------------------------------
    // "U" Axes label
    const int width_u_label_offset = -4;
    const int height_u_label_offset = -4;
    p->drawText(QPoint(scene_size.width() + width_u_label_offset, h_center + height_u_label_offset),"U");

    // "V" Axes label
    const int width_v_label_offset = -12;
    const int height_v_label_offset = margin_ + 12;
    p->drawText(QPoint(w_center + width_v_label_offset, 0 + height_v_label_offset),"V");

    // Grid left and right limits
    const int grid_width_left_offset = -margin_ + 4;
    const int grid_width_right_offset = 4;
    p->drawText(QPoint(0 + margin_ + grid_width_left_offset, h_center + 4),
                QString::number(-params.width/2, 'f', 2));
    p->drawText(QPoint(scene_size.width() + margin_ + grid_width_right_offset, h_center + 4),
                QString::number(params.width/2, 'f', 2));

    // Grid top and buttom limits
    const int grid_width_top_offset = -6;
    const int grid_width_buttom_offset = margin_ + 16;
    p->drawText(QPoint(w_center - 16, 0 + margin_ + grid_width_top_offset),
                QString::number(-params.height/2, 'f', 2));
    p->drawText(QPoint(w_center - 16, scene_size.height() + grid_width_buttom_offset),
                QString::number(params.height/2, 'f', 2));
}

void PortraitPlaneScene::drawForeground(QPainter *p, const QRectF &rect)
{
    if (targets_ == nullptr || antenna_params_ == nullptr)
        return;

    auto params = antenna_params_->readGridParams();

    QRectF scene_size(rect);
    scene_size.setWidth(scene_size.width() - margin_ * 2);
    scene_size.setHeight(scene_size.height() - margin_ * 2);

    const double marker_size = 8;

    QBrush target_brush(Qt::black);
    QPen target_pen(Qt::black);

    p->setPen(target_pen);
    p->setBrush(target_brush);

    for (const auto& item : targets_->get_targets())
    {
        int x = (scene_size.width() / 2 + margin_)
                + scene_size.width() * (item.u / params.width);
        int y = (scene_size.height() / 2 + margin_)
                - scene_size.height() * (item.v / params.height);
        p->drawEllipse(x - marker_size / 2, y - marker_size/2, marker_size, marker_size);
    }
}

void PortraitPlaneScene::set_model(TargetsModel* model)
{
    assert(model != nullptr);

    targets_ = model;
}

void PortraitPlaneScene::set_model(AntennaParamsModel* model)
{
    assert(model != nullptr);

    antenna_params_ = model;
}
