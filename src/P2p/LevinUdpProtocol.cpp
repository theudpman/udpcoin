// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "LevinUdpProtocol.h"
#include <System/TcpConnection.h>
#include <System/UdpConnector.h>
#include <System/Ipv4Address.h>
#include "LevinConstants.h"

namespace System {
	class UdpPacket;
	class TcpConnection;
}

using namespace CryptoNote;

LevinUdpProtocol::LevinUdpProtocol(System::TcpConnection& connection)
  : m_conn(connection) {
}

bool LevinUdpProtocol::parseUdpPacket(System::UdpPacket* udpPacket) {

}

void LevinUdpProtocol::sendUdpMessage(uint32_t command, const BinaryArray& out, std::string udpPort) {
	bucket_head2 head = { 0 };
	head.m_signature = LEVIN_SIGNATURE;
	head.m_cb = out.size();
	head.m_have_to_return_data = false;
	head.m_command = command;
	head.m_protocol_version = LEVIN_PROTOCOL_VER_1;
	head.m_flags = LEVIN_PACKET_REQUEST;

	// write header and body in one operation
	BinaryArray writeBuffer;
	writeBuffer.reserve(sizeof(head) + out.size());

	Common::VectorOutputStream stream(writeBuffer);
	stream.writeSome(&head, sizeof(head));
	stream.writeSome(out.data(), out.size());

	auto ipAddressAndPort = m_conn.getPeerAddressAndPort();
	System::Ipv4Address ipAddress = ipAddressAndPort.first;
	uint16_t udpPortInt = std::stoi(udpPort);
	System::UdpConnector udp_conn(ipAddress.getValue(), udpPortInt);
	udp_conn.sendUdpPackets(writeBuffer.data(), writeBuffer.size());
}

