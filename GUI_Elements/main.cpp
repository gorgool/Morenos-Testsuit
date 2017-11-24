#include <QApplication>
#include <QDebug>
#include <thread>
#include <chrono>
#include <mutex>

#include "Composer.h"
#include "PacketManager.h"
#include "GUI_Elements.h"
#include "TargetsModelAdapter.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    PacketReceiver* receiver = new PacketReceiver;
    AntennaParamsModel* antenna_model = new AntennaParamsModel;
    TargetsModelAdapter* targets_model = new TargetsModelAdapter;

    antenna_model->updateGridParams(0.4, 0.6, 0.02, 20000.0, 300000.0, 50000.0);
    antenna_model->updateDirection(15.24, 25.02);

    Composer window(1200, 700);

    {
        QWidget* wrapper = new QWidget;
        wrapper->setContentsMargins(0,0,0,0);
        QHBoxLayout* h_layout = new QHBoxLayout(wrapper);
        h_layout->setSpacing(10);
        h_layout->setContentsMargins(0,0,0,0);
        PlotPosition* ppi = new PlotPosition(500, 800, 500, 800,
                                             "D:\\Development\\GUI RCS\\UI_Elements\\Common\\Res\\radar_icon.png", wrapper);
        ppi->set_model(antenna_model);
        ppi->set_model(targets_model->get_model());
        h_layout->addWidget(ppi);
        QVBoxLayout* v_layout = new QVBoxLayout;
        v_layout->setSpacing(10);
        v_layout->setContentsMargins(0,0,0,0);
        PortraitPlane* pp = new PortraitPlane(1000, 300, 1000, 300, wrapper);
        pp->set_model(antenna_model);
        pp->set_model(targets_model->get_model());
        TargetTable* tt = new TargetTable(1000, 500, 1000, 500, wrapper);
        tt->set_model(antenna_model);
        tt->set_model(targets_model->get_model());
        v_layout->addWidget(pp);
        v_layout->addWidget(tt);
        h_layout->addLayout(v_layout);

        window.add_menu_item("Observe", "D:\\Development\\GUI RCS\\UI_Elements\\Common\\Res\\observe_icon3.ico", wrapper);
    }

    window.show();

    std::thread message_thread([&]()
    {
        while(true)
        {
            try
            {
                SearchResult_Msg msg;
                auto ret = receiver->wait_packet(msg);
                if (ret == 0)
                    continue;

                targets_model->update(msg, 10001, TargetType::Interference);
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
