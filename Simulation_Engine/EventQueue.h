#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <cinttypes>

class EnvironmentState;

/**
 * @struct Event
 * 
 * @brief Simulation event. Execute specific function in the triiger time.
 * 
 */
struct Event
{
  /** @brief Activation time, microseconds. */
  std::uint64_t trigger_time;

  /** @brief Function to execute in trigger time. */
  std::function<void(EnvironmentState&)> exec;

  Event(std::uint64_t _trigger_time, std::function<void(EnvironmentState&)>&& callback) : 
    trigger_time(_trigger_time), exec(callback) {}

  /** @brief Overloaded less operator for priority queue. */
  friend bool operator<(const Event& lhs, const Event& rhs)
  {
    return lhs.trigger_time > rhs.trigger_time;
  }
};

/**
 * @class EventQueue
 * 
 * @brief Container for the events objects 
 * 
 */
class EventQueue
{
  /** @brief Simulation events queue. Sorted by trigger time. */
  std::priority_queue<Event> event_queue;
public:

  EventQueue() {}

  /**
   * @fn  void add_event(const Event& ev);
   *
   * @brief Add event to the priority queue.
   *
   * @param ev Reference to the event.
   */
  void add_event(const Event& ev)
  {
    event_queue.push(ev);
  }

  /**
   * @fn  std::vector<Event> get_events(const std::uint64_t time);
   *
   * @brief Return all events which triiger time equal to given time.
   *
   * @param time Trigger time for events. It must be strictly equal to trigger time.
   * 
   * @return Vector of events. If there are no such events return empty vector.
   */
  std::vector<Event> get_events(const std::uint64_t time)
  {
    std::vector<Event> ret;

    if (event_queue.empty())
      return ret;

    while (true)
    {
      if (event_queue.empty())
        break;

      auto ev = event_queue.top();
      if (ev.trigger_time == time)
      {
        ret.push_back(ev);
        event_queue.pop();
      }
      else
      {
        break;
      }
    }

    return ret;
  }

  std::size_t size() const
  {
      return event_queue.size();
  }

/**
   * @fn  void EthernetHeader::serialize(unsigned char* buf);
   *
   * @brief Serialize ethernet protocol header in the given buffer.
   *
   * @param buf Pointer to the buffer to serialize into. Terminate on null.
   */
  void clear()
  {
      event_queue = std::priority_queue<Event>();
  }
};
