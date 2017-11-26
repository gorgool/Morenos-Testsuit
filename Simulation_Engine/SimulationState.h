#pragma once

#include <cinttypes>
#include <stdexcept>
#include <iterator>
#include <array>

#include "SearchResult_Msg.h"

class TargetState
{
public:

    TargetState();

    TargetState& set_coordinates(double u, double v, double u_vel, double v_vel);
    TargetState& set_channel(std::uint8_t id);
    TargetState& set_variance(double variance);
    TargetState& set_power(std::uint16_t power);
    TargetState& set_frequency(double start, double width);

	void advance(std::uint64_t time);
    void reset();
	PlotDescription get_message() const;

    double get_u() { return current_u_; }
    double get_v() { return current_v_; }

private:

    const std::uint64_t delta_threshold_ = 1000000;

    std::uint64_t prev_timestamp_;

    double initial_u_;
    double initial_v_;

    double current_u_;
    double current_v_;

    double u_vel_;
    double v_vel_;

    std::uint8_t channel_id_;

    double variance_;

    std::uint16_t power_;

    double freq_range_start_;
    double freq_range_width_;
};

class EnvironmentState
{
public:

    EnvironmentState();

    EnvironmentState& set_polarization(std::uint8_t polarization);
    EnvironmentState& set_search_id(std::uint8_t id);
    EnvironmentState& set_gain(std::array<double, 8> gain);
    EnvironmentState& set_target(TargetState&& target);
    EnvironmentState& set_target(const TargetState& target);
    EnvironmentState& set_uv_limits(const double u_left, const double u_right, const double v_left, const double v_right);
    bool check_limits(const double u, const double v) const;

	void advance(std::uint64_t time);

	SearchResult_Msg get_message() const;

private:

    std::uint64_t timestamp_;

    std::vector<TargetState> targets_;

    std::uint8_t polarization_type_;

    double signal_amp_[8];

    std::uint8_t search_area_id_;

    double u_limit_left_;
    double u_limit_right_;
    double v_limit_left_;
    double v_limit_right_;
};
