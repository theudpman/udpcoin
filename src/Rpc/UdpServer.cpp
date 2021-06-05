// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "UdpServer.h"
#include <boost/scope_exit.hpp>

#include <System/InterruptedException.h>
#include <System/Ipv4Address.h>
#include "../System/UdpPacket.h"

using namespace Logging;

namespace CryptoNote {

UdpServer::UdpServer(System::Dispatcher& dispatcher, Logging::ILogger& log)
  : m_dispatcher(dispatcher), workingContextGroup(dispatcher), logger(log, "HttpServer") {

}

void UdpServer::start(const std::string& address, uint16_t port) {
  m_listener = System::UdpListener(m_dispatcher, System::Ipv4Address(address), port);
  workingContextGroup.spawn(std::bind(&UdpServer::listenForUdpTransactions, this));
}

void UdpServer::stop() {
  workingContextGroup.interrupt();
  workingContextGroup.wait();
}

void UdpServer::listenForUdpTransactions() {
  try {
    System::UdpPacket udpPacket;
    bool packtRead = false;

    while (!packtRead) {
      try {
    	udpPacket = m_listener.receiveUdpPacket();
    	packtRead = true;
      } catch (System::InterruptedException&) {
        throw;
      } catch (std::exception&) {
        // try again
      }
    }

    logger(DEBUGGING) << "Incoming UDP packet: " << udpPacket.hexString();

    workingContextGroup.spawn(std::bind(&UdpServer::listenForUdpTransactions, this));



    logger(DEBUGGING) << "Processed UDP packet: " << udpPacket.hexString();

  } catch (System::InterruptedException&) {
  } catch (std::exception& e) {
    logger(WARNING) << "Connection error: " << e.what();
  }
}

}
