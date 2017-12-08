#include "MessagesListModel.h"

MessagesListModel::MessagesListModel()
{
    connect(this, SIGNAL(model_changed(int)), this, SLOT(update_view(int)));
}

int MessagesListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return data_.size();
}

QVariant MessagesListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return data_[index.row()];
    }

    return QVariant();
}

void MessagesListModel::add(const QString &entry)
{
    data_.append(entry);
    emit model_changed(data_.size());
}

void MessagesListModel::update_view(int idx)
{
    emit dataChanged(createIndex(idx - 1,0), createIndex(idx,0));
}
