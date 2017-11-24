#pragma once

#include <cassert>
#include <cmath>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

// Grid parameters (all in UV space):
//  * width - vertical width of scan sector
//  * height - horizontal width of scan sector
//  * step - step of grid lines
struct GridParams
{
    double width;
    double height;
    double step;
    double range_max;
    double range_min;
    double range_step;
};

// Direction of antenna center (all in angular degrees).
struct Direction
{
    double azimuth;
    double elevation;
};

/*
 * Antenna parameters model. Consists of grid parameters and direction.
 * Using to display current scan sector and direction of antenna center.
 * After each update emit modelChanged signal.
 */

class AntennaParamsModel : public QObject
{
    Q_OBJECT

public:

    AntennaParamsModel() : grid_{0,0,0,0,0,0}, direction_{0,0} {}

    // Update direction. Parameters in angular degrees.
    void updateDirection(const double az, const double el)
    {
        assert(az >= 0.0 && az <= 360.0);
        assert(el >= -10.0 && el <= 90.0);

        QMutexLocker lock(&mutex_);

        direction_.azimuth = az;
        direction_.elevation = el;

        emit model_changed();
    }

    // Read current direction state.
    const Direction readDirection()
    {
        QMutexLocker lock(&mutex_);
        return direction_;
    }

    // Update grid parameters. Parameters in UV space.
    void updateGridParams(const double width = 0.0, const double height = 0.0, const double step = 0.0,
                          const double range_min = 0.0, const double range_max = 0.0, const double range_step = 0.0)
    {
        assert(width >= 0.0 && width <= 1.0);
        assert(height >= 0.0 && height <= 1.0);
        assert(step >= 0.0 && step < std::min(height, width));
        assert(range_min > 0.0);
        assert(range_max > range_min);
        assert(range_step > 0 && range_step < (range_max - range_min));

        QMutexLocker lock(&mutex_);

        grid_.height = height;
        grid_.width = width;
        grid_.step = step;

        grid_.range_max = range_max;
        grid_.range_min = range_min;
        grid_.range_step = range_step;

        emit model_changed();
    }

    // Read current grid parameters.
    const GridParams readGridParams()
    {
        QMutexLocker lock(&mutex_);
        return grid_;
    }

signals:

    void model_changed();

private:

    GridParams grid_;
    Direction direction_;
    QMutex mutex_;
};
