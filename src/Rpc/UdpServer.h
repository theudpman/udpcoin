// Copyright (c) 2021 UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <unordered_set>

#include <HTTP/HttpRequest.h>
#include <HTTP/HttpResponse.h>

#include <System/ContextGroup.h>
#include <System/Dispatcher.h>
#include <System/UdpListener.h>
#include <System/Event.h>

#include <Logging/LoggerRef.h>

namespace CryptoNote {

class UdpServer {

public:

  UdpServer(System::Dispatcher& dispatcher, Logging::ILogger& log);

  void start(const std::string& address, uint16_t port);
  void stop();

protected:

  System::Dispatcher& m_dispatcher;

private:

  void listenForUdpTransactions();

  System::ContextGroup workingContextGroup;
  Logging::LoggerRef logger;
  System::UdpListener m_listener;
};

}
