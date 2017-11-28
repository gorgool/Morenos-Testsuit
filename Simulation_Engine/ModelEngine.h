#pragma once

#include <cinttypes>
#include <iostream>
#include <atomic>

#include "EventQueue.h"
#include "SimulationState.h"
#include "PacketManager.h"

/*
  Model engine. Control execution of the model.
*/

class ModelEngine
{
  // Network packets manager.
  PacketSender sender_;

  // Event queue.
  EventQueue ev_queue_;

  // Current modelling time, milliseconds.
  std::uint64_t model_time_ = 0;

  // Modelling step, milliseconds.
  std::uint64_t time_step_ = 100;

  EnvironmentState state_;

  std::atomic_bool running_;

public:

  // Constructor.
  ModelEngine() : running_(false) {}

  // Execution of modelling cycle. Blocking.
  void run();

  // Stop modelling execution.
  void stop();

  // Load model events from file.
  void load_events();
};
