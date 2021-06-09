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
  listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (listener == -1) {
    message = "socket failed, " + lastErrorMessage();
  } else {
	  int on = 1;

	  if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) == -1) {
		message = "setsockopt failed, " + lastErrorMessage();
	  } else {
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		address.sin_addr.s_addr = htonl( addr.getValue());
		if (bind(listener, reinterpret_cast<sockaddr *>(&address), sizeof address) != 0) {
		  message = "bind failed, " + lastErrorMessage();
		} else {
		  epoll_event listenEvent;
		  listenEvent.events = 0;
		  listenEvent.data.ptr = nullptr;

		  if (epoll_ctl(dispatcher.getEpoll(), EPOLL_CTL_ADD, listener, &listenEvent) == -1) {
			message = "epoll_ctl failed, " + lastErrorMessage();
		  } else {
			context = nullptr;
			return;
		  }

	  }
    }

    int result = close(listener);
    assert(result != -1);
  }

  throw std::runtime_error("UdpListener::UdpListener, " + message);
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

UdpPacket* UdpListener::receiveUdpPacket() {
  assert(dispatcher != nullptr);
  assert(context == nullptr);
  if (dispatcher->interrupted()) {
    throw InterruptedException();
  }


  ContextPair contextPair;
  OperationContext listenerContext;
  listenerContext.interrupted = false;
  listenerContext.context = dispatcher->getCurrentContext();

  contextPair.writeContext = nullptr;
  contextPair.readContext = &listenerContext;

  epoll_event listenEvent;
  listenEvent.events = EPOLLIN | EPOLLONESHOT;
  listenEvent.data.ptr = &contextPair;
  std::string message;
  if (epoll_ctl(dispatcher->getEpoll(), EPOLL_CTL_MOD, listener, &listenEvent) == -1) {
    message = "epoll_ctl failed, " + lastErrorMessage();
  } else {
    context = &listenerContext;
    dispatcher->getCurrentContext()->interruptProcedure = [&]() {
        assert(dispatcher != nullptr);
        assert(context != nullptr);
        OperationContext* listenerContext = static_cast<OperationContext*>(context);
        if (!listenerContext->interrupted) {
          epoll_event listenEvent;
          listenEvent.events = 0;
          listenEvent.data.ptr = nullptr;

          if (epoll_ctl(dispatcher->getEpoll(), EPOLL_CTL_MOD, listener, &listenEvent) == -1) {
            throw std::runtime_error("UdpListener::stop, epoll_ctl failed, " + lastErrorMessage() );
          }

          listenerContext->interrupted = true;

          if (listener) {
          	shutdown(listener, SHUT_RDWR);
          }

          dispatcher->pushContext(listenerContext->context);
        }
    };

    dispatcher->dispatch();
    dispatcher->getCurrentContext()->interruptProcedure = nullptr;
    assert(dispatcher != nullptr);
    assert(listenerContext.context == dispatcher->getCurrentContext());
    assert(contextPair.writeContext == nullptr);
    assert(context == &listenerContext);
    context = nullptr;
    listenerContext.context = nullptr;
    if (listenerContext.interrupted) {
      throw InterruptedException();
    }

    if((listenerContext.events & (EPOLLERR | EPOLLHUP)) != 0) {
      throw std::runtime_error("UdpListener::accept, accepting failed");
    }

    sockaddr inAddr;
    socklen_t inLen = sizeof(inAddr);
    uint8_t buffer[MAX_SAFE_UDP_DATA_SIZE];
    struct sockaddr_storage src_addr;

    struct iovec iov[1];
    iov[0].iov_base = buffer;
    iov[0].iov_len = sizeof(buffer);

    struct msghdr message;
    message.msg_name = &src_addr;
    message.msg_namelen = sizeof(src_addr);
    message.msg_iov = iov;
    message.msg_iovlen = 1;
    message.msg_control = 0;
    message.msg_controllen = 0;

    ssize_t count = recvmsg(listener, &message, 0);

	if (count >= 0) {
		UdpPacket* udpPacket = new UdpPacket(&buffer[0], MAX_SAFE_UDP_DATA_SIZE);
		std::cout << "UDP Packet received: "  << *(udpPacket->getHexString()) << std::endl;
		return udpPacket;
	}


    return nullptr;
 }
//
//  throw std::runtime_error("UdpListener::receiveUdpPacket, " + message);
}

}
