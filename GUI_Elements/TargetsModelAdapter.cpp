#include "TargetsModelAdapter.h"

TargetsModelAdapter::TargetsModelAdapter()
{}

TargetsModelAdapter::TargetsModelAdapter(const SearchResult_Msg &msg)
{
    update(msg);
}

void TargetsModelAdapter::update(const SearchResult_Msg &msg)
{
    int id = 10001;
    const TargetType type = TargetType::Interference;

    for (std::size_t idx = 0; idx < msg.p.size(); ++idx)
    {
        auto& t = msg.p[idx];
        Target target
        {
            id++,
            t.u,
            t.v,
            1000000.0,
            PolarizationType::Vertical,
            msg.search_area_id,
            t.u_var,
            t.v_var,
            t.channel_id,
            t.power,
            t.freq_range_start,
            t.freq_range_width,
            type
        };

        model_.update_target(target);
    }

    // Remove not updated targets
    if (msg.p.size() < static_cast<std::size_t>(model_.get_targets().size()))
    {
        const std::size_t number_of_elements = model_.get_targets().size();
        for (std::size_t idx = msg.p.size(); idx < number_of_elements; ++idx)
            model_.remove_target(id++);
    }
}

TargetsModel* TargetsModelAdapter::get_model()
{
    return &model_;
}
