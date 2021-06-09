// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "CryptoNote.h"
#include <Common/MemoryInputStream.h>
#include <Common/VectorOutputStream.h>
#include "Serialization/KVBinaryInputStreamSerializer.h"
#include "Serialization/KVBinaryOutputStreamSerializer.h"
#include "System/UdpPacket.h"


namespace System {
	class UdpPacket;
	class TcpConnection;
}

namespace CryptoNote {

class LevinUdpProtocol {
public:
  LevinUdpProtocol(System::TcpConnection& connection);
  void sendUdpMessage(uint32_t command, const BinaryArray& out, std::string udpPort);
  bool parseUdpPacket(System::UdpPacket* udpPacket);

private:
  bool readStrict(uint8_t* ptr, size_t size);
  void writeStrict(const uint8_t* ptr, size_t size);
  System::TcpConnection& m_conn;
};

}
