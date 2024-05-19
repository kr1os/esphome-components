#pragma once
#include <vector>
#include <string>
#include "CRC16.h"

class SHSmartLight
{
private:
	const char HEX_ARRAY[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	
	int payload_length = 16;
	int sendCount = 0;
	const int advertising_channel[3] = {37, 38, 39};
	vector<int8_t> RF_ADDR = {-10, -100, 84, 110, 85};
	long SEND_DEFAULT_TIME = 3000;
	std::string roomId = "C2F3A528";
	std::string controlId = "01";
	vector<int8_t> rawPrefix = {0x02,0x01,0x1A,0x1B,(int8_t)0xFF,0x22,(int8_t)0x97};
	CRC16 crc16;

	int crc_start() {
		return payload_length + 8;
	}
	int packet_length() {
		return payload_length + 8 + 2;
	}
	std::string bytesToHex(vector<int8_t> bytes);
	void fill_zero(vector<int8_t> & mBytes);
	vector<int8_t> Build(vector<int8_t>& vArr, vector<int8_t>& vArr2, int i);
	vector<int8_t> get_HsRfPacket_iOSCompatible(vector<int8_t>& vArr, vector<int8_t>& vArr2);
	void ble_whitening(vector<int8_t>& vArr, int i, vector<int8_t>& vWhite);
	void ble_whitening_ex(vector<int8_t>& vArr, int i, int i2, vector <int8_t>& vWhite);
	void ble_whitening_for_rf_packet(vector<int8_t>& vArr, int i, vector <int8_t>& vWhite);
	vector<int8_t> bit_reverse_in_byte(vector<int8_t>& vArr);
	std::string startTx(vector<int8_t>& vArr, vector<int8_t>& vArr2, int i);
	std::string sendCmd(vector<int8_t> & vArr, vector<int8_t>& vArr2, int i);
	std::string sendStatusOpCmd(int i, int i2);
	std::string sendStatusOpCmd(int i);
	std::string sendStatusWyCmd(int8_t bright, int8_t cct);
	vector<int8_t> hexToBytes(std::string str);
	std::string getCommandPrefix()
	{
		return std::string(rawPrefix.begin(), rawPrefix.end());
	}
public:
	std::string getTurnOnCmd(bool raw = false)
	{
		return ((raw == true) ? getCommandPrefix() : "") + sendStatusOpCmd(0x03);
	}
	std::string getTurnOffCmd(bool raw = false)
	{
		return ((raw == true) ? getCommandPrefix() : "") + sendStatusOpCmd(0x04);
	}
	std::string getCCTCmd(int8_t bright, int8_t cct, bool raw = false)
	{
		return ((raw == true) ? getCommandPrefix() : "") + sendStatusWyCmd(bright, cct);
	}
	void setRoomID(std::string roomID) { this->roomId = roomID; }
	void setControlID(std::string controlId) { this->controlId = controlId;}
	std::string getRoomID() { return this->roomId; }
	std::string getControlID() { return this->controlId; }
};

