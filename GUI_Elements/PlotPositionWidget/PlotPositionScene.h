#ifndef PLOTPOSITIONSCENE_H
#define PLOTPOSITIONSCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

#include "AntennaParamsModel.h"
#include "TargetsModel.h"

struct ScreenCoords
{
    int x,y;
};


class PlotPositionScene : public QGraphicsScene
{
public:

    PlotPositionScene(QImage& radar_image, QObject* parent = 0);

    void drawBackground(QPainter* p, const QRectF& rect);

    void drawForeground(QPainter* p, const QRectF& rect);

    void set_model(TargetsModel* model);

    void set_model(AntennaParamsModel* model);

    ScreenCoords transform_coords(const double u, const double R, const QRectF& coordinate_grid);

private:

    TargetsModel* targets_;
    AntennaParamsModel* antenna_params_;
    QImage radar_image_;

    const int margin_ = 36;
    const int bottom_offset_ = 100;
};

#endif // PLOTPOSITIONSCENE_H
