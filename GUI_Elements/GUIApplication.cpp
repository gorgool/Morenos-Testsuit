#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>

#include "Composer.h"
#include "PacketManager.h"
#include "GUI_Elements.h"
#include "TargetsModelAdapter.h"
#include "ConfigManager.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    PacketReceiver* receiver = new PacketReceiver;
    AntennaParamsModel* antenna_model = new AntennaParamsModel;
    TargetsModelAdapter* targets_model = new TargetsModelAdapter;
    MessagesModel* messages;

    antenna_model->updateGridParams(1.0, 1.0, 0.04, 10000.0, 1.5e6, 1.5e5);
    antenna_model->updateDirection(15.24, 25.02);

    std::string res_path;
    try
    {
        ConfigManager cfg_manager;
        cfg_manager.set_path(R"(../etc/)");
        cfg_manager.load_config("morenos_settings");
        auto& gui_settings = cfg_manager.get_section("gui");
        res_path = cfg_manager.get_value<std::string>(gui_settings, "res_path");
        messages = new MessagesModel(QString::fromStdString(cfg_manager.get_value<std::string>(gui_settings, "reg_path")));
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what();
        return -1;
    }

    Composer window;
    window.set_exit_icon(QString::fromStdString(res_path + "exit_icon.png"));

    {
        QWidget* wrapper = new QWidget;
        wrapper->setContentsMargins(0,0,0,0);
        QHBoxLayout* h_layout = new QHBoxLayout(wrapper);
        h_layout->setSpacing(10);
        h_layout->setContentsMargins(0,0,0,0);

        PlotPosition* ppi = new PlotPosition(300, 500,
                                             QString::fromStdString(res_path + "radar_icon.png"), wrapper);

        ppi->set_model(antenna_model);
        ppi->set_model(targets_model->get_model());
        h_layout->addWidget(ppi);
        h_layout->setStretchFactor(ppi, 3);

        QVBoxLayout* v_layout = new QVBoxLayout;
        v_layout->setSpacing(10);
        v_layout->setContentsMargins(0,0,0,0);

        PortraitPlane* pp = new PortraitPlane(1000, 300, wrapper);
        pp->set_model(antenna_model);
        pp->set_model(targets_model->get_model());
        TargetTable* tt = new TargetTable(1000, 450, wrapper);
        tt->set_model(antenna_model);
        tt->set_model(targets_model->get_model());
        v_layout->addWidget(pp);
        v_layout->addWidget(tt);
        h_layout->addLayout(v_layout);
        h_layout->setStretchFactor(v_layout, 5);

        window.add_menu_item("Observe", QString::fromStdString(res_path + "observe_icon.ico"), wrapper);
    }
    {
        PacketMonitor* monitor = new PacketMonitor;
        monitor->set_model(messages);
        window.add_menu_item("Debug", QString::fromStdString(res_path + "debug_icon.png"), monitor);
    }

    window.showFullScreen();

    std::thread message_thread([&]()
    {
        while(true)
        {
            try
            {
                SearchResult_MsgRaw msg;
                auto ret = receiver->read_packet(msg);
                if (ret == 0)
                    continue;

                // Register message
                auto& e = messages->add_entry(msg);

                auto decoded_msg = decode(msg);
                if (validate(decoded_msg) == false)
                {
                    messages->log("Message validation failed. ID : " + QString::number(e.get_id()));
                    continue;
                }

                targets_model->update(decoded_msg);
            }
            catch(std::runtime_error& ex)
            {
                qDebug() << ex.what();
            }
        }
    });
    message_thread.detach();

    return app.exec();
}
