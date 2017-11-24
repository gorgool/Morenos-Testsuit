#include "TargetsModelAdapter.h"

TargetsModelAdapter::TargetsModelAdapter()
{}

TargetsModelAdapter::TargetsModelAdapter(const SearchResult_Msg &msg, const int id, TargetType type)
{
    update(msg, id, type);
}

void TargetsModelAdapter::update(const SearchResult_Msg &msg, const int id, TargetType type)
{
    for (const PlotDescription& t : msg.p)
    {
        Target target
        {
            id,
            t.u,
            t.v,
            1000000.0,
            PolarizationType::Vertical,
            msg.search_area_id,
            t.variance,
            t.channel_id,
            t.power,
            t.freq_range_start,
            t.freq_range_width,
            type
        };

        model_.update_target(target);
    }
}

TargetsModel* TargetsModelAdapter::get_model()
{
    return &model_;
}
