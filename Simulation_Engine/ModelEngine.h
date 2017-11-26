#pragma once

#include <cinttypes>
#include <chrono>
#include <iostream>

#include "EventQueue.h"
#include "SimulationState.h"

/*
  Model engine. Control execution of the model.
*/

class ModelEngine
{
  // Event queue.
  EventQueue ev_queue_;

  // Current modelling time, microseconds.
  std::uint64_t model_time_ = 0;

  // Modelling step, microseconds.
  std::uint64_t time_step_ = 100;

  EnvironmentState state_;

public:

  // Constructor.
  ModelEngine() {}

  // Execution of "times" modelling cycles. 
  void run();

  void load_events(const std::string& file_path);
};
