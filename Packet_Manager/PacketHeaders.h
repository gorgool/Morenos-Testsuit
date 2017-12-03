#pragma once

#include <cinttypes>
#include <cassert>
#include <cstring>

/**
 * @struct EthernetHeader
 * 
 * @brief Ethernet standard protocol fields
 * 
 */
struct EthernetHeader
{
  /** @brief MAC address of destination machine. */
  std::uint8_t dest[6];

  /** @brief MAC address of source machine. */
  std::uint8_t source[6];

  /** @brief Type of the protocol. */
  std::uint16_t type = 0;

 /**
   * @fn  void EthernetHeader::serialize(unsigned char* buf);
   *
   * @brief Serialize ethernet protocol header in the given buffer.
   *
   * @param buf Pointer to the buffer to serialize into. Terminate on null.
   */
  void serialize(unsigned char* buf)
  {
    assert(buf != nullptr);

    unsigned char* ptr = buf;

    memcpy(ptr, &dest[0], sizeof(dest));
    ptr += sizeof(dest);

    memcpy(ptr, &source[0], sizeof(source));
    ptr += sizeof(source);

    memcpy(ptr, &type, sizeof(type));
  }
};

/**
 * @struct SystemHeader
 * 
 * @brief Custom protocol system fileds.
 * 
 */
struct SystemHeader
{
  /** @brief Custom protocol command.*/
  std::uint16_t command = 0;

  /** @brief Custom protocol id of the frame. */
  std::uint16_t frame_id = 0;

  /** @brief Custom protocol id of the packet. */
  std::uint32_t packet_id = 0;

  /** @brief Custom protocol id of part of the packet. */
  std::uint32_t packet_chunk_id = 0;

/**
   * @fn  void SystemHeader::serialize(unsigned char* buf);
   *
   * @brief Serialize custom protocol system header in the given buffer.
   *
   * @param buf Pointer to the buffer to serialize into. Terminate on null.
   */
  void serialize(unsigned char* buf)
  {
    assert(buf != nullptr);

    unsigned char* ptr = buf;

    memcpy(ptr, &command, sizeof(command));
    ptr += sizeof(command);

    memcpy(ptr, &frame_id, sizeof(frame_id));
    ptr += sizeof(frame_id);

    memcpy(ptr, &packet_id, sizeof(packet_id));
    ptr += sizeof(packet_id);

    memcpy(ptr, &packet_chunk_id, sizeof(packet_chunk_id));
  }
};

/**
 * @struct MessageHeader
 * 
 * @brief Custom protocol message fields.
 * 
 */
struct MessageHeader
{
  /** @brief Custom protocol special code. Must be 0x5555. */
  std::uint16_t code = 0x5555;

  /** @brief Custom protocol id of the message group. */
  std::uint8_t group_id = 27;

  /** @brief Custom protocol id of the message in aforementioned group. */
  std::uint8_t message_id = 1;

  /** @brief Custom protocol message lenght. */
  std::uint16_t msg_lenght;

  /** @brief Custom protocol id of the destination. */
  std::uint8_t destination_id = 0;

  /** @brief Custom protocol id of the source. */
  std::uint8_t source_id = 0;

  /** @brief Timestamp of receive. */
  char receive_timestamp[5] = { 0 };

  /** @brief Timestamp of transmit. */
  char transmit_timestamp[5] = { 0 };

/**
   * @fn  void MessageHeader::serialize(unsigned char* buf);
   *
   * @brief Serialize Custom protocol message header in the given buffer.
   *
   * @param buf Pointer to the buffer to serialize into. Terminate on null.
   */
  void serialize(unsigned char* buf)
  {
    assert(buf != nullptr);

    unsigned char* ptr = buf;

    memcpy(ptr, &code, sizeof(code));
    ptr += sizeof(code);

    memcpy(ptr, &group_id, sizeof(group_id));
    ptr += sizeof(group_id);

    memcpy(ptr, &message_id, sizeof(message_id));
    ptr += sizeof(message_id);

    memcpy(ptr, &msg_lenght, sizeof(msg_lenght));
    ptr += sizeof(msg_lenght);

    memcpy(ptr, &destination_id, sizeof(destination_id));
    ptr += sizeof(destination_id);

    memcpy(ptr, &source_id, sizeof(source_id));
    ptr += sizeof(source_id);

    memcpy(ptr, &receive_timestamp[0], sizeof(receive_timestamp));
    ptr += sizeof(receive_timestamp);

    memcpy(ptr, &transmit_timestamp[0], sizeof(transmit_timestamp));
  }
};