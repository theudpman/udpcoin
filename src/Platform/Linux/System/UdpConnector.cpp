// Copyright (c) 2021 The UDPCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "UdpConnector.h"
#include <cassert>
#include <stdexcept>

#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>

#include <System/InterruptedException.h>
#include <System/Ipv4Address.h>
#include "Dispatcher.h"
#include "ErrorMessage.h"
#include "TcpConnection.h"

inline uint32_t hostToNetwork(uint32_t n) {
	return (n << 24) | (n & 0xff00) << 8 | (n & 0xff0000) >> 8 | (n >> 24);
}


namespace System {


	UdpConnector::UdpConnector(uint32_t ipAddressParam) {
		ipAddress = ipAddressParam;
	}

	int UdpConnector::sendUdpPacket(const uint8_t* ptr, size_t size) {



	}


}
