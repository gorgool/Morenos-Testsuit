#include "ModelEngine.h"
#include "ConfigManager.hpp"
#include <thread>
#include <chrono>

void ModelEngine::run()
{
    running_ = true;

    while(running_.load())
    {
        // Get event for current time.
        auto events = ev_queue_.get_events(model_time_);

        if (!events.empty())
        {
            // Execute events.
            for (auto& ev : events)
                ev.exec(state_);
        }

        state_.advance(model_time_);

        auto msg = state_.get_message();

        if (validate(msg) == false)
        {
            throw SystemException("Model Enginee: message validation fail.");
            return;
        }

        sender_.send_packet(msg);

        model_time_ += time_step_;
        std::this_thread::sleep_for(std::chrono::milliseconds(time_step_));
    }
}

void ModelEngine::stop()
{
    running_ = false;
}

void ModelEngine::load_events()
{
        ConfigManager mng;
        mng.set_path(R"(../etc/)");
        mng.load_config("simulation");

        auto& params = mng.get_section("params");

        state_.set_gain({0, 0, 0, 0, 0, 0, 0, 0});
        state_.set_polarization(mng.get_value<std::uint32_t>(params, "polarization"));
        state_.set_search_id(mng.get_value<std::uint32_t>(params, "search_area_id"));
        state_.set_uv_limits(
            mng.get_value<double>(params, "u_left_limit"),
            mng.get_value<double>(params, "u_right_limit"),
            mng.get_value<double>(params, "v_left_limit"),
            mng.get_value<double>(params, "v_right_limit")
            );

        model_time_ = mng.get_value<std::uint64_t>(params, "model_start_time");
        time_step_ = mng.get_value<std::uint64_t>(params, "model_delta_time");

        auto& events = mng.get_section("events");

        auto target_events = mng.get_object_array(events,"target_events");
        for (auto& ev : target_events)
        {
            auto event_type = mng.get_value<std::string>(*ev, "type");

            if (event_type == "target_add")
            {
                TargetState target;

                target
                    .set_channel(mng.get_value<std::uint32_t>(*ev, "channel_id"))
                    .set_frequency(mng.get_value<double>(*ev, "freq_start"), mng.get_value<double>(*ev, "freq_width"))
                    .set_power(mng.get_value<std::uint32_t>(*ev, "power"))
                    .set_variance(mng.get_value<double>(*ev, "variance"))
                    .set_coordinates(mng.get_value<double>(*ev, "u"), mng.get_value<double>(*ev, "v"),
                                     mng.get_value<double>(*ev, "u_vel"), mng.get_value<double>(*ev, "v_vel"));

                ev_queue_.add_event(
                    Event(mng.get_value<std::uint64_t>(*ev, "trigger_time"),
                          [target](EnvironmentState& state)
                          {
                            state.set_target(target);
                          }));
            }
            else
            {
                throw ConfigException("Model Engine: Unknown type of an event.");
            }
        }
}
