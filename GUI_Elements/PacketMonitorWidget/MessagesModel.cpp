#include "MessagesModel.h"
#include <cassert>
#include <QDateTime>
#include <QTextStream>
#include <QDir>

uint64_t PacketEntry::get_id() const
{
    return id;
}

std::string PacketEntry::get_title() const
{
    return "Message 27.1 - id : " + std::to_string(get_id());
}

uint64_t PacketEntry::generate_id()
{
    static std::uint64_t packet_id = 1000;
    return packet_id++;
}

MessagesModel::MessagesModel(const QString& reg_files_dir) : reg_files_dir_(reg_files_dir)
{}

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
            quint64 process_time = 0;
            memcpy(&process_time, &t.raw_msg.process_time[0], sizeof(t.raw_msg.process_time));
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

PacketEntry& MessagesModel::add_entry(SearchResult_MsgRaw &msg)
{
    PacketEntry entry;
    entry.raw_msg = msg;
    entry.id = PacketEntry::generate_id();
    entries_list_.add(QString::fromStdString(entry.get_title()));

    beginInsertRows(QModelIndex(), rowCount() + 1, rowCount() + 1);
    journal_.push_back(entry);
    endInsertRows();

    return journal_.last();
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

void MessagesModel::change_plot_mode(int)
{
    decoded_ = !decoded_;
    emit set_plots(decoded_);
}

void MessagesModel::select_plots(QModelIndex idx)
{
    selected_plots_.clear();
    for (auto& plot : journal_[idx.row()].raw_msg.p)
        selected_plots_.push_back(plot);

    emit set_plots(decoded_);
}

void MessagesModel::save_to_file()
{
    QString timestamp = QString::number(QDateTime::currentSecsSinceEpoch());

    if (!QDir(reg_files_dir_).exists())
        QDir().mkpath(reg_files_dir_);

    QFile msg_reg_file(reg_files_dir_ + "Message_27.1 - " + timestamp + ".reg");
    QFile plot_reg_file(reg_files_dir_ + "Plots_27.1 - " + timestamp + ".reg");

    if (msg_reg_file.open(QIODevice::WriteOnly) && plot_reg_file.open(QIODevice::WriteOnly))
    {
        msg_reg_file.setPermissions(QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ReadOther | QFileDevice::WriteOther);
        plot_reg_file.setPermissions(QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ReadOther | QFileDevice::WriteOther);

        QTextStream msg_stream(&msg_reg_file);
        QTextStream plot_stream(&plot_reg_file);
        //Headers
        msg_stream << "ID\tProcess Time\tPolarization\tSearch ID\tGain Coefficients\tPlots Number\n";
        plot_stream << "ID\tReference Time\tChannel ID\tPower\tU\tV\tU Variance\tV Variance\tFreqency Start\tFrequency Width\n";
        // Not thread safe, but journal size can not be less
        // than read before value and values can not be recolated.
        int last_idx = journal_.size();
        for (int idx = 0; idx < last_idx; ++idx)
        {
            auto& msg = journal_[idx].raw_msg;
            msg_stream << journal_[idx].get_id() << "\t"
                       << msg.process_time << "\t"
                       << msg.polarization_type << "\t"
                       << msg.search_area_id << "\t"
                       << "{ " << msg.signal_amp[0] << " "
                               << msg.signal_amp[1] << " "
                               << msg.signal_amp[2] << " "
                               << msg.signal_amp[3] << " "
                               << msg.signal_amp[4] << " "
                               << msg.signal_amp[5] << " "
                               << msg.signal_amp[6] << " "
                               << msg.signal_amp[7] << " }\t"
                       << msg.plots_count << "\n";

            for (std::size_t plot_idx = 0; plot_idx < msg.p.size(); ++plot_idx)
            {
                PlotDescription plot = decode(msg.p[plot_idx]);

                plot_stream << journal_[idx].get_id() << "\t"
                            << plot.referance_time << "\t"
                            << plot.channel_id << "\t"
                            << plot.u << "\t"
                            << plot.v << "\t"
                            << plot.u_var << "\t"
                            << plot.v_var << "\t"
                            << plot.freq_range_start << "\t"
                            << plot.freq_range_width << "\n";
            }

        }
    }
    else
        throw std::runtime_error("Failed to create reg file.");

}
