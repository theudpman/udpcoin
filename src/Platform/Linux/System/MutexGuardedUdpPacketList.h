// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "System/UdpPacket.h"

#include <string>
#include <vector>
#include <thread>
#include <mutex>


namespace System {

class Dispatcher;

class MutexGuardedUdpPacketList {

public:
	MutexGuardedUdpPacketList();
	MutexGuardedUdpPacketList(Dispatcher& dispatcher);
	~MutexGuardedUdpPacketList();
	MutexGuardedUdpPacketList(MutexGuardedUdpPacketList&& other);
	MutexGuardedUdpPacketList& operator=(MutexGuardedUdpPacketList&& other);
	UdpPacket* removeFirstElement();
	void appendElement(UdpPacket* packet);

private:
	std::vector<UdpPacket*> *packets;
	Dispatcher* dispatcher;
	void* context;
};

}
