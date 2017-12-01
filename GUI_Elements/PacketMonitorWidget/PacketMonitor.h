#pragma once

#include <QWidget>
#include <QStringListModel>
#include <QPlainTextEdit>
#include <QTableView>
#include <QListView>
#include <QCheckBox>
#include <QPushButton>
#include <QTreeWidget>

#include "MessagesModel.h"

class PacketMonitor : public QWidget
{
    Q_OBJECT

public:
    PacketMonitor(QWidget* parent = 0);
    void set_model(MessagesModel* model);

private slots:
    void append_log(QString log_msg);
    void display_plots(bool decoded);
    void select_list(QModelIndex idx);
    void select_table(QModelIndex idx);

private:
    QFont title_font_;
    MessagesModel* model_;

    QTableView incoming_packets_table_;
    QListView incoming_packets_list_;

    QPlainTextEdit log_widget_;
    QTreeWidget plot_view_;

    QCheckBox decoded_check;
    QPushButton save_button;
};
