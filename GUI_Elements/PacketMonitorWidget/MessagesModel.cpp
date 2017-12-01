#include "MessagesModel.h"
#include <cassert>

uint64_t PacketEntry::get_id() const
{
    static std::uint64_t packet_id = 1000;
    return packet_id++;
}

std::string PacketEntry::get_title() const
{
    return "Message 27.1 - id : " + std::to_string(get_id());
}

MessagesModel::MessagesModel()
{
    selected_plots_.push_back(
                PlotDescriptionRaw
                {
                    {
                        static_cast<std::uint8_t>(245),
                        static_cast<std::uint8_t>(100),
                        static_cast<std::uint8_t>(1),
                        static_cast<std::uint8_t>(0),
                        static_cast<std::uint8_t>(0)
                    },
                    1154,
                    4021,
                    1010,
                    8,
                    12530,
                    56,
                    3
                }
                );
    selected_plots_.push_back(
                PlotDescriptionRaw
                {
                    {
                        static_cast<std::uint8_t>(245),
                        static_cast<std::uint8_t>(100),
                        static_cast<std::uint8_t>(1),
                        static_cast<std::uint8_t>(0),
                        static_cast<std::uint8_t>(0)
                    },
                    1154,
                    4021,
                    1010,
                    8,
                    12530,
                    56,
                    3
                }
                );
}

int MessagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return journal_.size();
}

int MessagesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    auto& t = journal_[row];

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    if (role == Qt::DisplayRole)
    {
        switch(col)
        {
        case 0:
        {
            std::uint64_t process_time = 0;
            memcpy(&process_time, &t.raw_msg.process_time[0], 5);
            return process_time;
        }
        case 1:
            return t.raw_msg.polarization_type;
        case 2:
            return t.raw_msg.search_area_id;
        case 3:
        {
            auto& g = t.raw_msg.signal_amp;
            return QString("[ ") +
                    QString::number(g[0]) + QString(" ") +
                    QString::number(g[1]) + QString(" ") +
                    QString::number(g[2]) + QString(" ") +
                    QString::number(g[3]) + QString(" ") +
                    QString::number(g[4]) + QString(" ") +
                    QString::number(g[5]) + QString(" ") +
                    QString::number(g[6]) + QString(" ") +
                    QString::number(g[7]) + QString(" ") +
                    QString(" ]");
        }
        case 4:
            return t.raw_msg.plots_count;
        }
    }

    return QVariant();
}

QVariant MessagesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString("Process time");
            case 1:
                return QString("Polarization");
            case 2:
                return QString("Search ID");
            case 3:
                return QString("Gain array");
            case 4:
                return QString("Number of plots");
            }
        }
    }
    return QVariant();
}

void MessagesModel::add_entry(SearchResult_MsgRaw &msg)
{
    PacketEntry entry;
    entry.raw_msg = msg;
    entries_list_.add(QString::fromStdString(entry.get_title()));

    beginInsertRows(QModelIndex(), rowCount() + 1, rowCount() + 1);
    journal_.push_back(entry);
    endInsertRows();
}

void MessagesModel::log(const QString &log_text)
{
    emit log_update(log_text);
}

MessagesListModel *MessagesModel::get_list_model()
{
    return &entries_list_;
}

QList<PlotDescriptionRaw> &MessagesModel::get_plots()
{
    return selected_plots_;
}

void MessagesModel::change_plot_mode()
{
    decoded_ = !decoded_;
}

void MessagesModel::select_plots(QModelIndex idx)
{
    selected_plots_.clear();
    for (auto& plot : journal_[idx.row()].raw_msg.p)
        selected_plots_.push_back(plot);

    emit set_plots(decoded_);
}
