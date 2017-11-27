#ifndef DESCRIPTIONWIDGET_H
#define DESCRIPTIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QFont>
#include <QSpacerItem>

#include "Target.h"

class DescriptionWidget : public QWidget
{
    Q_OBJECT
public:
    DescriptionWidget(QWidget* parent = 0);

    void set_target(QList<Target>::iterator t);

public slots:
    void update();
    void remove_target(int target_idx);

private:

    // Reset information with " - "
    void reset();

    // Reset target model pointer
    void reset_model();

    // Display information
    void display();

    // Description of one target
    QList<Target>::iterator target_;
    bool target_setted_;

    // Font
    QFont title_font_;
    QFont info_font_;

    // Parameters values labels
    QLabel target_id_;
    QLabel polarization_value_;
    QLabel search_area_value_;
    QLabel u_coordinate_value_;
    QLabel v_coordinate_value_;
    QLabel sigma_value_;
    QLabel range_;
    QLabel channel_value_;
    QLabel power_value_;
    QLabel frequency_start_value_;
    QLabel frequency_width_value_;
};


#endif // DESCRIPTIONWIDGET_H
