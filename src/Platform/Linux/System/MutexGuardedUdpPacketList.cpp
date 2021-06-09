// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "System/MutexGuardedUdpPacketList.h"

#include <cassert>
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

std::mutex mtxGuardingUdpPacketList;

MutexGuardedUdpPacketList::MutexGuardedUdpPacketList() : dispatcher(nullptr) {
	context = nullptr;
	packets = new std::vector<UdpPacket*>();
}

MutexGuardedUdpPacketList::MutexGuardedUdpPacketList(Dispatcher& dispatcher) : dispatcher(&dispatcher) {
	packets = new std::vector<UdpPacket*>();
	context = nullptr;
}

MutexGuardedUdpPacketList::~MutexGuardedUdpPacketList() {

	if (packets != nullptr) {
		delete packets;
	}

	if (dispatcher != nullptr) {
	    assert(context == nullptr);
	}
}

MutexGuardedUdpPacketList::MutexGuardedUdpPacketList(MutexGuardedUdpPacketList&& other) : dispatcher(other.dispatcher) {
  if (other.dispatcher != nullptr) {
    assert(other.context == nullptr);
    context = nullptr;
    other.dispatcher = nullptr;
  }

  packets = new std::vector<UdpPacket*>();
}

MutexGuardedUdpPacketList& MutexGuardedUdpPacketList::operator=(MutexGuardedUdpPacketList&& other) {
  if (dispatcher != nullptr) {
    assert(context == nullptr);
  }

  dispatcher = other.dispatcher;
  if (other.dispatcher != nullptr) {
    assert(other.context == nullptr);
    context = nullptr;
    other.dispatcher = nullptr;
  }

  return *this;
}

UdpPacket* MutexGuardedUdpPacketList::removeFirstElement() {
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
      context = &listenerContext;
      dispatcher->getCurrentContext()->interruptProcedure = [&]() {
          assert(dispatcher != nullptr);
          assert(context != nullptr);
          OperationContext* listenerContext = static_cast<OperationContext*>(context);
          if (!listenerContext->interrupted) {
            epoll_event listenEvent;
            listenEvent.events = 0;
            listenEvent.data.ptr = nullptr;


            listenerContext->interrupted = true;

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

	UdpPacket* retVal = nullptr;

	if (packets->size() == 0) {
		return retVal;
	}

	{
		std::lock_guard<std::mutex> guard{mtxGuardingUdpPacketList};
		retVal = (*packets)[0];
		packets->erase(packets->begin());
	}

	return retVal;
}

void MutexGuardedUdpPacketList::appendElement(UdpPacket* packet) {
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

	{
		std::lock_guard<std::mutex> guard{mtxGuardingUdpPacketList};
		packets->insert(packets->end(), packet);

	}
}

}
