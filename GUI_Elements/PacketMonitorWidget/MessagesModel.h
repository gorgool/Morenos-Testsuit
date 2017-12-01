#pragma once

#include <QAbstractTableModel>
#include <QStringListModel>

#include <QList>
#include "SearchResult_Msg.h"
#include "MessagesListModel.h"

struct PacketEntry
{
    std::uint64_t id;
    SearchResult_MsgRaw raw_msg;
    std::uint64_t get_id() const;
    std::string get_title() const;
    static std::uint64_t generate_id();
};

class MessagesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MessagesModel(const QString& reg_files_dir);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    PacketEntry& add_entry(SearchResult_MsgRaw& msg);
    void log(const QString& log_text);

    MessagesListModel* get_list_model();
    QList<PlotDescriptionRaw>& get_plots();

signals:
    void log_update(QString);
    void set_plots(bool);

public slots:
    void change_plot_mode(int);
    void select_plots(QModelIndex idx);
    void save_to_file();

private:
    QList<PacketEntry> journal_;
    MessagesListModel entries_list_;

    bool decoded_ = false;
    QList<PlotDescriptionRaw> selected_plots_;

    QString reg_files_dir_;
};

