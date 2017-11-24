#pragma once

#include <cinttypes>
#include <vector>

// Single plot description
struct PlotDescriptionRaw
{
  // Referance time of the following parameters 
  std::uint8_t referance_time[5];

  // Azimuth angle in cosine space
  std::int16_t u;

  // Elevation angle in cosine space
  std::int16_t v;

  // Varianve of u and v
  std::uint16_t variance;

  // Id of the central channel assosiated with the current plot
  std::uint8_t channel_id;

  // The logarithm of the reflected power from the current plot
  std::uint16_t power;

  // Begin of the frequency range   
  std::uint16_t freq_range_start;

  // The width of the frequency range, starting from freq_range_start
  std::uint16_t freq_range_width;

  const static std::size_t msg_size;
};

// Single plot description (decoded)
struct PlotDescription
{
  // Referance time of the following parameters, microseconds
  std::uint64_t referance_time;

  // Azimuth angle in cosine space, dimensionless
  double u;

  // Elevation angle in cosine space, dimensionless
  double v;

  // Varianve of u and v, dimensionless
  double variance;

  // Id of the central channel assosiated with the current plot, dimensionless
  std::uint8_t channel_id;

  // The logarithm of the reflected power from the current plot, dimensionless
  double power;

  // Begin of the frequency range, MHz
  double freq_range_start;

  // The width of the frequency range, starting from freq_range_start, MHz
  double freq_range_width;
};

std::uint16_t serialize(const PlotDescriptionRaw& msg, unsigned char* buf);
void deserialize(const unsigned char* const buf, PlotDescriptionRaw& ret);
PlotDescription decode(const PlotDescriptionRaw& msg);
PlotDescriptionRaw encode(const PlotDescription& msg);
bool validate(const PlotDescription& msg);

// Message 27.1 (28.1)
struct SearchResult_MsgRaw
{
  // Device timestamp
  std::uint8_t transfer_time[5];

  // Last signal processing timestamp
  std::uint8_t process_time[5];

  // Type of polarization: 0 or 1
  std::uint8_t polarization_type;

  // Central channels gain
  std::uint16_t signal_amp[8];

  // Id of the search area
  std::uint8_t search_area_id;

  // Number of discovered objects (plot) 
  std::uint8_t plots_count;

  // Discovered plots
  std::vector<PlotDescriptionRaw> p;

  std::size_t size() const
  {
    return sizeof(transfer_time) + sizeof(process_time) + sizeof(polarization_type) + sizeof(signal_amp)
      + sizeof(search_area_id) + sizeof(plots_count) + p.size() * PlotDescriptionRaw::msg_size;
  }
};

struct SearchResult_Msg
{
  // Device timestamp
  std::uint64_t transfer_time;

  // Last signal processing timestamp
  std::uint64_t process_time;

  // Type of polarization: 0 or 1
  std::uint8_t polarization_type;

  // Central channels gain
  double signal_amp[8];

  // Id of the search area
  std::uint8_t search_area_id;

  // Number of discovered objects (plot) 
  std::uint8_t plots_count;

  // Discovered plots
  std::vector<PlotDescription> p;
};

std::uint16_t serialize(const SearchResult_MsgRaw& msg, unsigned char* buf);
void deserialize(const unsigned char* const buf, SearchResult_MsgRaw& ret);
SearchResult_Msg decode(const SearchResult_MsgRaw& msg);
SearchResult_MsgRaw encode(const SearchResult_Msg& msg);
bool validate(const SearchResult_Msg& msg);