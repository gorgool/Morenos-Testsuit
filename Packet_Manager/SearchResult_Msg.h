#pragma once

#include <cinttypes>
#include <vector>

/**
 * @struct PlotDescriptionRaw
 * 
 * @brief Single plot (target) description
 * 
 */
struct PlotDescriptionRaw
{
  /** @brief Referance time of the following parameters. */
  std::uint8_t referance_time[5] = {0, 0, 0, 0, 0};

  /** @brief Azimuth angle in cosine space. */
  std::int32_t u = 0;

  /** @brief Elevation angle in cosine space. */
  std::int32_t v = 0;

  /** @brief Varianve of u. */
  std::uint32_t u_var = 0;

  /** @brief Varianve of v. */
  std::uint32_t v_var = 0;

  /** @brief Id of the central channel assosiated with the current plot. */
  std::uint8_t channel_id = 0;

  /** @brief The logarithm of the reflected power from the current plot. */
  std::uint32_t power = 0;

  /** @brief Begin of the frequency range. */
  std::uint32_t freq_range_start = 0;

  /** @brief The width of the frequency range, starting from freq_range_start. */
  std::uint32_t freq_range_width = 0;

  /** @brief Size of the message. */
  const static std::size_t msg_size;
};

/**
 * @struct PlotDescription
 * 
 * @brief Single plot (target) description (decoded)
 * 
 */
struct PlotDescription
{
  /** @brief Referance time of the following parameters, microseconds. */
  std::uint64_t referance_time = 0;

  /** @brief Azimuth angle in cosine space, dimensionless. */
  double u = 0.0;

  /** @brief Elevation angle in cosine space, dimensionless. */
  double v = 0.0;

  /** @brief Varianve of u, dimensionless. */
  double u_var = 0.0;

  /** @brief Varianve of v, dimensionless. */
  double v_var = 0.0;

  /** @brief Id of the central channel assosiated with the current plot, dimensionless. */
  std::uint8_t channel_id = 0;

  /** @brief The logarithm of the reflected power from the current plot, dimensionless. */
  double power = 0.0;

  /** @brief Begin of the frequency range, MHz. */
  double freq_range_start = 0.0;

  /** @brief The width of the frequency range, starting from freq_range_start, MHz. */
  double freq_range_width = 0.0;
};

/**
   * @fn  std::uint16_t serialize(const PlotDescriptionRaw& msg, unsigned char* buf);
   *
   * @brief Serialize given message in buffer.
   *
   * @param msg Reference to message.
   * @param msg Pointer to buffer to serialize into. Terminate on null.
	 * 
	 * @return Size of the serialized message in bytes.
   */
std::uint16_t serialize(const PlotDescriptionRaw& msg, unsigned char* buf);

/**
   * @fn  std::uint16_t serialize_to_network(const PlotDescriptionRaw& msg, unsigned char* buf);
   *
   * @brief Serialize given message in buffer. Convert all integer fields into network byte order (Big endian).
   *
   * @param msg Reference to message.
   * @param msg Pointer to buffer to serialize into. Terminate on null.
     *
     * @return Size of the serialized message in bytes.
   */
std::uint16_t serialize_to_network(const PlotDescriptionRaw& msg, unsigned char* buf);


/**
   * @fn  void deserialize(const unsigned char* const buf, PlotDescriptionRaw& ret);
   *
   * @brief Deserialize given buffer in the message struct.
   *
   * @param msg Reference to message, where the result to be write in.
   * @param buf Pointer to buffer with data.  Terminate on null.
	 * 
   */
void deserialize(const unsigned char* const buf, PlotDescriptionRaw& ret);

/**
   * @fn  void deserialize(const unsigned char* const buf, PlotDescriptionRaw& ret);
   *
   * @brief Deserialize given buffer in the message struct. Convert all interger fields from network byte
   *        order (big endian) to native byte order.
   *
   * @param msg Reference to message, where the result to be write in.
   * @param buf Pointer to buffer with data.  Terminate on null.
     *
   */
void deserialize_from_network(const unsigned char* const buf, PlotDescriptionRaw& ret);

/**
   * @fn  PlotDescription decode(const PlotDescriptionRaw& msg);
   *
   * @brief Change representaion of the raw message into usable in calculations format.
   *
   * @param msg Reference to the raw message.
	 * 
	 * @return Decoded message.
   */
PlotDescription decode(const PlotDescriptionRaw& msg);

/**
   * @fn  PlotDescriptionRaw encode(const PlotDescription& msg);
   *
   * @brief Encode message into the format that used in hardware.
   *
   * @param msg Reference to the message.
	 * 
	 * @return Encoded raw message.
   */
PlotDescriptionRaw encode(const PlotDescription& msg);

/**
   * @fn  bool validate(const PlotDescription& msg);
   *
   * @brief Checks given message for complience with the range and allowed values. 
   *
   * @param msg Reference to the message.
	 * 
	 * @return True if message is passes all checks; False otherwise.
   */
bool validate(const PlotDescription& msg);

/**
 * @struct SearchResult_MsgRaw
 * 
 * @brief Message 27.1 (28.1)
 * 
 */
struct SearchResult_MsgRaw
{
  /** @brief Device timestamp. */
  std::uint8_t transfer_time[5] = {0, 0, 0, 0, 0};

  /** @brief Last signal processing timestamp. */
  std::uint8_t process_time[5] = {0, 0, 0, 0, 0};

  /** @brief Type of polarization. */
  std::uint8_t polarization_type = 0;

  /** @brief Central channels gain. */
  std::uint16_t signal_amp[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  /** @brief Id of the search area. */
  std::uint8_t search_area_id = 0;

  /** @brief Number of discovered objects (plot). */
  std::uint8_t plots_count = 0;

  /** @brief Discovered plots. */
  std::vector<PlotDescriptionRaw> p;

  std::size_t size() const
  {
    return sizeof(transfer_time) + sizeof(process_time) + sizeof(polarization_type) + sizeof(signal_amp)
      + sizeof(search_area_id) + sizeof(plots_count) + p.size() * PlotDescriptionRaw::msg_size;
  }
};

/**
 * @struct SearchResult_MsgRaw
 * 
 * @brief Message 27.1 (28.1) decoded
 * 
 */
struct SearchResult_Msg
{
  /** Device timestamp, microseconds. */
  std::uint64_t transfer_time = 0;

  /** Last signal processing timestamp, microseconds. */
  std::uint64_t process_time = 0;

  /** Type of polarization. */
  std::uint8_t polarization_type = 0;

  /** Central channels gain. */
  double signal_amp[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  /** Id of the search area. */
  std::uint8_t search_area_id = 0;

  /** Number of discovered objects (plot). */
  std::uint8_t plots_count = 0;

  /** Discovered plots. */
  std::vector<PlotDescription> p;
};

/**
   * @fn  std::uint16_t serialize(const SearchResult_MsgRaw& msg, unsigned char* buf);
   *
   * @brief Serialize given message in buffer.
   *
   * @param msg Reference to message.
   * @param msg Pointer to buffer to serialize into. Terminate on null.
	 * 
	 * @return Size of the serialized message in bytes.
   */
std::uint16_t serialize(const SearchResult_MsgRaw& msg, unsigned char* buf);

/**
   * @fn  std::uint16_t serialize(const SearchResult_MsgRaw& msg, unsigned char* buf);
   *
   * @brief Serialize given message in buffer. Convert all integer fields into network byte order (Big endian).
   *
   * @param msg Reference to message.
   * @param msg Pointer to buffer to serialize into. Terminate on null.
     *
     * @return Size of the serialized message in bytes.
   */
std::uint16_t serialize_to_network(const SearchResult_MsgRaw& msg, unsigned char* buf);

/**
   * @fn  void deserialize(const unsigned char* const buf, SearchResult_MsgRaw& ret);
   *
   * @brief Deserialize given buffer in the message struct.
   *
   * @param msg Reference to message, where the result to be write in.
   * @param buf Pointer to buffer with data.  Terminate on null.
	 * 
   */
void deserialize(const unsigned char* const buf, SearchResult_MsgRaw& ret);

/**
   * @fn  void deserialize(const unsigned char* const buf, SearchResult_MsgRaw& ret);
   *
   * @brief Deserialize given buffer in the message struct. Convert all interger fields from network byte
   *        order (big endian) to native byte order.
   *
   * @param msg Reference to message, where the result to be write in.
   * @param buf Pointer to buffer with data.  Terminate on null.
     *
   */
void deserialize_from_network(const unsigned char* const buf, SearchResult_MsgRaw& ret);

/**
   * @fn  SearchResult_Msg decode(const SearchResult_MsgRaw& msg);
   *
   * @brief Change representaion of the raw message into usable in calculations format.
   *
   * @param msg Reference to the raw message.
	 * 
	 * @return Decoded message.
   */
SearchResult_Msg decode(const SearchResult_MsgRaw& msg);

/**
   * @fn  SearchResult_MsgRaw encode(const SearchResult_Msg& msg);
   *
   * @brief Encode message into the format that used in hardware.
   *
   * @param msg Reference to the message.
	 * 
	 * @return Encoded raw message.
   */
SearchResult_MsgRaw encode(const SearchResult_Msg& msg);

/**
   * @fn  bool validate(const SearchResult_Msg& msg);
   *
   * @brief Checks given message for complience with the range and allowed values. 
   *
   * @param msg Reference to the message.
	 * 
	 * @return True if message is passes all checks; False otherwise.
   */
bool validate(const SearchResult_Msg& msg);
