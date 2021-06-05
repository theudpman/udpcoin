// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdint>
#include <string>

#include "System/UdpPacket.h"

#define		MAX_SAFE_UDP_DATA_SIZE			508
#define		IP_HEADER_SIZE					60
#define		UDP_HEADER_SIZE					8

#define 	MAX_SAFE_UDP_PACKET_SIZE		(MAX_SAFE_UDP_DATA_SIZE + IP_HEADER_SIZE + UDP_HEADER_SIZE)

namespace System {

class Dispatcher;
class Ipv4Address;
class UdpPacket;

class UdpListener {
public:
  UdpListener();
  UdpListener(Dispatcher& dispatcher, const Ipv4Address& address, uint16_t port);
  UdpListener(const UdpListener&) = delete;
  UdpListener(UdpListener&& other);
  ~UdpListener();
  UdpListener& operator=(const UdpListener&) = delete;
  UdpListener& operator=(UdpListener&& other);
  UdpPacket receiveUdpPacket();
  void closeSocket();

private:
  Dispatcher* dispatcher;
  void* context;
  int listener;
};

}
