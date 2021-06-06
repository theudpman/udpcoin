// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "System/UdpPacket.h"

#include <vector>

namespace System {


UdpPacket::UdpPacket(const uint8_t* data, size_t dataLen, size_t totalPackets, size_t packetSequence) {
	rawData = new std::vector<uint8_t>();
	fillRawDataVectorFromPointer(rawData, data, dataLen);

	numPackets = totalPackets;
	packetNum = packetSequence;
	udpSequenceHeader = CREATE_UDP_SEQUENCE_HEADER(totalPackets, packetSequence);
	rawData->insert(rawData->begin(), udpSequenceHeader);
}

UdpPacket::UdpPacket(const uint8_t* data, size_t dataLen) {
	rawData = new std::vector<uint8_t>();
	fillRawDataVectorFromPointer(rawData, data, dataLen);

	udpSequenceHeader = (*rawData)[0];
	numPackets = GET_TOTAL_PACKETS(udpSequenceHeader);
	packetNum = GET_PACKET_SEQUENCE(udpSequenceHeader);
}

UdpPacket::~UdpPacket() {
	delete rawData;
}

void UdpPacket::fillRawDataVectorFromPointer(std::vector<uint8_t>* data, const uint8_t* inputData, size_t dataLen) {
	for (int i = 0; i < dataLen; ++i) {
		data->insert(data->end(), inputData[i]);
	}
}

std::string UdpPacket::hexString() {

}

uint8_t* UdpPacket::getData() {
	return rawData->data();
}

uint16_t UdpPacket::getDataLength() {
	return rawData->size();
}

}
