#pragma once

#include <QWidget>
#include <QLayout>
#include <QHash>
#include <QToolButton>
#include <QGroupBox>

#include "MenuItem.h"

class Composer : public QWidget
{
    Q_OBJECT

public:
    Composer(QWidget* parent = 0);
    ~Composer();
    void add_menu_item(const QString& title, const QString& icon_path, QWidget* display_widget);
    void set_exit_icon(const QString& icon_path);
signals:
    void display_widget(QString title);

private slots:
    void set_widget(QString title);
    void exit_application();

private:

    QHBoxLayout main_layout_;

    // Left side menu
    QVBoxLayout side_menu_layout_;
    QHash<QString, MenuItem*> menu_;

    // Exit button
    QToolButton exit_button_;
    QIcon exit_icon_;
};
