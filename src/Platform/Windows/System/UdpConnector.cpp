// Copyright (c) 2021 The UDPCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "UdpConnector.h"
#include <cassert>
#include <stdexcept>

#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <System/InterruptedException.h>
#include <System/Ipv4Address.h>
#include "Dispatcher.h"
#include "ErrorMessage.h"
#include "TcpConnection.h"




#define			MAX_UDP_PACKETS_TO_SEND						1


inline uint32_t hostToNetwork(uint32_t n) {
	return (n << 24) | (n & 0xff00) << 8 | (n & 0xff0000) >> 8 | (n >> 24);
}


namespace System {


	UdpConnector::UdpConnector(uint32_t ipAddressParam, uint16_t udpPortParam) {
		ipAddress = ipAddressParam;
		udpPort = udpPortParam;
	}

	bool UdpConnector::isValid() {
		return false;
	}

	void UdpConnector::sendUdpPackets(const uint8_t* ptr, size_t size) {
		// needs to be implemented

		return;
	}

	void UdpConnector::splitAndSendUdpPackets(int socket, const uint8_t* ptr, size_t size) {
		// needs to be implemented
	}

	void UdpConnector::sendPacketMultipleAttempts(int socket, UdpPacket* packet) {
		// needs to be implemented
	}

	int UdpConnector::sendUdpSequencePacket(int socket, UdpPacket* packet) {
		// needs to be implemented
		return -1;
	}

}
