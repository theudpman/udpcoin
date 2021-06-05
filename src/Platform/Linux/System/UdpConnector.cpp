// Copyright (c) 2021 The UDPCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "UdpConnector.h"
#include <cassert>
#include <stdexcept>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include <System/InterruptedException.h>
#include <System/Ipv4Address.h>
#include "Dispatcher.h"
#include "ErrorMessage.h"
#include "TcpConnection.h"

inline uint32_t hostToNetwork(uint32_t n) {
	return (n << 24) | (n & 0xff00) << 8 | (n & 0xff0000) >> 8 | (n >> 24);
}


namespace System {


	UdpConnector::UdpConnector(uint32_t ipAddressParam, int udpPortParam) {
		ipAddress = ipAddressParam;
		udpPort = udpPortParam;
	}

	int UdpConnector::sendUdpPacket(const uint8_t* ptr, size_t size) {
		sockaddr_in servaddr;
		int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (fd < 0) {
			// log here
			return -1;
		}

		bzero((char*)&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(ipAddress);
		servaddr.sin_port = htons(udpPort);
		int udpSendResult = sendto(fd, ptr, size, 0, (sockaddr*)&servaddr, sizeof(servaddr));

		if (udpSendResult < 0){
			// log here
			close(fd);
			return -1;
		}

		close(fd);
		return size;
	}


}
