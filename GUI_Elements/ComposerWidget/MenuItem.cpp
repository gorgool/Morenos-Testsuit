#include "MenuItem.h"
#include "QLayout"
#include <QDebug>

MenuItem::MenuItem(const QString &title, const QString& icon_path, QWidget *display_widget) :
    QObject(),
    button_icon_(icon_path),
    menu_widget_()
{
    button_widget_.setText(title);
    button_widget_.setFixedHeight(50);
    button_widget_.setFixedWidth(50);
    button_widget_.setIcon(button_icon_);
    button_widget_.setIconSize(button_widget_.size());

    QLabel* title_widget = new QLabel(&menu_widget_);
    title_widget->setText(title);
    title_widget->setFixedHeight(20);
    title_widget->setAlignment(Qt::AlignHCenter);
    title_font_.setBold(true);
    title_font_.setPixelSize(16);
    title_font_.setFamily("Segoe UI Light");
    title_widget->setFont(title_font_);

    display_widget->setParent(&menu_widget_);

    menu_widget_.resize(display_widget->size());
    QVBoxLayout* layout = new QVBoxLayout(&menu_widget_);

    layout->addWidget(title_widget);
    layout->addWidget(display_widget);

    connect(get_button(), SIGNAL(clicked(bool)), this, SLOT(button_clicked(bool)));
}

QToolButton* MenuItem::get_button()
{
    return &button_widget_;
}

QWidget* MenuItem::get_widget()
{
    return &menu_widget_;
}

void MenuItem::button_clicked(bool)
{
    emit clicked(button_widget_.text());
}
