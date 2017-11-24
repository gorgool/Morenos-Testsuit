#ifndef PORTRAITPLANESCENE_H
#define PORTRAITPLANESCENE_H

#include <QGraphicsScene>
#include <QPainter>

#include "AntennaParamsModel.h"
#include "TargetsModel.h"

class PortraitPlaneScene : public QGraphicsScene
{
public:

    PortraitPlaneScene(QObject* parent = 0);

    void drawBackground(QPainter* p, const QRectF& rect);

    void drawForeground(QPainter* p, const QRectF& rect);

    void set_model(TargetsModel* model);

    void set_model(AntennaParamsModel* model);

private:

    TargetsModel* targets_;
    AntennaParamsModel* antenna_params_;

    const int margin_ = 36;
};


#endif // PORTRAITPLANESCENE_H
