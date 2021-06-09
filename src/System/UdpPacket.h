// Copyright (c) 2021 The UDP Coin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <string>
#include <vector>


#define		MAX_SAFE_UDP_DATA_SIZE				507
#define		UDP_SEQUENCE_HEADER					1

#define		IP_HEADER_SIZE						60
#define		UDP_HEADER_SIZE						8

#define 	MAX_SAFE_UDP_PACKET_SIZE			(MAX_SAFE_UDP_DATA_SIZE + UDP_SEQUENCE_HEADER + IP_HEADER_SIZE + UDP_HEADER_SIZE)


#define 	GET_TOTAL_PACKETS(x)				((x & 0xf0) >> 4)
#define		GET_PACKET_SEQUENCE(x)				(x & 0x0f)

#define		CREATE_UDP_SEQUENCE_HEADER(t, s)	((t << 4) | s)

char const hexIndexToChar[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F'};


namespace System {

class UdpPacket {

public:
	UdpPacket(const uint8_t* data, size_t dataLen);
	UdpPacket(const uint8_t* data, size_t dataLen, size_t totalPackets, size_t packetSequence);
	~UdpPacket();
	std::string* getHexString();
	uint8_t* getData();
	uint16_t getDataLength();

private:
	void fillRawDataVectorFromPointer(std::vector<uint8_t>* data, const uint8_t* inputData, size_t dataLen);
	std::string* bytesToString(uint8_t* bytes, int size);
	std::vector<uint8_t> *rawData;
	uint8_t numPackets;
	uint8_t packetNum;
	uint8_t udpSequenceHeader;
	std::string* hexString;

};

}
