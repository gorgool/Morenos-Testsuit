#pragma once

#include <cinttypes>
#include <cassert>
#include <cstring>

struct EthernetHeader
{
  std::uint8_t dest[6];
  std::uint8_t source[6];
  std::uint16_t type = 0;

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

struct SystemHeader
{
  std::uint16_t command = 0;
  std::uint16_t frame_id = 0;
  std::uint32_t packet_id = 0;
  std::uint32_t packet_chunk_id = 0;

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

struct MessageHeader
{
  std::uint16_t code = 0x5555;
  std::uint8_t group_id = 27;
  std::uint8_t message_id = 1;
  std::uint16_t msg_lenght;
  std::uint8_t destination_id = 0;
  std::uint8_t source_id = 0;

  char receive_timestamp[5] = { 0 };
  char transmit_timestamp[5] = { 0 };

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