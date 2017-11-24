#pragma once

#include <cinttypes>
#include <stdexcept>
#include <iterator>
#include <array>

#include "SearchResult_Msg.h"

class TargetEngine
{
public:

	TargetEngine();

	TargetEngine& set_coordinates(double u, double v, double u_vel, double v_vel);
	TargetEngine& set_channel(std::uint8_t id);
	TargetEngine& set_variance(double variance);
	TargetEngine& set_power(std::uint16_t power);
	TargetEngine& set_frequency(double start, double width);

	void advance(std::uint64_t time);

	PlotDescription get_message() const;

private:

	const std::uint64_t delta_threshold = 1000000;

	std::uint64_t _prev_timestamp;

	double _u;
	double _v;

	double _u_vel;
	double _v_vel;

	std::uint8_t _channel_id;

	double _variance;

	std::uint16_t _power;

	double _freq_range_start;
	double _freq_range_width;
};

class SimulationEngine
{
public:

	SimulationEngine();

	SimulationEngine& set_polarization(std::uint8_t polarization);
	SimulationEngine& set_search_id(std::uint8_t id);
	SimulationEngine& set_gain(std::array<double, 8> gain);
	SimulationEngine& set_target(TargetEngine&& target);

	void advance(std::uint64_t time);

	SearchResult_Msg get_message() const;

private:

	std::uint64_t _timestamp;

	std::vector<TargetEngine> _targets;

	std::uint8_t _polarization_type;

	double _signal_amp[8];

	std::uint8_t _search_area_id;
};