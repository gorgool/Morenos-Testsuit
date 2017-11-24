#pragma once

#include <system_error>
#include <string>
#include "SearchResult_Msg.h"
#include "PacketHeaders.h"
#include "pcap.h"


class PacketSender
{
public:

	PacketSender();
	~PacketSender();
	void send_packet(const SearchResult_Msg& msg);

private:

	pcap_t *handle;

	// Link-layer headers parameters
	EthernetHeader eth_header;
  SystemHeader sys_header;
  MessageHeader msg_header;

	// Data and payload buffer
	unsigned char data[1500];
  unsigned char payload[1500];
};

class PacketReceiver
{
public:

	PacketReceiver();
	~PacketReceiver();
	int wait_packet(SearchResult_Msg& msg);

private:

	pcap_t *handle;

	// Geader for packet receiving
	struct pcap_pkthdr* desc_header;

	// Link-layer headers parameters
  EthernetHeader eth_header;
  SystemHeader sys_header;
  MessageHeader msg_header;

	// Data and payload buffer
	const unsigned char *data;
  unsigned char payload[1500];
};