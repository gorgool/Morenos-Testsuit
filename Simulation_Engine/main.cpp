#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <atomic>
#include <stdexcept>
#include <exception>
#include <cmath>

#include "SearchResult_Msg.h"
#include "PacketManager.h"
#include "SimulationEngine.h"

int main(int argc, const char* argv[])
{
  std::ofstream reg_data("reg.dat");

  if (!reg_data.good())
  {
    std::cerr << "Error opening file.\n";
    return 1;
  }

  reg_data << "Time\tPolarization\tSignal_Amp\tSearch_ID\tPlot_Number\tTarget_u\tTarget_v\n";

  try
  {
    PacketSender sender;
    SimulationEngine engine;
    TargetEngine target;

    target
      .set_channel(5)
      .set_frequency(35.24, 12.365)
      .set_power(12345)
      .set_variance(0.005)
      .set_coordinates(0.05, -0.1, 0.0015, -0.02);


    engine
      .set_gain({ 10.0, 10.5, 11.0, 12.0, 13.0, 100.0, 101.0, 102.0 })
      .set_polarization(1)
      .set_search_id(66)
      .set_target(std::move(target));

    std::uint64_t time = 10000;
    std::atomic_bool running(true);

    std::thread msg_thread([&]()
    {
      while (running.load())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        engine.advance(time);

        auto msg = engine.get_message();
        if (validate(msg))
          sender.send_packet(msg);
        else
          return 1;

        reg_data
          << msg.process_time << "\t"
          << static_cast<int>(msg.polarization_type) << "\t"
          << "{"
          << msg.signal_amp[0] << ","
          << msg.signal_amp[1] << ","
          << msg.signal_amp[2] << ","
          << msg.signal_amp[3] << ","
          << msg.signal_amp[4] << ","
          << msg.signal_amp[5] << ","
          << msg.signal_amp[6] << ","
          << msg.signal_amp[7]
          << "}\t"
          << static_cast<int>(msg.search_area_id) << "\t"
          << static_cast<int>(msg.plots_count) << "\t"
          << msg.p[0].u << "\t"
          << msg.p[0].v << "\n";

        time += 200000;
        time = time % (static_cast<std::uint64_t>(std::pow(2, 40) - 1));
      }
      return 0;
    });

    std::cin.ignore();
    running = false;

    msg_thread.join();
  }
  catch (const std::runtime_error& ex)
  {
    std::cerr << "Execution stopped.\n";
    std::cerr << ex.what() << std::endl;
  }
  catch (const std::invalid_argument& ex)
  {
    std::cerr << "Execution stopped.\n";
    std::cerr << ex.what() << std::endl;
  }

  return 0;
}