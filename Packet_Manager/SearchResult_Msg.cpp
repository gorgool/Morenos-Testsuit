#include "SearchResult_Msg.h"
#include <iostream>
#include <cassert>
#include <exception>
#include <cmath>
#include <cstring>

const std::size_t PlotDescriptionRaw::msg_size = sizeof(PlotDescriptionRaw::referance_time) + 
                                                        sizeof(PlotDescriptionRaw::u) + 
                                                        sizeof(PlotDescriptionRaw::v) + 
                                                        sizeof(PlotDescriptionRaw::variance) + 
                                                        sizeof(PlotDescriptionRaw::channel_id) + 
                                                        sizeof(PlotDescriptionRaw::power) + 
                                                        sizeof(PlotDescriptionRaw::freq_range_start) + 
                                                        sizeof(PlotDescriptionRaw::freq_range_width);

// Validation function helper
template< typename ValueType , typename PredType>
static void check_value(const std::string& value_name, const ValueType& value, PredType&& pred)
{
  if (pred(value) == false)
  {
    throw std::domain_error("Validation failed. " + value_name + " out of range.\n");
  }
}

std::uint16_t serialize(const PlotDescriptionRaw & msg, unsigned char * buf)
{
  assert(buf != nullptr);

  unsigned char* msg_ptr = buf;

  memcpy(msg_ptr, &msg.referance_time[0], 5);
  msg_ptr += 5;

  memcpy(msg_ptr, &msg.u, 2);
  msg_ptr += 2;

  memcpy(msg_ptr, &msg.v, 2);
  msg_ptr += 2;

  memcpy(msg_ptr, &msg.variance, 2);
  msg_ptr += 2;

  memcpy(msg_ptr, &msg.channel_id, 1);
  msg_ptr += 1;

  memcpy(msg_ptr, &msg.power, 2);
  msg_ptr += 2;

  memcpy(msg_ptr, &msg.freq_range_start, 2);
  msg_ptr += 2;

  memcpy(msg_ptr, &msg.freq_range_width, 2);
  msg_ptr += 2;

  return PlotDescriptionRaw::msg_size;
}

void deserialize(const unsigned char * const buf, PlotDescriptionRaw & ret)
{
  assert(buf != nullptr);

  const unsigned char* msg_ptr = buf;

  memcpy(&ret.referance_time[0], msg_ptr, 5);
  msg_ptr += 5;

  memcpy(&ret.u, msg_ptr, 2);
  msg_ptr += 2;

  memcpy(&ret.v, msg_ptr, 2);
  msg_ptr += 2;

  memcpy(&ret.variance, msg_ptr, 2);
  msg_ptr += 2;

  memcpy(&ret.channel_id, msg_ptr, 1);
  msg_ptr += 1;

  memcpy(&ret.power, msg_ptr, 2);
  msg_ptr += 2;

  memcpy(&ret.freq_range_start, msg_ptr, 2);
  msg_ptr += 2;

  memcpy(&ret.freq_range_width, msg_ptr, 2);
  msg_ptr += 2;
}

PlotDescription decode(const PlotDescriptionRaw & msg)
{
  PlotDescription ret{ 0 };

  ret.channel_id = msg.channel_id;
  ret.freq_range_start = msg.freq_range_start * (115.0 / std::pow(2, 10));
  ret.freq_range_width = msg.freq_range_width * (115.0 / std::pow(2, 10));
  ret.power = msg.power;
  ret.u = msg.u * (1.0 / std::pow(2, 15));
  ret.v = msg.v * (1.0 / std::pow(2, 15));
  ret.variance = msg.variance * (1.0 / std::pow(2, 15));

  memcpy(&ret.referance_time, &msg.referance_time[0], 5);

  return ret;
}

PlotDescriptionRaw encode(const PlotDescription & msg)
{
  PlotDescriptionRaw ret{ 0 };

  ret.channel_id = msg.channel_id;
  ret.freq_range_start = static_cast<std::uint16_t>(msg.freq_range_start / (115.0 / std::pow(2, 10)));
  ret.freq_range_width = static_cast<std::uint16_t>(msg.freq_range_width / (115.0 / std::pow(2, 10)));
  ret.power = static_cast<std::uint16_t>(msg.power);
  ret.u = static_cast<std::uint16_t>(msg.u / (1.0 / std::pow(2, 15)));
  ret.v = static_cast<std::uint16_t>(msg.v / (1.0 / std::pow(2, 15)));
  ret.variance = static_cast<std::uint16_t>(msg.variance / (1.0 / std::pow(2, 15)));

  memcpy(&ret.referance_time, &msg.referance_time, 5);

  return ret;
}

bool validate(const PlotDescription & msg)
{
  try
  {
	check_value("referance time", msg.referance_time, [](auto& val) { return val <= std::pow(2, 40); });
    check_value("channel id", msg.channel_id, [](auto& val) { return val > 0 && val < 9 ; });
    check_value("start of the frequency range", msg.freq_range_start, [](auto& val) { return val >= 0.0 && val <= 115.0; });
    check_value("width of the frequency range", msg.freq_range_width, [](auto& val) { return val >= 0.0 && val < 115.0; });
    check_value("logarithm of power ", msg.power, [](auto& val) { return val >= 0 && val <= (std::pow(2, 15) - 1); });
    check_value("referance time", msg.referance_time, [](auto& val) { return val >= 0 && val <= (std::pow(2, 40) - 1); });
    check_value("u angle", msg.u, [](auto& val) { return val >= -1.0 && val <= 1.0; });
    check_value("v angle", msg.v, [](auto& val) { return val >= -1.0 && val <= 1.0; });
    check_value("variance", msg.variance, [](auto& val) { return val >= 0.0 && val <= 1.0; });
   
    return true;
  }
  catch (std::domain_error& ex)
  {
    std::cerr << "Plot description message : " << ex.what();
    return false;
  }
}

std::uint16_t serialize(const SearchResult_MsgRaw & msg, unsigned char * buf)
{
  assert(buf != nullptr);

  unsigned char* msg_ptr = buf;

  memcpy(msg_ptr, &msg.transfer_time[0], 5);
  msg_ptr += 5;

  memcpy(msg_ptr, &msg.process_time[0], 5);
  msg_ptr += 5;

  memcpy(msg_ptr, &msg.polarization_type, 1);
  msg_ptr += 1;

  memcpy(msg_ptr, &msg.signal_amp[0], 2 * 8);
  msg_ptr += 2 * 8;

  memcpy(msg_ptr, &msg.search_area_id, 1);
  msg_ptr += 1;

  memcpy(msg_ptr, &msg.plots_count, 1);
  msg_ptr += 1;

  for (std::uint8_t idx = 0; idx < msg.plots_count; ++idx)
  {
    unsigned char plot_buf[PlotDescriptionRaw::msg_size];
    serialize(msg.p[idx], plot_buf);

    memcpy(msg_ptr, plot_buf, PlotDescriptionRaw::msg_size);
    msg_ptr += PlotDescriptionRaw::msg_size;
  }

  return 5 + 5 + 1 + (2 * 8) + 1 + 1 + PlotDescriptionRaw::msg_size * msg.plots_count;
}

void deserialize(const unsigned char * const buf, SearchResult_MsgRaw & ret)
{
  assert(buf != nullptr);

  const unsigned char* msg_ptr = buf;

  memcpy(&ret.transfer_time[0], msg_ptr, 5);
  msg_ptr += 5;

  memcpy(&ret.process_time[0], msg_ptr, 5);
  msg_ptr += 5;

  memcpy(&ret.polarization_type, msg_ptr, 1);
  msg_ptr += 1;

  memcpy(&ret.signal_amp[0], msg_ptr, 2 * 8);
  msg_ptr += 2 * 8;

  memcpy(&ret.search_area_id, msg_ptr, 1);
  msg_ptr += 1;

  memcpy(&ret.plots_count, msg_ptr, 1);
  msg_ptr += 1;

  for (std::uint8_t idx = 0; idx < ret.plots_count; ++idx)
  {
    PlotDescriptionRaw raw_msg;
    deserialize(msg_ptr, raw_msg);
    msg_ptr += PlotDescriptionRaw::msg_size;

    ret.p.emplace_back(std::move(raw_msg));
  }
}

SearchResult_Msg decode(const SearchResult_MsgRaw & msg)
{
  SearchResult_Msg ret{ 0 };

  memcpy(&ret.transfer_time, &msg.transfer_time[0], 5);

  memcpy(&ret.process_time, &msg.process_time[0], 5);

  ret.polarization_type = msg.polarization_type;
  ret.plots_count = msg.plots_count;
  ret.search_area_id = msg.search_area_id;

  for (std::size_t amp_counter = 0; amp_counter < 8; ++amp_counter)
    ret.signal_amp[amp_counter] = msg.signal_amp[amp_counter] * 0.5;

  for (std::size_t plot_counter = 0; plot_counter < msg.p.size(); ++plot_counter)
    ret.p.emplace_back(decode(msg.p[plot_counter]));

  return ret;
}

SearchResult_MsgRaw encode(const SearchResult_Msg & msg)
{
  SearchResult_MsgRaw ret{ 0 };

  memcpy(&ret.transfer_time, &msg.transfer_time, 5);

  memcpy(&ret.process_time, &msg.process_time, 5);

  ret.polarization_type = msg.polarization_type;
  ret.plots_count = msg.plots_count;
  ret.search_area_id = msg.search_area_id;

  for (std::size_t amp_counter = 0; amp_counter < 8; ++amp_counter)
    ret.signal_amp[amp_counter] = static_cast<std::uint16_t>(msg.signal_amp[amp_counter] / 0.5);

  for (std::size_t plot_counter = 0; plot_counter < msg.p.size(); ++plot_counter)
    ret.p.emplace_back(encode(msg.p[plot_counter]));

  return ret;
}

bool validate(const SearchResult_Msg & msg)
{
  try
  {
	check_value("transfer time", msg.transfer_time, [](auto& val) { return val <= std::pow(2, 40); });
	check_value("process time", msg.process_time, [](auto& val) { return val <= std::pow(2, 40); });
    check_value("transfer time", msg.transfer_time, [](auto& val) { return val >= 0 && val <= (std::pow(2, 40) - 1); });
    check_value("process time", msg.process_time, [](auto& val) { return val >= 0 && val <= (std::pow(2, 40) - 1); });
    check_value("polarization type", msg.polarization_type, [](auto& val) { return val == 0 || val == 1; });
    check_value("signal gain", msg.signal_amp, [](auto& val) 
    { 
      for (auto g : val)
      {
        if (g < 0.0 || g > 120.0)
          return false;
      }
      return true; 
    });
    check_value("number of plots", msg.plots_count, [&](auto& val) { return val == msg.p.size(); });
    check_value("plots", msg.p, [](auto& val) 
    { 
      for (auto& plot : val)
      {
        if (validate(plot) == false)
          return false;
      }
      return true;
    });

    return true;
  }
  catch (std::domain_error& ex)
  {
    std::cerr << "Search result message : " << ex.what();
    return false;
  }
}