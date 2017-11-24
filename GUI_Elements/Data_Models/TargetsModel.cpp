#include "TargetsModel.h"
#include <cassert>

#include <QDebug>
#include <algorithm>
#include <cassert>

TargetsModel::TargetsModel(const int cols, QObject *parent) :
    QAbstractTableModel(parent),
    cols_count_(cols),
    antenna_(nullptr)
{
    connect(this, SIGNAL(model_changed(int)), this, SLOT(update_view(int)));
}

int TargetsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return targets_.size();
}

int TargetsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return cols_count_;
}

QVariant TargetsModel::data(const QModelIndex &index, int role) const
{
    assert(antenna_ != nullptr);

    int row = index.row();
    int col = index.column();

    auto& t = targets_[row];

    if (role == Qt::DisplayRole)
    {
        switch(col)
        {
        case 0:
            return t.id;
        case 1:
            return antenna_->readDirection().azimuth + std::sin(t.u) * 180.0 / M_PI;
        case 2:
            return antenna_->readDirection().elevation + std::sin(t.v) * 180.0 / M_PI;
        case 3:
            return QString::number(t.freq_start) + " - " + QString::number(t.freq_start + t.freq_width);
        }
    }

    return QVariant();
}

QVariant TargetsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString("ID");
            case 1:
                return QString("Azimuth, deg");
            case 2:
                return QString("Elevation, deg");
            case 3:
                return QString("Frequency, MHz");
            }
        }
    }
    return QVariant();
}

void TargetsModel::update_target(const Target &t)
{
    QMutexLocker lock_guard(&m_);

    validate_target(t);

    auto it = std::find_if(targets_.begin(), targets_.end(), [&](auto& val) { return val.id == t.id; });
    if (it != targets_.end())
    {
        *it = t;
        int idx = std::distance(targets_.begin(), it);
        emit model_changed(idx);
    }
    else
    {
        if (targets_.empty())
        {
            beginResetModel();
            targets_.push_back(t);
            endResetModel();
        }
        else
        {
            beginInsertRows(QModelIndex(), rowCount() + 1, rowCount() + 1);
            targets_.push_back(t);
            endInsertRows();
        }
        emit model_changed(-1);
    }
}

void TargetsModel::remove_target(const int id)
{
    QMutexLocker lock_guard(&m_);

    auto it = std::find_if(targets_.begin(), targets_.end(), [&](auto& val) { return val.id == id; });
    if (it != targets_.end())
    {
        int row_idx = std::distance(targets_.begin(), it);
        beginRemoveRows(QModelIndex(), row_idx, row_idx);
        targets_.erase(it);
        endRemoveRows();
        emit target_removed(it->id);
    }
}

QList<Target>::iterator TargetsModel::get_target(int idx)
{
    assert(idx < targets_.size());

    return targets_.begin() + idx;
}

void TargetsModel::set_antenna_params(AntennaParamsModel *antenna_params)
{
    assert(antenna_params != nullptr);

    antenna_ = antenna_params;
}

const QList<Target> &TargetsModel::get_targets() const
{
    return targets_;
}

void TargetsModel::update_view(int idx)
{
    if (idx == -1)
        emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1));
    else
        emit dataChanged(createIndex(idx, 0), createIndex(idx, columnCount() - 1));
}

void TargetsModel::validate_target(const Target &t)
{
    assert(std::abs(t.u) <= 1.0);
    assert(std::abs(t.v) <= 1.0);
    assert(t.r > 0.0);

    return;
}
