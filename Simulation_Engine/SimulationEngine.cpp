#include "SimulationEngine.h"
#include <cmath>

TargetEngine::TargetEngine() :
	_prev_timestamp(0), _u(0.0), _v(0.0), _u_vel(0.0), _v_vel(0.0),
	_channel_id(0), _variance(0.0), _power(0),
	_freq_range_start(0.0), _freq_range_width(0.0) {}

TargetEngine & TargetEngine::set_coordinates(double u, double v, double u_vel, double v_vel)
{
	if (std::abs(u) > 1.0) throw std::invalid_argument("u is out of range.");
	if (std::abs(v) > 1.0) throw std::invalid_argument("v is out of range.");

	_u = u;
	_v = v;
	_u_vel = u_vel;
	_v_vel = v_vel;

	return *this;
}

TargetEngine & TargetEngine::set_channel(std::uint8_t id)
{
	if (id > 8 || id < 1) throw std::invalid_argument("channel id is out of range.");

	_channel_id = id;

	return *this;
}

TargetEngine & TargetEngine::set_variance(double variance)
{
	if (std::abs(variance) > 1.0) throw std::invalid_argument("variance is too large.");

	_variance = variance;

	return *this;
}

TargetEngine & TargetEngine::set_power(std::uint16_t power)
{
	_power = power;

	return *this;
}

TargetEngine & TargetEngine::set_frequency(double start, double width)
{
	if (start < 0.0 || start > 115.0) throw std::invalid_argument("start frequency is out of range.");
	if (width < 0.0 || width > 115.0) throw std::invalid_argument("width of the frequency range is out of range.");

	_freq_range_start = start;
	_freq_range_width = width;

	return *this;
}

void TargetEngine::advance(std::uint64_t time)
{
	auto time_delta = time - _prev_timestamp;

	// If time delta greater than threshold preserve current coordinate state
	if (time_delta < delta_threshold)
	{
		_u += _u_vel * (time_delta * 1.0e-6);
		_v += _v_vel * (time_delta * 1.0e-6);

		// If coordinates larger than 1.0 wrap it
		if (std::abs(_u) > 1.0) _u = _u - std::copysign(1.0, _u);
		if (std::abs(_v) > 1.0) _v = _v - std::copysign(1.0, _v);
	}

	_prev_timestamp = time;
}

PlotDescription TargetEngine::get_message() const
{
	PlotDescription ret{ 0 };

	ret.channel_id = _channel_id;
	ret.freq_range_start = _freq_range_start;
	ret.freq_range_width = _freq_range_width;
	ret.power = _power;
	ret.u = _u;
	ret.v = _v;
	ret.variance = _variance;
	ret.referance_time = _prev_timestamp;

	return ret;
}

SimulationEngine::SimulationEngine() : _timestamp(0), _polarization_type(0), _signal_amp{ 0 }, _search_area_id(0)
{}

SimulationEngine & SimulationEngine::set_polarization(std::uint8_t polarization)
{
	if (polarization != 0 && polarization != 1) throw std::invalid_argument("polarization type is out of range.");

	_polarization_type = polarization;

	return *this;
}

SimulationEngine & SimulationEngine::set_search_id(std::uint8_t id)
{
	_search_area_id = id;

	return *this;
}

SimulationEngine & SimulationEngine::set_gain(std::array<double, 8> gain)
{
	for (std::size_t idx = 0; idx < 8; ++idx)
	{
		if (gain[idx] < 0.0 || gain[idx] > 120.0) throw std::invalid_argument("gain is out of range.");
		_signal_amp[idx] = gain[idx];
	}

	return *this;
}

SimulationEngine & SimulationEngine::set_target(TargetEngine && target)
{
	_targets.push_back(std::forward<TargetEngine>(target));

	return *this;
}

void SimulationEngine::advance(std::uint64_t time)
{
	for (auto& t : _targets)
		t.advance(time);

	_timestamp = time;
}

SearchResult_Msg SimulationEngine::get_message() const
{
	SearchResult_Msg ret;

	ret.polarization_type = _polarization_type;
	ret.search_area_id = _search_area_id;

	for (std::size_t idx = 0; idx < 8; ++idx)
		ret.signal_amp[idx] = _signal_amp[idx];

	ret.plots_count = static_cast<std::uint8_t>(_targets.size());
	ret.process_time = _timestamp;
	ret.transfer_time = _timestamp;

	for (auto t : _targets)
		ret.p.push_back(t.get_message());

	return ret;
}
