#include "TargetsTable.h"
#include <QHeaderView>
#include <QDebug>

TargetTable::TargetTable(const int width, const int height, const int min_width, const int min_height, QWidget *parent) :
    QWidget(parent),
    table_(this),
    targets_(nullptr),
    antenna_params_(nullptr)
{
    resize(width, height);
    setMinimumHeight(min_height);
    setMinimumWidth(min_width);
    setContentsMargins(0,0,0,0);

    // Title
    QLabel* title = new QLabel("Targets Table", this);
    title->setAlignment(Qt::AlignHCenter);
    title_font_.setBold(true);
    title_font_.setPixelSize(16);
    title_font_.setFamily("Segoe UI Light");
    title->setFont(title_font_);

    // Table
    QWidget* table_widget = new QWidget(this);
    QVBoxLayout* v_layout = new QVBoxLayout(table_widget);
    v_layout->setSpacing(5);
    v_layout->setContentsMargins(0,0,0,0);
    v_layout->addWidget(title);
    v_layout->addWidget(&table_);

    // Description widget
    QHBoxLayout* h_layout = new QHBoxLayout(this);
    h_layout->setSpacing(5);
    h_layout->setContentsMargins(0,0,0,0);
    h_layout->addWidget(table_widget);
    h_layout->addWidget(&info_);

    table_.setSelectionBehavior(QAbstractItemView::SelectRows);
    table_.setSelectionMode(QAbstractItemView::SingleSelection);
    table_.setAlternatingRowColors(true);

    connect(&table_, &QTableView::clicked,
            [this](QModelIndex idx)
    {
        this->info_.set_target(targets_->get_target(idx.row()));
    });

    setStyleSheet("selection-background-color: grey;");
}

TargetTable::~TargetTable() {}

void TargetTable::set_model(TargetsModel* target_model)
{
    assert(target_model != nullptr);

    targets_ = target_model;
    table_.setModel(target_model);

    if (antenna_params_ != nullptr)
    {
        targets_->set_antenna_params(antenna_params_);
    }

    connect(target_model, SIGNAL(target_removed(int)), &info_, SLOT(remove_target(int)));
    connect(target_model, SIGNAL(model_changed(int)), &info_, SLOT(update()));

    for (int c = 0; c < table_.horizontalHeader()->count(); ++c)
    {
        table_.horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
}

void TargetTable::set_model(AntennaParamsModel* model)
{
    assert(model != nullptr);
    antenna_params_ = model;

    if (targets_ != nullptr)
    {
        targets_->set_antenna_params(model);
    }
}
