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
    Composer(const int width, const int height, QWidget* parent = 0);
    ~Composer();
    void add_menu_item(const QString& title, const QString& icon_path, QWidget* display_widget);

signals:
    void display_widget(QString title);

private slots:
    void set_widget(QString title);

private:

    QHBoxLayout main_layout_;

    // Left side menu
    QVBoxLayout side_menu_layout_;
    QHash<QString, MenuItem*> menu_;
};
