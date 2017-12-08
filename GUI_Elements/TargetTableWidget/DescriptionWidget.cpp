#include "DescriptionWidget.h"
#include <QFrame>
#include <QDebug>

DescriptionWidget::DescriptionWidget(QWidget *parent) :
    QWidget(parent),
    target_setted_(false)
{
    const int widget_width = 300;
    setMinimumWidth(widget_width);
    setMaximumWidth(widget_width);

    info_font_.setPixelSize(14);
    info_font_.setFamily("Segoe UI Light");

    // Title
    QLabel* title = new QLabel("Target Description", this);
    title->setAlignment(Qt::AlignHCenter);
    title_font_.setBold(true);
    title_font_.setPixelSize(16);
    title_font_.setFamily("Segoe UI Light");
    title->setFont(title_font_);

    // Layout
    QVBoxLayout* v_layout = new QVBoxLayout(this);
    v_layout->setContentsMargins(0,0,0,0);
    v_layout->setSpacing(4);
    v_layout->addWidget(title);

    QFrame* horizontal_line = new QFrame(this);
    horizontal_line->setFrameShape(QFrame::HLine);
    v_layout->addWidget(horizontal_line);

    // Fields

    auto label_setup_helper = [&](const QString& field_name, QLabel& value)
    {
        QLabel* name_label = new QLabel(this);
        name_label->setText(field_name + " : ");
        name_label->setFont(info_font_);
        value.setFont(info_font_);
        QWidget* label = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(label);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setAlignment(Qt::AlignLeft);
        layout->addWidget(name_label);
        layout->addWidget(&value);
        v_layout->addWidget(label);
    };

    // ID
    label_setup_helper("Target ID", target_id_);
    // Polarization
    label_setup_helper("Polarization", polarization_value_);
    // Search Area
    label_setup_helper("Search area ID", search_area_value_);
    // Coordinates
    label_setup_helper("U", u_coordinate_value_);
    label_setup_helper("V", v_coordinate_value_);
    // U Variance
    label_setup_helper("U Variance", u_sigma_value_);
    // V Variance
    label_setup_helper("V Variance", v_sigma_value_);
    // Range
    label_setup_helper("Slant range", range_);
    // Channel
    label_setup_helper("Channel ID", channel_value_);
    // Signal power
    label_setup_helper("Power", power_value_);
    // Frequency
    label_setup_helper("Frequency range start", frequency_start_value_);
    label_setup_helper("Frequency range width", frequency_width_value_);

    v_layout->addStretch();

    reset();
}

void DescriptionWidget::update()
{
    display();
}

void DescriptionWidget::remove_target(int target_id)
{
    if (target_setted_ == false)
        return;

    if (target_id == target_->id)
    {
        reset_model();
    }
}

void DescriptionWidget::set_target(QList<Target>::iterator t)
{
    target_ = t;
    target_setted_ = true;
}

void DescriptionWidget::reset()
{
    target_id_.setText(" - ");
    polarization_value_.setText(" - ");
    search_area_value_.setText(" - ");
    u_coordinate_value_.setText(" - ");
    v_coordinate_value_.setText(" - ");
    u_sigma_value_.setText(" - ");
    v_sigma_value_.setText(" - ");
    range_.setText(" - ");
    channel_value_.setText(" - ");
    power_value_.setText(" - ");
    frequency_start_value_.setText(" - ");
    frequency_width_value_.setText(" - ");
}

void DescriptionWidget::reset_model()
{
    target_setted_ = false;
}

void DescriptionWidget::display()
{
    if (target_setted_ == false)
    {
        reset();
        return;
    }

    static QMap<PolarizationType, QString> polarization_translate{{
            {PolarizationType::Vertical, "Vertical"},
            {PolarizationType::Horizontal, "Horizontal"},
            {PolarizationType::Elliptic, "Elliptic"},
            {PolarizationType::Undefined, "Undefined"}
    }};

    target_id_.setText(QString::number(target_->id));
    polarization_value_.setText(polarization_translate[target_->polarization]);
    search_area_value_.setText(QString::number(target_->search_area_id));
    u_coordinate_value_.setText(QString::number(target_->u));
    v_coordinate_value_.setText(QString::number(target_->v));
    u_sigma_value_.setText(QString::number(target_->u_sigma));
    v_sigma_value_.setText(QString::number(target_->v_sigma));

    if (target_->type == TargetType::Interference)
        range_.setText(QString::number(target_->r, 'd', 3));
    else
        range_.setText("Undefined");

    channel_value_.setText(QString::number(target_->channel_id));
    power_value_.setText(QString::number(target_->power));
    frequency_start_value_.setText(QString::number(target_->freq_start));
    frequency_width_value_.setText(QString::number(target_->freq_width));
}
