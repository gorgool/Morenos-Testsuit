#ifndef PLOTPOSITION_H
#define PLOTPOSITION_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QGraphicsView>

#include "PlotPositionScene.h"

class AntennaParamsModel;
class TargetsModel;

class PlotPosition : public QWidget
{
    Q_OBJECT

public:
    PlotPosition(const int width, const int height,
                 const QString& image_path, QWidget* parent = 0);
    ~PlotPosition();

    // Set data models. Overloads:
    //  - for list of targets, dinamic search areas, etc.
    //  - for antenna initial parameters (sector size, grid step, initial center direction)
    void set_model(TargetsModel* );
    void set_model(AntennaParamsModel* );

public slots:
    void redraw_all();
    void redraw_targets();

private:

    void resizeEvent(QResizeEvent *event);

    // Fonts
    QFont title_font_;
    QFont axes_font_;

    QGraphicsView graph_;
    QImage radar_image_;
    PlotPositionScene scene_;

    // Data Models
    TargetsModel* targets_;
    AntennaParamsModel* antenna_params_;
};

#endif // PLOTPOSITION_H
