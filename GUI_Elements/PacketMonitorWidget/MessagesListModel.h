#pragma once

#include <QAbstractListModel>

class MessagesListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MessagesListModel();

    // QAbstractItemModel interface

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    void add(const QString& entry);

signals:
    void model_changed(int idx);

private slots:
    void update_view(int idx);

private:
    QStringList data_;
};
