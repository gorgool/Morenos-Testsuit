#pragma once

#include <QString>
#include <QToolButton>
#include <QLabel>
#include <QObject>

class MenuItem : public QObject
{
    Q_OBJECT
public:
    MenuItem(const QString& title, const QString& icon_path, QWidget* display_widget);
    QToolButton* get_button();
    QWidget* get_widget();

public slots:
    void button_clicked(bool);

signals:
    void clicked(QString);

private:

    QIcon button_icon_;
    QFont title_font_;
    QToolButton button_widget_;
    QWidget menu_widget_;
};
