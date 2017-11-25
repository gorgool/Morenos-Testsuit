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

    memcpy(ptr, &dest[0], 6);
    ptr += 6;

    memcpy(ptr, &source[0], 6);
    ptr += 6;

    memcpy(ptr, &type, 2);
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

    memcpy(ptr, &command, 2);
    ptr += 2;

    memcpy(ptr, &frame_id, 2);
    ptr += 2;

    memcpy(ptr, &packet_id, 4);
    ptr += 4;

    memcpy(ptr, &packet_chunk_id, 4);
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

    memcpy(ptr, &code, 2);
    ptr += 2;

    memcpy(ptr, &group_id, 1);
    ptr += 1;

    memcpy(ptr, &message_id, 1);
    ptr += 1;

    memcpy(ptr, &msg_lenght, 2);
    ptr += 2;

    memcpy(ptr, &destination_id, 1);
    ptr += 1;

    memcpy(ptr, &source_id, 1);
    ptr += 1;

    memcpy(ptr, &receive_timestamp[0], 5);
    ptr += 5;

    memcpy(ptr, &transmit_timestamp[0], 5);
  }
};