#include "ModelEngine.h"

void ModelEngine::run()
{
    while(true)
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
        model_time_ += time_step_;

        // TODO RealTime mode - sleep
    }
}

void ModelEngine::load_events(const std::string &file_path)
{
    // TODO load simulation script
}
