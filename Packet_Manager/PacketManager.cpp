#include "PacketManager.h"
#include <iostream>
#include <cstring>

PacketSender::PacketSender() :
	eth_header{ { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0x40, 0x16, 0x7e, 0x76, 0xFF, 0x9c }}
{
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t *interfaces;

  if (pcap_findalldevs(&interfaces, errbuf) == -1)
    throw std::runtime_error("Couldn't find default device: " + std::string(errbuf));

  if (interfaces == NULL)
    throw std::runtime_error("Couldn't find default device: " + std::string(errbuf));

	handle = pcap_open_live(interfaces[0].name, BUFSIZ, 0, 0, errbuf);
	if (handle == NULL)
		throw std::runtime_error("Couldn't find default device: " + std::string(errbuf));
}

PacketSender::~PacketSender()
{
	pcap_close(handle);
}

void PacketSender::send_packet(const SearchResult_Msg& msg)
{
	auto p = encode(msg);
	auto size = serialize(p, &data[sizeof(EthernetHeader) + sizeof(SystemHeader) + sizeof(MessageHeader)]);

	eth_header.type = size + sizeof(EthernetHeader) + sizeof(SystemHeader) + sizeof(MessageHeader);
	memcpy(&data[0], &eth_header, sizeof(EthernetHeader));
	memcpy(&data[sizeof(EthernetHeader)], &sys_header, sizeof(SystemHeader));
	msg_header.msg_lenght = 10 + size;
	memcpy(&data[sizeof(EthernetHeader) + sizeof(SystemHeader)], &msg_header, sizeof(MessageHeader));

	auto err = pcap_sendpacket(handle, data, size + sizeof(EthernetHeader) + sizeof(SystemHeader) + sizeof(MessageHeader));
	if (err != 0)
		throw std::runtime_error("send message error.");
}

PacketReceiver::PacketReceiver() :
	eth_header{ { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
	{ 0x40, 0x16, 0x7e, 0x76, 0xff, 0x9c }}
{
	char errbuf[PCAP_ERRBUF_SIZE];
	bpf_u_int32 mask;
	bpf_u_int32 net;
  pcap_if_t *interfaces;

  if (pcap_findalldevs(&interfaces, errbuf) == -1)
    throw std::runtime_error("Couldn't find default device: " + std::string(errbuf));

  if (interfaces == NULL)
    throw std::runtime_error("Couldn't find default device: " + std::string(errbuf));

	if (pcap_lookupnet(interfaces->name, &net, &mask, errbuf) == -1)
		throw std::runtime_error("Couldn't find default device: " + std::string(errbuf));

    handle = pcap_open_live(interfaces->name, BUFSIZ, 1, 1, errbuf);
	if (handle == NULL)
		throw std::runtime_error("Couldn't find default device: " + std::string(errbuf));

	bpf_program fp;

	if (pcap_compile(handle, &fp, "ether[26:2]==0x5555", 0, mask) == -1)
		throw std::runtime_error("Couldn't compile filter: " + std::string(pcap_geterr(handle)));

	if (pcap_setfilter(handle, &fp) == -1)
		throw std::runtime_error("Couldn't setup filter: " + std::string(pcap_geterr(handle)));
}

PacketReceiver::~PacketReceiver()
{
	pcap_close(handle);
}

int PacketReceiver::read_packet(SearchResult_Msg & msg)
{
    auto ret = pcap_next_ex(handle, &desc_header, &data);

    if (ret == 0)
      return ret;

    if (ret == -1)
    {
        throw std::runtime_error("Error reading packet: " + std::string(pcap_geterr(handle)));
        return ret;
    }

    // Check 27.1 message
    const unsigned char* ptr = &data[sizeof(EthernetHeader) + sizeof(SystemHeader) + 2];

    if ( (*(const std::uint8_t*)ptr) != 27 )
      throw std::runtime_error("Wrong packet format: message group id not 27.");

    if ( (*(const std::uint8_t*)(ptr + 1)) != 1)
      throw std::runtime_error("Wrong packet format: message id not 1.");

		SearchResult_MsgRaw temp;
		deserialize(&data[sizeof(EthernetHeader) + sizeof(SystemHeader) + sizeof(MessageHeader)], temp);

		msg = decode(temp);

    return ret;
}
