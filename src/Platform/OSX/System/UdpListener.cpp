// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "UdpListener.h"
#include <cassert>
#include <stdexcept>

#include <iostream>
#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "Dispatcher.h"
#include <System/ErrorMessage.h>
#include <System/InterruptedException.h>
#include <System/Ipv4Address.h>
#include "System/UdpPacket.h"

#include "Logging/ConsoleLogger.h"
#include <Logging/LoggerManager.h>

namespace System {

UdpListener::UdpListener() : dispatcher(nullptr) {
}

UdpListener::UdpListener(Dispatcher& dispatcher, const Ipv4Address& addr, uint16_t port) : dispatcher(&dispatcher) {
  std::string message;
  listener = -1;
 // needs implementation
}

UdpListener::UdpListener(UdpListener&& other) : dispatcher(other.dispatcher) {
  if (other.dispatcher != nullptr) {
    assert(other.context == nullptr);
    listener = other.listener;
    context = nullptr;
    other.dispatcher = nullptr;
  }
}

UdpListener::~UdpListener() {
  if (dispatcher != nullptr) {
    assert(context == nullptr);
    int result = close(listener);
    assert(result != -1);
  }
}

UdpListener& UdpListener::operator=(UdpListener&& other) {
  if (dispatcher != nullptr) {
    assert(context == nullptr);
    if (close(listener) == -1) {
      throw std::runtime_error("UdpListener::operator=, close failed, " + lastErrorMessage());
    }
  }

  dispatcher = other.dispatcher;
  if (other.dispatcher != nullptr) {
    assert(other.context == nullptr);
    listener = other.listener;
    context = nullptr;
    other.dispatcher = nullptr;
  }

  return *this;
}

void UdpListener::closeSocket() {

}

bool UdpListener::isValid() {
	return false;
}

UdpPacket* UdpListener::receiveUdpPacket() {
	// needs implementation
    return nullptr;
 }
//
//  throw std::runtime_error("UdpListener::receiveUdpPacket, " + message);
}

}
