#include "PacketMonitor.h"

#include <QLabel>
#include <QLayout>
#include <cassert>

PacketMonitor::PacketMonitor(QWidget *parent) :
    QWidget(parent), model_(nullptr)
{
    QLabel* title = new QLabel("Packet Monitor", this);
    title->setFixedHeight(20);
    title->setAlignment(Qt::AlignHCenter);
    title_font_.setBold(true);
    title_font_.setPixelSize(16);
    title_font_.setFamily("Segoe UI Light");
    title->setFont(title_font_);

    decoded_check.setText("Decode plots info");
    save_button.setText("Save to file...");

    QVBoxLayout* v_layout = new QVBoxLayout(this);
    v_layout->setSpacing(5);
    v_layout->setContentsMargins(0,0,0,0);
    v_layout->addWidget(title);

    QGridLayout* layout = new QGridLayout;
    v_layout->addLayout(layout);

    layout->addWidget(&decoded_check, 0, 7, 1, 1);
    layout->addWidget(&save_button, 8, 0, 1, 1);
    // Incoming packets list
    incoming_packets_list_.setFixedWidth(240);
    incoming_packets_list_.setStyleSheet(
                "QListView::item:selected:active { background-color: grey; color: black;} \
                 QListView::item:selected:!active { background-color: grey; color: black; }");
    layout->addWidget(&incoming_packets_list_, 0, 0, 8, 1);

    // Center table
    incoming_packets_table_.setSelectionBehavior(QAbstractItemView::SelectRows);
    incoming_packets_table_.setSelectionMode(QAbstractItemView::SingleSelection);
    incoming_packets_table_.setAlternatingRowColors(true);
    incoming_packets_table_.setStyleSheet(
                "QTableView::item:selected:active { selection-background-color: grey; selection-color: black; } \
                 QTableView::item:selected:!active { selection-background-color: grey; selection-color: black; }");
    layout->addWidget(&incoming_packets_table_, 0, 1, 7, 4);

    // Log window
    log_widget_.setReadOnly(true);
    log_widget_.setFixedHeight(200);
    layout->addWidget(&log_widget_, 7, 1, 2, 4);

    // Plots Info
    plot_view_.setFixedWidth(300 + 10);
    plot_view_.setColumnCount(2);
    plot_view_.setHeaderLabels(QStringList({"Name", "Value"}));
    plot_view_.setColumnWidth(0, 200);
    plot_view_.setColumnWidth(1, 100);
    layout->addWidget(&plot_view_, 1, 7, 8, 1);
}

void PacketMonitor::set_model(MessagesModel *model)
{
    assert(model != nullptr);
    model_ = model;

    incoming_packets_list_.setModel(model_->get_list_model());
    incoming_packets_table_.setModel(model_);

    connect(&save_button, SIGNAL(clicked(bool)), model_, SLOT(save_to_file()));
    connect(&decoded_check, SIGNAL(stateChanged(int)), model_, SLOT(change_plot_mode(int)));
    connect(model_, SIGNAL(log_update(QString)), this, SLOT(append_log(QString)));
    connect(&incoming_packets_list_, SIGNAL(clicked(QModelIndex)), model_, SLOT(select_plots(QModelIndex)));
    connect(&incoming_packets_list_, SIGNAL(clicked(QModelIndex)), this, SLOT(select_table(QModelIndex)));
    connect(&incoming_packets_table_, SIGNAL(clicked(QModelIndex)), model_, SLOT(select_plots(QModelIndex)));
    connect(&incoming_packets_table_, SIGNAL(clicked(QModelIndex)), this, SLOT(select_list(QModelIndex)));
    connect(model_, SIGNAL(set_plots(bool)), this, SLOT(display_plots(bool)));

    incoming_packets_table_.setColumnWidth(0, 100);
    incoming_packets_table_.setColumnWidth(1, 100);
    incoming_packets_table_.setColumnWidth(2, 80);
    incoming_packets_table_.setColumnWidth(3, 260);
    incoming_packets_table_.setColumnWidth(4, 130);
}

void PacketMonitor::append_log(QString log_msg)
{
    log_widget_.appendPlainText(log_msg);
}

void PacketMonitor::display_plots(bool decoded)
{
    auto& plots = model_->get_plots();
    plot_view_.clear();
    for(int idx = 0; idx < plots.size(); ++idx)
    {
        QTreeWidgetItem *plotItem = new QTreeWidgetItem(&plot_view_);
        plotItem->setExpanded(true);
        plotItem->setText(0, "Plot " + QString::number(idx + 1));

        // =========== Fields of one plot ================
        PlotDescription decoded_plot = decode(plots[idx]);

        // Referance time
        {
            QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
            fieldItem->setText(0, "Reference time");
            if (decoded)
            {
                fieldItem->setText(1, QString::number(decoded_plot.referance_time));
            }
            else
            {
                std::uint64_t ref_time = 0;
                memcpy(&ref_time, &plots[idx].referance_time[0], sizeof(plots[idx].referance_time));
                fieldItem->setText(1, QString::number(ref_time));
            }
            plotItem->addChild(fieldItem);
        }
        // Channel ID
        {
            QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
            fieldItem->setText(0, "Channel ID");
            if (decoded)
                fieldItem->setText(1, QString::number(decoded_plot.channel_id));
            else
                fieldItem->setText(1, QString::number(plots[idx].channel_id));
            plotItem->addChild(fieldItem);
        }
        // Power
        {
            QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
            fieldItem->setText(0, "Power");
            if (decoded)
                fieldItem->setText(1, QString::number(decoded_plot.power));
            else
                fieldItem->setText(1, QString::number(plots[idx].power));
            plotItem->addChild(fieldItem);
        }
        // U coordinate
        {
            QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
            fieldItem->setText(0, "U coordinate");
            if (decoded)
                fieldItem->setText(1, QString::number(decoded_plot.u));
            else
                fieldItem->setText(1, QString::number(plots[idx].u));
            plotItem->addChild(fieldItem);
        }
        // V coordinate
        {
            QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
            fieldItem->setText(0, "V coordinate");
            if (decoded)
                fieldItem->setText(1, QString::number(decoded_plot.v));
            else
                fieldItem->setText(1, QString::number(plots[idx].v));
            plotItem->addChild(fieldItem);
        }
        // Variance
        {
            QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
            fieldItem->setText(0, "Variance");
            if (decoded)
                fieldItem->setText(1, QString::number(decoded_plot.variance));
            else
                fieldItem->setText(1, QString::number(plots[idx].variance));
            plotItem->addChild(fieldItem);
        }
        // Frequency
        {
            QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
            fieldItem->setText(0, "Frequency start");
            if (decoded)
                fieldItem->setText(1, QString::number(decoded_plot.freq_range_start));
            else
                fieldItem->setText(1, QString::number(plots[idx].freq_range_start));
            plotItem->addChild(fieldItem);
        }
        {
            QTreeWidgetItem *fieldItem = new QTreeWidgetItem();
            fieldItem->setText(0, "Frequency width");
            if (decoded)
                fieldItem->setText(1, QString::number(decoded_plot.freq_range_width));
            else
                fieldItem->setText(1, QString::number(plots[idx].freq_range_width));
            plotItem->addChild(fieldItem);
        }
    }
}

void PacketMonitor::select_list(QModelIndex idx)
{
    incoming_packets_list_.setCurrentIndex(model_->get_list_model()->index(idx.row()));
}

void PacketMonitor::select_table(QModelIndex idx)
{
    incoming_packets_table_.setCurrentIndex(model_->index(idx.row(), 0));
}
