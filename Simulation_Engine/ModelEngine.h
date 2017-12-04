#pragma once

#include <cinttypes>
#include <iostream>
#include <atomic>

#include "EventQueue.h"
#include "SimulationState.h"
#include "PacketManager.h"
#include "ConfigManager.hpp"

/**
 * @class ModelEngine
 * 
 * @brief Controls execution and internal state of a model 
 * 
 */
class ModelEngine
{
  /** @brief Network packets manager. Used for sending 27.1 message. */
  PacketSender sender_;

  /** @brief Event queue. */
  EventQueue ev_queue_;

  /** @brief Current modelling time, milliseconds. */
  std::uint64_t model_time_ = 0;

  /** @brief Modelling step, milliseconds. */
  std::uint64_t time_step_ = 100;

  /** @brief Internal state of the model. */
  EnvironmentState state_;

  /** @brief Flag for start and stop state of execution. */
  std::atomic_bool running_;

public:

  ModelEngine() : running_(false) {}

  /**
   * @fn void run();
   *
   * @brief Start execution of the model in real time with time_step_. Blocking.
   */
  void run();

  /**
   * @fn void stop();
   *
   * @brief Stop execution of the model. Internal state preserved.
   */
  void stop();

  /**
   * @fn void load_events(const ConfigManager& params);
   *
   * @brief Loads events descriptions and execution parameters from ConfigManager settings.
   *
   * @param params Reference to config section with events description.
   */
  void load_events(const ConfigManager& params);
};
