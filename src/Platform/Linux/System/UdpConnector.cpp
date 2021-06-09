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
#include <string.h>

#include <System/InterruptedException.h>
#include <System/Ipv4Address.h>
#include "Dispatcher.h"
#include "ErrorMessage.h"
#include "TcpConnection.h"




#define			MAX_UDP_PACKETS_TO_SEND						3


inline uint32_t hostToNetwork(uint32_t n) {
	return (n << 24) | (n & 0xff00) << 8 | (n & 0xff0000) >> 8 | (n >> 24);
}


namespace System {


	UdpConnector::UdpConnector(uint32_t ipAddressParam, uint16_t udpPortParam) {
		ipAddress = ipAddressParam;
		udpPort = udpPortParam;
	}

	void UdpConnector::sendUdpPackets(const uint8_t* ptr, size_t size) {
		int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (fd < 0) {
			throw std::runtime_error("Failed to open socket for sending UDP packets");
		}

		if (size < MAX_SAFE_UDP_DATA_SIZE) {
			UdpPacket packet(ptr, size, 1, 1);
			sendPacketMultipleAttempts(fd, &packet);
			int closeSocketRetCode = close(fd);

			if (closeSocketRetCode < 0) {
				throw std::runtime_error("Failed to close socket for sending UDP packets");
			}

			return;
		}

		splitAndSendUdpPackets(fd, ptr, size);
		int closeSocketRetCode = close(fd);
		if (closeSocketRetCode < 0) {
			throw std::runtime_error("Failed to close socket for sending UDP packets");
		}

		return;
	}

	void UdpConnector::splitAndSendUdpPackets(int socket, const uint8_t* ptr, size_t size) {
		uint8_t totalPackets = (uint8_t) (size / (MAX_SAFE_UDP_DATA_SIZE + UDP_SEQUENCE_HEADER));
		uint8_t packetNum;
		size_t totalBytes = 0;

		while (totalBytes < size) {
			size_t dataLen = MAX_SAFE_UDP_DATA_SIZE;

			if (size - totalBytes < MAX_SAFE_UDP_DATA_SIZE) {
				dataLen = size - totalBytes;
			}

			uint8_t packetData[dataLen];
			memcpy(&packetData[0], ptr, dataLen);
			UdpPacket udpPacket(&packetData[0], dataLen, totalPackets, packetNum);
			sendPacketMultipleAttempts(socket, &udpPacket);

			totalBytes += MAX_SAFE_UDP_DATA_SIZE;
			packetNum += 1;
			ptr += MAX_SAFE_UDP_DATA_SIZE;
		}
	}

	void UdpConnector::sendPacketMultipleAttempts(int socket, UdpPacket* packet) {
		for (int i = 0; i < MAX_UDP_PACKETS_TO_SEND; ++i) {
			if (sendUdpSequencePacket(socket, packet) < 0) {
				throw std::runtime_error("Failed to send UDP packet " + (i + 1));
			}
		}
	}

	int UdpConnector::sendUdpSequencePacket(int socket, UdpPacket* packet) {
		sockaddr_in servaddr;
		bzero((char*)&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(ipAddress);
		servaddr.sin_port = htons(udpPort);
		return sendto(socket, packet->getData(), packet->getDataLength(), 0, (sockaddr*)&servaddr, sizeof(servaddr));
	}


}
