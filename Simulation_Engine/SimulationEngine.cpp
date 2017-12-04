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
#include "ModelEngine.h"
#include "ConfigManager.hpp"

int main(int argc, const char* argv[])
{
  try
  {
    ModelEngine engine;
    ConfigManager mng;
    mng.set_path(R"(../etc/)");
    mng.load_config("simulation");

    engine.load_events(mng);

    std::thread th(
    [&]()
    {
        engine.run();
    });

    std::cin.ignore();
    engine.stop();
    th.join();

    return 0;
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
