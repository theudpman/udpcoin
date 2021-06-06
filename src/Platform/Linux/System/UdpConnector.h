#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include "TcpConnection.h"
#include "Dispatcher.h"
#include "System/UdpPacket.h"

#define NO_HOST_ERROR		-1

namespace System {

	class UdpConnector {
		public:
			UdpConnector(uint32_t ipAddress, uint16_t udpPort);
			void sendUdpPackets(const uint8_t* ptr, size_t size);

		private:
			void sendPacketMultipleAttempts(int socket, UdpPacket* packet);
			int sendUdpSequencePacket(int socket, UdpPacket* packet);
			void splitAndSendUdpPackets(int socket, const uint8_t* ptr, size_t size);
			uint32_t ipAddress;
			int udpPort;
	};

}
