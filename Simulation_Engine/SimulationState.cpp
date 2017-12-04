#include "SimulationState.h"
#include <cmath>
#include <algorithm>

TargetState::TargetState() :
    prev_timestamp_(0), initial_u_(0.0), initial_v_(0.0), current_u_(0.0),
    current_v_(0.0), u_vel_(0.0), v_vel_(0.0),
    channel_id_(0), target_id_(0), variance_(0.0), power_(0),
    freq_range_start_(0.0), freq_range_width_(0.0) {}

TargetState & TargetState::set_coordinates(double u, double v, double u_vel, double v_vel)
{
	if (std::abs(u) > 1.0) throw std::invalid_argument("u is out of range.");
	if (std::abs(v) > 1.0) throw std::invalid_argument("v is out of range.");

    initial_u_ = u;
    initial_v_ = v;
    current_u_ = u;
    current_v_ = v;
    u_vel_ = u_vel;
    v_vel_ = v_vel;

	return *this;
}

TargetState & TargetState::set_channel(std::uint8_t id)
{
	if (id > 8 || id < 1) throw std::invalid_argument("channel id is out of range.");

    channel_id_ = id;

    return *this;
}

TargetState &TargetState::set_id(uint32_t id)
{
    target_id_ = id;

    return *this;
}

TargetState & TargetState::set_variance(double variance)
{
	if (std::abs(variance) > 1.0) throw std::invalid_argument("variance is too large.");

    variance_ = variance;

	return *this;
}

TargetState & TargetState::set_power(std::uint16_t power)
{
    power_ = power;

	return *this;
}

TargetState & TargetState::set_frequency(double start, double width)
{
	if (start < 0.0 || start > 115.0) throw std::invalid_argument("start frequency is out of range.");
	if (width < 0.0 || width > 115.0) throw std::invalid_argument("width of the frequency range is out of range.");

    freq_range_start_ = start;
    freq_range_width_ = width;

	return *this;
}

void TargetState::advance(std::uint64_t time)
{
    auto time_delta = time - prev_timestamp_;

	// If time delta greater than threshold preserve current coordinate state
    if (time_delta < delta_threshold_)
	{
        current_u_ += u_vel_ * (time_delta * 1.0e-3);
        current_v_ += v_vel_ * (time_delta * 1.0e-3);

		// If coordinates larger than 1.0 wrap it
        if (std::abs(current_u_) > 1.0) current_u_ = current_u_ - std::copysign(1.0, current_u_);
        if (std::abs(current_v_) > 1.0) current_v_ = current_v_ - std::copysign(1.0, current_v_);
	}

    prev_timestamp_ = time;
}

void TargetState::reset()
{
    current_u_ = initial_u_;
    current_v_ = initial_v_;
}

PlotDescription TargetState::get_message() const
{
    PlotDescription ret;

    ret.channel_id = channel_id_;
    ret.freq_range_start = freq_range_start_;
    ret.freq_range_width = freq_range_width_;
    ret.power = power_;
    ret.u = current_u_;
    ret.v = current_v_;
    ret.variance = variance_;
    ret.referance_time = prev_timestamp_;

	return ret;
}

EnvironmentState::EnvironmentState() :
    timestamp_(0), polarization_type_(0), signal_amp_{ 0 },
    search_area_id_(0), u_limit_left_(1.0), u_limit_right_(1.0),
    v_limit_left_(1.0), v_limit_right_(1.0)
{}

EnvironmentState & EnvironmentState::set_polarization(std::uint8_t polarization)
{
	if (polarization != 0 && polarization != 1) throw std::invalid_argument("polarization type is out of range.");

    polarization_type_ = polarization;

	return *this;
}

EnvironmentState & EnvironmentState::set_search_id(std::uint8_t id)
{
    search_area_id_ = id;

	return *this;
}

EnvironmentState & EnvironmentState::set_gain(std::array<double, 8> gain)
{
	for (std::size_t idx = 0; idx < 8; ++idx)
	{
		if (gain[idx] < 0.0 || gain[idx] > 120.0) throw std::invalid_argument("gain is out of range.");
        signal_amp_[idx] = gain[idx];
	}

    return *this;
}

EnvironmentState &EnvironmentState::set_uv_limits(const double u_left, const double u_right, const double v_left, const double v_right)
{
    if (std::abs(u_left) > 1.0  ||
        std::abs(v_left) > 1.0  ||
        std::abs(u_right) > 1.0 ||
        std::abs(v_right) > 1.0)
                throw std::invalid_argument("uv limits is out of range.");

    u_limit_left_ = u_left;
    u_limit_right_ = u_right;
    v_limit_left_ = v_left;
    v_limit_right_ = v_right;

    return *this;
}

bool EnvironmentState::check_limits(const double u, const double v) const
{
    return  u > u_limit_left_ && u < u_limit_right_ &&
            v > v_limit_left_ && v < v_limit_right_;
}

EnvironmentState & EnvironmentState::set_target(TargetState && target)
{
    targets_.push_back(std::forward<TargetState>(target));

	return *this;
}

EnvironmentState & EnvironmentState::set_target(const TargetState& target)
{
    targets_.push_back(target);

    return *this;
}

void EnvironmentState::remove_target(const uint32_t id)
{
    auto it = std::find_if(targets_.begin(), targets_.end(),
                           [&](const TargetState& val) { return id == val.get_id(); });

    if (it != targets_.end())
        targets_.erase(it);
}

void EnvironmentState::advance(std::uint64_t time)
{
    for (auto& t : targets_)
    {
		t.advance(time);
        // Wrap coordinates, to keep target in scan area.
        if (check_limits(t.get_u(), t.get_v()) == false)
            t.reset();
    }

    timestamp_ = time;
}

SearchResult_Msg EnvironmentState::get_message() const
{
	SearchResult_Msg ret;

    ret.polarization_type = polarization_type_;
    ret.search_area_id = search_area_id_;

	for (std::size_t idx = 0; idx < 8; ++idx)
        ret.signal_amp[idx] = signal_amp_[idx];

    ret.plots_count = static_cast<std::uint8_t>(targets_.size());
    ret.process_time = timestamp_;
    ret.transfer_time = timestamp_;

    for (auto t : targets_)
		ret.p.push_back(t.get_message());

	return ret;
}
