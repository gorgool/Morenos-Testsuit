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

int main(int argc, const char* argv[])
{
  try
  {
    PacketSender sender;
    ModelEngine engine;
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
