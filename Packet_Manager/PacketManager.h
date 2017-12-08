#pragma once

#include <system_error>
#include <string>
#include "SearchResult_Msg.h"
#include "PacketHeaders.h"
#include "pcap.h"

/**
 * @class PacketSender
 * 
 * @brief Class that allows to transmit messages.
 * 
 */
class PacketSender
{
public:

	PacketSender();
	~PacketSender();

	/**
   * @fn  void PacketSender::send_packet(const SearchResult_MsgRaw& msg);
   *
   * @brief Sends given message.
   *
   * @param msg Message to be sent.
   */
    void send_packet(const SearchResult_MsgRaw& msg);

private:

	/** @brief Internal pcap handle. */
	pcap_t *handle;

	/** @brief Ethernet header. */
	EthernetHeader eth_header;

	/** @brief Custom protocol system header. */
    SystemHeader sys_header;

	/** @brief Custom protocol message header. */
    MessageHeader msg_header;

	/** @brief Buffer for message raw data. */
	unsigned char data[1500];
};

/**
 * @class PacketReceiver
 * 
 * @brief Class that allows to receive messages.
 * 
 */
class PacketReceiver
{
public:

	PacketReceiver();
	~PacketReceiver();

	/**
   * @fn  int PacketReceiver::read_packet(SearchResult_MsgRaw& msg);
   *
   * @brief Reading incloming messages. If there are no incoming messages block for 1 millisecond.
   *
   * @param msg Reference to message, where incoming message be written.
	 * 
	 * @return 0 if timeout expired; 1 on success receive.
   */
    int read_packet(SearchResult_MsgRaw& msg);

private:

	/** @brief Internal pcap handle. */
	pcap_t *handle;

	/** @brief Internal pcap receiving message description. */
	struct pcap_pkthdr* desc_header;

	/** @brief Ethernet header. */
	EthernetHeader eth_header;

	/** @brief Custom protocol system header. */
    SystemHeader sys_header;

	/** @brief Custom protocol message header. */
    MessageHeader msg_header;

	/** @brief Pointer to pcap internal message buffer. */
	const unsigned char *data;

	/** @brief Buffer for message raw data. */
    unsigned char payload[1500];
};
