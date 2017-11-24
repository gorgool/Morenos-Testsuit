#ifndef TARGETTABLE_H
#define TARGETTABLE_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QTableView>
#include <QTimer>

#include "DescriptionWidget.h"
#include "TargetsModel.h"

class AntennaParamsModel;
class TargetsModel;

class TargetTable : public QWidget
{
    Q_OBJECT

public:
    TargetTable(const int width, const int height,
                const int min_width = 500, const int min_height = 500,
                QWidget* parent = 0);

    ~TargetTable();

    // Set data models. Overloads:
    //  - for list of targets, dinamic search areas, etc.
    //  - for antenna initial parameters (sector size, grid step, initial center direction)
    void set_model(TargetsModel* );
    void set_model(AntennaParamsModel* );

private:
    // Fonts
    QFont title_font_;

    // Table
    QTableView table_;

    // Target information widget
    DescriptionWidget info_;

    // Data Models
    TargetsModel* targets_;
    AntennaParamsModel* antenna_params_;
};




#endif // TARGETTABLE_H
