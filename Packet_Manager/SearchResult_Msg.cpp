#include "SearchResult_Msg.h"
#include <iostream>
#include <cassert>
#include <exception>
#include <cmath>
#include <cstring>

const std::size_t PlotDescriptionRaw::msg_size = sizeof(PlotDescriptionRaw::referance_time) + 
                                                        sizeof(PlotDescriptionRaw::u) + 
                                                        sizeof(PlotDescriptionRaw::v) + 
                                                        sizeof(PlotDescriptionRaw::u_var) +
                                                        sizeof(PlotDescriptionRaw::v_var) + 
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

  memcpy(msg_ptr, &msg.referance_time[0], sizeof(msg.referance_time));
  msg_ptr += sizeof(msg.referance_time);

  memcpy(msg_ptr, &msg.u, sizeof(msg.u));
  msg_ptr += sizeof(msg.u);

  memcpy(msg_ptr, &msg.v, sizeof(msg.v));
  msg_ptr += sizeof(msg.v);

  memcpy(msg_ptr, &msg.u_var, sizeof(msg.u_var));
  msg_ptr += sizeof(msg.u_var);

  memcpy(msg_ptr, &msg.v_var, sizeof(msg.v_var));
  msg_ptr += sizeof(msg.v_var);

  memcpy(msg_ptr, &msg.channel_id, sizeof(msg.channel_id));
  msg_ptr += sizeof(msg.channel_id);

  memcpy(msg_ptr, &msg.power, sizeof(msg.power));
  msg_ptr += sizeof(msg.power);

  memcpy(msg_ptr, &msg.freq_range_start, sizeof(msg.freq_range_start));
  msg_ptr += sizeof(msg.freq_range_start);

  memcpy(msg_ptr, &msg.freq_range_width, sizeof(msg.freq_range_width));

  return PlotDescriptionRaw::msg_size;
}

void deserialize(const unsigned char * const buf, PlotDescriptionRaw & ret)
{
  assert(buf != nullptr);

  const unsigned char* msg_ptr = buf;

  memcpy(&ret.referance_time[0], msg_ptr, sizeof(ret.referance_time));
  msg_ptr += sizeof(ret.referance_time);

  memcpy(&ret.u, msg_ptr, sizeof(ret.u));
  msg_ptr += sizeof(ret.u);

  memcpy(&ret.v, msg_ptr, sizeof(ret.v));
  msg_ptr += sizeof(ret.v);

  memcpy(&ret.u_var, msg_ptr, sizeof(ret.u_var));
  msg_ptr += sizeof(ret.u_var);

  memcpy(&ret.v_var, msg_ptr, sizeof(ret.v_var));
  msg_ptr += sizeof(ret.v_var);

  memcpy(&ret.channel_id, msg_ptr, sizeof(ret.channel_id));
  msg_ptr += sizeof(ret.channel_id);

  memcpy(&ret.power, msg_ptr, sizeof(ret.power));
  msg_ptr += sizeof(ret.power);

  memcpy(&ret.freq_range_start, msg_ptr, sizeof(ret.freq_range_start));
  msg_ptr += sizeof(ret.freq_range_start);

  memcpy(&ret.freq_range_width, msg_ptr, sizeof(ret.freq_range_width));
}

PlotDescription decode(const PlotDescriptionRaw & msg)
{
  PlotDescription ret;

  ret.channel_id = msg.channel_id;
  ret.freq_range_start = msg.freq_range_start * (115.0 / std::pow(2, 10));
  ret.freq_range_width = msg.freq_range_width * (115.0 / std::pow(2, 10));
  ret.power = msg.power;
  ret.u = msg.u * (1.0 / std::pow(2, 15));
  ret.v = msg.v * (1.0 / std::pow(2, 15));
  ret.u_var = msg.u_var * (1.0 / std::pow(2, 15));
  ret.v_var = msg.v_var * (1.0 / std::pow(2, 15));

  memcpy(&ret.referance_time, &msg.referance_time[0], sizeof(msg.referance_time));

  return ret;
}

PlotDescriptionRaw encode(const PlotDescription & msg)
{
  PlotDescriptionRaw ret;

  ret.channel_id = msg.channel_id;
  ret.freq_range_start = static_cast<std::uint16_t>(msg.freq_range_start / (115.0 / std::pow(2, 10)));
  ret.freq_range_width = static_cast<std::uint16_t>(msg.freq_range_width / (115.0 / std::pow(2, 10)));
  ret.power = static_cast<std::uint16_t>(msg.power);
  ret.u = static_cast<std::uint16_t>(msg.u / (1.0 / std::pow(2, 15)));
  ret.v = static_cast<std::uint16_t>(msg.v / (1.0 / std::pow(2, 15)));
  ret.u_var = static_cast<std::uint16_t>(msg.u_var / (1.0 / std::pow(2, 15)));
  ret.v_var = static_cast<std::uint16_t>(msg.v_var / (1.0 / std::pow(2, 15)));

  memcpy(&ret.referance_time[0], &msg.referance_time, sizeof(ret.referance_time));

  return ret;
}

bool validate(const PlotDescription & msg)
{
  try
  {
	  check_value("Referance time", msg.referance_time, [](auto& val) { return val <= std::pow(2, 40); });
    check_value("Channel id", msg.channel_id, [](auto& val) { return val > 0 && val < 9 ; });
    check_value("Start of the frequency range", msg.freq_range_start, [](auto& val) { return val >= 0.0 && val <= 115.0; });
    check_value("Width of the frequency range", msg.freq_range_width, [](auto& val) { return val >= 0.0 && val < 115.0; });
    check_value("Logarithm of power ", msg.power, [](auto& val) { return val >= 0 && val <= (std::pow(2, 15) - 1); });
    check_value("Referance time", msg.referance_time, [](auto& val) { return val >= 0 && val <= (std::pow(2, 40) - 1); });
    check_value("U angle", msg.u, [](auto& val) { return val >= -1.0 && val <= 1.0; });
    check_value("V angle", msg.v, [](auto& val) { return val >= -1.0 && val <= 1.0; });
    check_value("U variance", msg.u_var, [](auto& val) { return val >= 0.0 && val <= 1.0; });
    check_value("V variance", msg.v_var, [](auto& val) { return val >= 0.0 && val <= 1.0; });
   
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

  memcpy(msg_ptr, &msg.transfer_time[0], sizeof(msg.transfer_time));
  msg_ptr += sizeof(msg.transfer_time);

  memcpy(msg_ptr, &msg.process_time[0], sizeof(msg.process_time));
  msg_ptr += sizeof(msg.process_time);

  memcpy(msg_ptr, &msg.polarization_type, sizeof(msg.polarization_type));
  msg_ptr += sizeof(msg.polarization_type);

  memcpy(msg_ptr, &msg.signal_amp[0], sizeof(msg.signal_amp));
  msg_ptr += sizeof(msg.signal_amp);

  memcpy(msg_ptr, &msg.search_area_id, sizeof(msg.search_area_id));
  msg_ptr += sizeof(msg.search_area_id);

  memcpy(msg_ptr, &msg.plots_count, sizeof(msg.plots_count));
  msg_ptr += sizeof(msg.plots_count);

  for (std::uint8_t idx = 0; idx < msg.plots_count; ++idx)
  {
    unsigned char plot_buf[PlotDescriptionRaw::msg_size];
    serialize(msg.p[idx], plot_buf);

    memcpy(msg_ptr, plot_buf, PlotDescriptionRaw::msg_size);
    msg_ptr += PlotDescriptionRaw::msg_size;
  }

  return msg_ptr - buf;
}

void deserialize(const unsigned char * const buf, SearchResult_MsgRaw & ret)
{
  assert(buf != nullptr);

  const unsigned char* msg_ptr = buf;

  memcpy(&ret.transfer_time[0], msg_ptr, sizeof(ret.transfer_time));
  msg_ptr += sizeof(ret.transfer_time);

  memcpy(&ret.process_time[0], msg_ptr, sizeof(ret.process_time));
  msg_ptr += sizeof(ret.process_time);

  memcpy(&ret.polarization_type, msg_ptr, sizeof(ret.polarization_type));
  msg_ptr += sizeof(ret.polarization_type);

  memcpy(&ret.signal_amp[0], msg_ptr, sizeof(ret.signal_amp));
  msg_ptr += sizeof(ret.signal_amp);

  memcpy(&ret.search_area_id, msg_ptr, sizeof(ret.search_area_id));
  msg_ptr += sizeof(ret.search_area_id);

  memcpy(&ret.plots_count, msg_ptr, sizeof(ret.plots_count));
  msg_ptr += sizeof(ret.plots_count);

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
  SearchResult_Msg ret;

  memcpy(&ret.transfer_time, &msg.transfer_time[0], sizeof(msg.transfer_time));

  memcpy(&ret.process_time, &msg.process_time[0], sizeof(msg.process_time));

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
  SearchResult_MsgRaw ret;

  memcpy(&ret.transfer_time[0], &msg.transfer_time, sizeof(ret.transfer_time));

  memcpy(&ret.process_time[0], &msg.process_time, sizeof(ret.process_time));

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
	  check_value("Transfer time", msg.transfer_time, [](auto& val) { return val <= std::pow(2, 40); });
	  check_value("Process time", msg.process_time, [](auto& val) { return val <= std::pow(2, 40); });
    check_value("Polarization type", msg.polarization_type, [](auto& val) { return val == 0 || val == 1; });
    check_value("Signal gain", msg.signal_amp, [](auto& val) 
    { 
      for (auto g : val)
      {
        if (g < 0.0 || g > 120.0)
          return false;
      }
      return true; 
    });
    check_value("Number of plots", msg.plots_count, [&](auto& val) { return val == msg.p.size(); });
    check_value("Plots", msg.p, [](auto& val) 
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
