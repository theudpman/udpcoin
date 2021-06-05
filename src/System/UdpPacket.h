// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <string>

namespace System {

class UdpPacket {

public:
	UdpPacket();
	std::string hexString();
};

}
