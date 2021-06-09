// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "System/UdpPacket.h"

#include <vector>
#include <ios>
#include <stdio.h>

namespace System {


UdpPacket::UdpPacket(const uint8_t* data, size_t dataLen, size_t totalPackets, size_t packetSequence) {
	rawData = new std::vector<uint8_t>();
	fillRawDataVectorFromPointer(rawData, data, dataLen);

	numPackets = totalPackets;
	packetNum = packetSequence;
	udpSequenceHeader = CREATE_UDP_SEQUENCE_HEADER(totalPackets, packetSequence);
	rawData->insert(rawData->begin(), udpSequenceHeader);
	hexString = 0;
}

UdpPacket::UdpPacket(const uint8_t* data, size_t dataLen) {
	rawData = new std::vector<uint8_t>();
	fillRawDataVectorFromPointer(rawData, data, dataLen);

	udpSequenceHeader = (*rawData)[0];
	numPackets = GET_TOTAL_PACKETS(udpSequenceHeader);
	packetNum = GET_PACKET_SEQUENCE(udpSequenceHeader);
	hexString = 0;
}

UdpPacket::~UdpPacket() {
	delete rawData;

	if (hexString) {
		delete hexString;
	}
}

void UdpPacket::fillRawDataVectorFromPointer(std::vector<uint8_t>* data, const uint8_t* inputData, size_t dataLen) {
	for (int i = 0; i < dataLen; ++i) {
		data->insert(data->end(), inputData[i]);
	}
}

std::string* UdpPacket::getHexString() {
	if (!hexString) {
		hexString = bytesToString(getData(), getDataLength());
	}

	return hexString;

}

uint8_t* UdpPacket::getData() {
	return rawData->data();
}

uint16_t UdpPacket::getDataLength() {
	return rawData->size();
}

std::string* UdpPacket::bytesToString(uint8_t* bytes, int size) {
  std::string* str = new std::string();

  for (int i = 0; i < size; ++i) {
    const char ch = bytes[i];
    str->append(&hexIndexToChar[(ch  & 0xF0) >> 4], 1);
    str->append(&hexIndexToChar[ch & 0xF], 1);
  }

  return str;
}

}
