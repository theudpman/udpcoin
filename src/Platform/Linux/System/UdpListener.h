// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdint>
#include <string>

namespace System {

class Dispatcher;
class Ipv4Address;

class UdpListener {
public:
  UdpListener();
  UdpListener(Dispatcher& dispatcher, const Ipv4Address& address, uint16_t port);
  UdpListener(const UdpListener&) = delete;
  UdpListener(UdpListener&& other);
  ~UdpListener();
  UdpListener& operator=(const UdpListener&) = delete;
  UdpListener& operator=(UdpListener&& other);

private:
  Dispatcher* dispatcher;
  void* context;
  int listener;
};

}
