#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include "TcpConnection.h"
#include "Dispatcher.h"

#define NO_HOST_ERROR		-1

namespace System {

	class UdpConnector {
		public:
			UdpConnector(uint32_t ipAddress);
			int sendUdpPacket(const uint8_t* ptr, size_t size);

		private:
			uint32_t ipAddress;
	};

}
