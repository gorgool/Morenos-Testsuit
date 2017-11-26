#include "Composer.h"
#include <QDebug>

Composer::Composer(QWidget *parent) :
    QWidget(parent), main_layout_(this)
{
    main_layout_.addLayout(&side_menu_layout_);
    side_menu_layout_.setContentsMargins(0, 0, 0, 50);
    side_menu_layout_.addSpacing(100);
    side_menu_layout_.addStretch();

    exit_button_.setFixedHeight(50);
    exit_button_.setFixedWidth(50);
    side_menu_layout_.addWidget(&exit_button_);

    connect(&exit_button_, SIGNAL(clicked(bool)), this, SLOT(exit_application()));
    connect(this, SIGNAL(display_widget(QString)), this, SLOT(set_widget(QString)));
}

Composer::~Composer()
{
    for(auto item : menu_)
    {
        delete item;
    }
}

void Composer::add_menu_item(const QString& title, const QString& icon_path, QWidget* display_widget)
{
    MenuItem* item = new MenuItem(title, icon_path, display_widget);
    menu_.insert(title, item);
    side_menu_layout_.insertWidget(side_menu_layout_.count() - 2, item->get_button());
    connect(item, SIGNAL(clicked(QString)), this, SLOT(set_widget(QString)));

    // Setup first menu widget as default
    if (menu_.size() == 1)
    {
        emit Composer::display_widget(menu_.begin().key());
    }
}

void Composer::set_exit_icon(const QString &icon_path)
{
    exit_icon_.addFile(icon_path);
    exit_button_.setIcon(exit_icon_);
    exit_button_.setIconSize(exit_button_.size());
}

void Composer::set_widget(QString title)
{
    // Remove previous item. Assume we have only 2 items (left menu and right widget) and previous widget at index 1

    if (main_layout_.count() > 1)
    {
        static_cast<QWidgetItem*>(main_layout_.takeAt(1))->widget()->hide();
        main_layout_.removeItem(main_layout_.takeAt(1));
    }

    main_layout_.addWidget(menu_[title]->get_widget());
    menu_[title]->get_widget()->setVisible(true);
}

void Composer::exit_application()
{
    exit(0);
}
