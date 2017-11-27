#ifndef PORTRAITPLANE_H
#define PORTRAITPLANE_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QGraphicsView>

#include "PortraitPlaneScene.h"

class AntennaParamsModel;
class TargetsModel;

class PortraitPlane : public QWidget
{
    Q_OBJECT

public:
    PortraitPlane(const int width, const int height,
                  QWidget* parent = 0);
    ~PortraitPlane();

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

    // Graph
    QGraphicsView graph_;
    PortraitPlaneScene scene_;

    // Data Models
    TargetsModel* targets_;
    AntennaParamsModel* antenna_params_;
};


#endif // PORTRAITPLANE_H
