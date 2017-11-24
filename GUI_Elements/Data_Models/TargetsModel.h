#pragma once

#include <QAbstractTableModel>
#include <QTableView>
#include <QMutex>
#include <QMutexLocker>

#include "Target.h"
#include "AntennaParamsModel.h"

class TargetsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TargetsModel(const int cols = 4, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // Update target. If there is no target with such id add new entry to targets list.
    void update_target(const Target& t);

    // Remove target by its id. If there is no target with given id do nothing.
    void remove_target(const int id);

    // Return iterator to target for given id. Terminate on if there is no such target (assert).
    QList<Target>::iterator get_target(int idx);

    // Setup anetenna params
    void set_antenna_params(AntennaParamsModel* antenna_params);

    // Get list of targets
    const QList<Target>& get_targets() const;

signals:
    void model_changed(int idx);
    void target_removed(int target_id);

public slots:
    void update_view(int idx);

private:
    // Number of visible parameters
    int cols_count_;

    // Targets container
    QList<Target> targets_;

    QMutex m_;

    // Antenna params
    AntennaParamsModel* antenna_;

    // Check given target. Terminate program if validation fails.
    void validate_target(const Target& t);
};
