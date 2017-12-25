#include "PlotPositionScene.h"

PlotPositionScene::PlotPositionScene(QImage& radar_image, QObject *parent) :
    QGraphicsScene(parent),
    targets_(nullptr),
    antenna_params_(nullptr),
    radar_image_(radar_image.scaled(160, 100))
{}

void PlotPositionScene::drawBackground(QPainter *p, const QRectF &rect)
{
    QRectF scene_size(rect);
    const int horizontal_margin = margin_ * 3;
    const int vertical_margin = margin_ * 2;
    scene_size.setWidth(scene_size.width() - horizontal_margin);
    scene_size.setHeight(scene_size.height() - vertical_margin);

    if (antenna_params_ == nullptr)
        return;

    auto params = antenna_params_->readGridParams();

    // ------------------------- Draw Radar Icon -------------------------------------------
    p->drawImage(scene_size.width() / 2 - radar_image_.width() / 2 + horizontal_margin / 2,
                 scene_size.height() - bottom_offset_ + vertical_margin,
                 radar_image_);


    // ------------------------- Draw upper arc, borders and center lines -------------------
    QPen axes_pen(Qt::black);
    axes_pen.setWidth(2);

    p->setPen(axes_pen);

    // Left border line
    {
        auto screen_coords_1 = transform_coords(-params.width / 2,
                                                params.range_max,
                                                QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));
        auto screen_coords_2 = transform_coords(-params.width / 2,
                                                params.range_min,
                                                QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

        p->drawLine(screen_coords_1.x + horizontal_margin / 2, screen_coords_1.y + vertical_margin,
                    screen_coords_2.x + horizontal_margin / 2, screen_coords_2.y + vertical_margin);
    }

    // Right border line
    {
        auto screen_coords_1 = transform_coords(params.width / 2,
                                                params.range_max,
                                                QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));
        auto screen_coords_2 = transform_coords(params.width / 2,
                                                params.range_min,
                                                QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

        p->drawLine(screen_coords_1.x + horizontal_margin / 2, screen_coords_1.y + vertical_margin,
                    screen_coords_2.x + horizontal_margin / 2, screen_coords_2.y + vertical_margin);
    }

    axes_pen.setStyle(Qt::DashLine);
    p->setPen(axes_pen);
    // Center line
    {
        auto screen_coords_1 = transform_coords(0,
                                                params.range_max,
                                                QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));
        auto screen_coords_2 = transform_coords(0,
                                                params.range_min,
                                                QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

        p->drawLine(screen_coords_1.x + horizontal_margin / 2, screen_coords_1.y + vertical_margin,
                    screen_coords_2.x + horizontal_margin / 2, screen_coords_2.y + vertical_margin);
    }

    // --------------------------- Draw grid ---------------------------------
    QPen grid_pen(Qt::black);
    grid_pen.setStyle(Qt::DashDotLine);
    p->setPen(grid_pen);

    // Vertical "angle" lines
    for(int grid_index = 1; grid_index < params.width / 2 / params.step; ++grid_index)
    {
        // To the right
        {
            auto screen_coords_1 = transform_coords(params.step * grid_index,
                                                    params.range_max,
                                                    QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));
            auto screen_coords_2 = transform_coords(params.step * grid_index,
                                                    params.range_min,
                                                    QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

            if (screen_coords_1.x < scene_size.width() && screen_coords_1.y < scene_size.height() &&
                screen_coords_2.x < scene_size.width() && screen_coords_2.y < scene_size.height())
            {
                p->drawLine(screen_coords_1.x + horizontal_margin / 2, screen_coords_1.y + vertical_margin,
                            screen_coords_2.x + horizontal_margin / 2, screen_coords_2.y + vertical_margin);
            }
        }
        // To the left
        {
            auto screen_coords_1 = transform_coords(-params.step * grid_index,
                                                    params.range_max,
                                                    QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));
            auto screen_coords_2 = transform_coords(-params.step * grid_index,
                                                    params.range_min,
                                                    QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

            if (screen_coords_1.x < scene_size.width() && screen_coords_1.y < scene_size.height() &&
                screen_coords_2.x < scene_size.width() && screen_coords_2.y < scene_size.height())
            {
                p->drawLine(screen_coords_1.x + horizontal_margin / 2, screen_coords_1.y + vertical_margin,
                            screen_coords_2.x + horizontal_margin / 2, screen_coords_2.y + vertical_margin);
            }
        }
    }

    // Horizontal "range" lines
    for(int range_index = 0; range_index < params.range_max / params.range_step; ++range_index)
    {
        QPainterPath path;

        auto start_point = transform_coords(-params.width / 2,
                                            range_index * params.range_step + params.range_min,
                                            QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

        path.moveTo(start_point.x + horizontal_margin / 2, start_point.y + vertical_margin);

        for(int dir_index = 1; dir_index < params.width / params.step; ++dir_index)
        {
            auto next_point = transform_coords(-params.width / 2 + params.step * dir_index,
                                               range_index * params.range_step + params.range_min,
                                               QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

            path.quadTo(next_point.x + horizontal_margin / 2, next_point.y + vertical_margin,
                        next_point.x + horizontal_margin / 2, next_point.y + vertical_margin);

        }

        auto end_point = transform_coords(params.width / 2,
                                          range_index * params.range_step + params.range_min,
                                          QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

        path.quadTo(end_point.x + horizontal_margin / 2, end_point.y + vertical_margin,
                    end_point.x + horizontal_margin / 2, end_point.y + vertical_margin);

        p->drawPath(path);
    }

    // --------------------------- Draw Text ---------------------------------
    // Direction text
    auto center_point = transform_coords(0, params.range_max,
                                         QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));
    p->drawText(QPoint(center_point.x + margin_ - 30, center_point.y + 2*margin_ - 20),
                "Azimuth - " + QString::number(antenna_params_->readDirection().azimuth, 'f', 3));

    // Max range text
    auto left_point = transform_coords(params.width / 2, params.range_max,
                                       QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

    p->drawText(QPoint(left_point.x + horizontal_margin / 2 - 30, left_point.y + vertical_margin - 20),
                QString::number(params.range_max, 'f', 1));

    // Draw range labels
    for(int range_index = 0; range_index < params.range_max / params.range_step; ++range_index)
    {
        auto left_point = transform_coords(params.width / 2, range_index * params.range_step + params.range_min,
                                           QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

        p->drawText(QPoint(left_point.x + horizontal_margin / 2 + 10, left_point.y + vertical_margin),
                QString::number(range_index * params.range_step + params.range_min, 'f', 1));
    }
}

void PlotPositionScene::drawForeground(QPainter *p, const QRectF &rect)
{
    if (targets_ == nullptr || antenna_params_ == nullptr)
        return;

    QRectF scene_size(rect);
    const int horizontal_margin = margin_ * 3;
    const int vertical_margin = margin_ * 2;
    scene_size.setWidth(scene_size.width() - horizontal_margin);
    scene_size.setHeight(scene_size.height() - vertical_margin);

    const double marker_size = 8;

    QBrush target_brush(Qt::black);
    QPen target_pen(Qt::black);

    QColor interferanse_color(Qt::darkGray);
    interferanse_color.setAlpha(127);
    QPen interferance_pen(interferanse_color);
    interferance_pen.setWidth(6);

    for (const auto& item : targets_->get_targets())
    {
        if (item.type == TargetType::Interference)
        {
            p->setPen(interferance_pen);

            if (antenna_params_ == nullptr)
                return;

            auto params = antenna_params_->readGridParams();
            auto target_begin = transform_coords(item.u, params.range_min,
                                                 QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));
            auto target_end = transform_coords(item.u, params.range_max,
                                                 QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

            p->drawLine(target_begin.x + horizontal_margin / 2, target_begin.y + vertical_margin,
                        target_end.x + horizontal_margin / 2, target_end.y + vertical_margin);

        }
        else
        {
            p->setPen(target_pen);
            p->setBrush(target_brush);

            auto target_point = transform_coords(item.u, item.r,
                                                 QRectF(0, 0, scene_size.width(), scene_size.height() - bottom_offset_));

            p->drawEllipse(target_point.x + horizontal_margin / 2 - marker_size/2, target_point.y + vertical_margin - marker_size/2, marker_size, marker_size);
        }
    }
}

void PlotPositionScene::set_model(TargetsModel* model)
{
    assert(model != nullptr);

    targets_ = model;
}

void PlotPositionScene::set_model(AntennaParamsModel* model)
{
    assert(model != nullptr);

    antenna_params_ = model;
}

ScreenCoords PlotPositionScene::transform_coords(const double u, const double R, const QRectF& coordinate_grid)
{
    assert(antenna_params_ != nullptr);
    assert(R >= antenna_params_->readGridParams().range_min);
    assert(R <= antenna_params_->readGridParams().range_max);
    assert(std::abs(u) <= antenna_params_->readGridParams().width / 2);

    const double R_norm = R / antenna_params_->readGridParams().range_max;
    double U_norm = u / (antenna_params_->readGridParams().width / 2) * std::sin(30.0 * M_PI / 180.0);

    int x = (0.5 + R_norm * U_norm) * coordinate_grid.width();
    int y = (1 - R_norm * std::cos(std::sin(U_norm))) * coordinate_grid.height();

    return ScreenCoords{x,y};
}
