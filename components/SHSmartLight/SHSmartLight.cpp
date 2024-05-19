#include "SHSmartLight.h"
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "CRC16.h"

using namespace std;

void SHSmartLight::fill_zero(vector<int8_t> & mBytes)
{
    for (int i = 0; i < mBytes.size(); i++)
    {
        mBytes[i] = 0;
    }
}

vector<int8_t> SHSmartLight::Build(vector<int8_t>& vArr, vector<int8_t>& vArr2, int i)
{
    payload_length = i;
    int packet_length = this->packet_length();
    vector<int8_t> mBytes(packet_length);
    fill_zero(mBytes);
    mBytes[0] = (vArr[0] & -128) == 128 ? (int8_t) - 86 : 85;
    std::copy(vArr.begin(), vArr.begin() + std::min((int)vArr.size(), 5), mBytes.begin()+1);
    for (int i2 = 0; i2 < 2; i2++)
    {
        mBytes[i2 + 6] = mBytes[5];
    }
    std::copy(vArr2.begin(), vArr2.begin() + std::min((int)vArr2.size(), payload_length), mBytes.begin()+8);
    int i3 = crc16.get_ccitt_crc16(mBytes, 8, payload_length, crc16.get_ccitt_crc16(mBytes, 1, 5, 65535));
    mBytes[crc_start()] = (int8_t)((i3 >> 8) & 255);
    mBytes[crc_start() + 1] = (int8_t)(i3 & 255);

    return mBytes;
}

vector<int8_t> SHSmartLight::get_HsRfPacket_iOSCompatible(vector<int8_t>& vArr, vector<int8_t>& vArr2)
{
    vector<int8_t> vArr3(16);

    std::copy(vArr2.begin(), vArr2.begin() + std::min((int)vArr2.size(), 8), vArr3.begin());
    
    if (vArr2.size() >= 9) {
        vArr3[11] = vArr2[8];
        if (vArr2.size() >= 10) {
            vArr3[13] = vArr2[9];
            if (vArr2.size() >= 11) {
                vArr3[15] = vArr2[10];
            }
        }
    }
    vArr3[8] = 76;
    vArr3[9] = -1;
    vArr3[10] = 0;
    vArr3[12] = 1;
    vArr3[14] = 2;
    return Build(vArr, vArr3, vArr3.size());
}


void SHSmartLight::ble_whitening(vector<int8_t>&  vArr, int i, vector<int8_t>& vWhite)
{
    int i2 = (((i & 1) << 6) | ((i & 32) >> 4) | 1 | ((i & 16) >> 2) | ((i & 8) << 0) | ((i & 4) << 2) | ((i & 2) << 4)) & 255;
    int i3 = 0;
    while (i3 < vArr.size())
    {
        int i4 = i2;
        int8_t b = 0;
        for (int i5 = 0; i5 < 8; i5++) {
            int i6 = i4 & 255;
            b |= ((((i6 & 64) >> 6) << i5) ^ (vArr[i3] & 255)) & (1 << i5);
            int i7 = i6 << 1;
            int i8 = (i7 >> 7) & 1;
            int i9 = (i7 & -2) | i8;
            i4 = ((i9 ^ (i8 << 4)) & 16) | (i9 & -17);
        }
        vWhite[i3] = (int8_t)b;
        i3++;
        i2 = i4;
    }
}

void SHSmartLight::ble_whitening_ex(vector<int8_t>& vArr, int i, int i2, vector<int8_t>& vWhite)
{
    vector<int8_t> vArr2(vArr.size() + i2);
    std::copy(vArr.begin(), vArr.end(), vArr2.begin() + i2);
    vector<int8_t> vBle_Whitening(vArr.size() + i2);
    this->ble_whitening(vArr2, i, vBle_Whitening);
    vWhite = vector<int8_t>(vArr.size());
    std::copy(vBle_Whitening.begin() + i2, vBle_Whitening.end(), vWhite.begin());
}

void SHSmartLight::ble_whitening_for_rf_packet(vector<int8_t>& vArr, int i, vector<int8_t>& vWhite)
{
    return this->ble_whitening_ex(vArr, i, 13, vWhite);
}

vector<int8_t> SHSmartLight::bit_reverse_in_byte(vector<int8_t>& vArr)
{
    int vs = vArr.size();
    vector<int8_t> vArrReversed(vs);

    for (int i3 = 0; i3 < vs; i3++) {
        int i4 = 0;
        for (int i5 = 0; i5 < 8; i5++) {
            i4 += (((vArr[i3] & 255) >> (7 - i5)) & 1) << i5;
        }
        vArrReversed[i3] = (int8_t)i4;
    }
    return vArrReversed;
}


std::string SHSmartLight::startTx(vector<int8_t> & vArr, vector<int8_t> & vArr2, int i)
{

    vector<int8_t> data = get_HsRfPacket_iOSCompatible(vArr, vArr2);
    vector<int8_t> revdata = this->bit_reverse_in_byte(data);
    vector<int8_t> vWhite;
    this->ble_whitening_for_rf_packet(revdata, advertising_channel[0], vWhite);

    int i2 = ((vWhite[1] & 255) << 8) + (vWhite[0] & 255);

    vector<int8_t> vArr3(vWhite.size() - 2);

    std::copy(vWhite.begin() + 2, vWhite.end(), vArr3.begin());
    //std::cout << bytesToHex(vArr3) << std::endl;
    //std::cout << "end;" << std::endl;
    return std::string(vArr3.begin(), vArr3.end());
}

std::string SHSmartLight::sendCmd(vector<int8_t> & vArr, vector<int8_t>& vArr2, int i)
{
    vector<int8_t> vArr3(11);
    for (int i2 = 0; i2 < vArr2.size(); i2++)
    {
        vArr3[i2] = vArr2[i2];
    }
    int i3 = sendCount;
    vArr3[9] = (int8_t)i3;
    vArr3[10] = (int8_t)(~i3);
    sendCount = sendCount + 1;
    if (sendCount > 254)
        sendCount = 0;
    return startTx(vArr, vArr3, i);
}

std::string SHSmartLight::sendStatusOpCmd(int i, int i2)
{
    vector<int8_t> h2b = hexToBytes(roomId);
    int hexByte = std::stol(controlId, nullptr, 16);
    vector<int8_t> tmpVec = { (int8_t)hexByte, 0, (int8_t)i, 0, 0, h2b[0], h2b[1], h2b[2], h2b[3] };
    return sendCmd(RF_ADDR, tmpVec, i2);
}
std::string SHSmartLight::sendStatusWyCmd(int8_t bright, int8_t cct)
{
    vector<int8_t> h2b = hexToBytes(roomId);
    int hexByte = std::stol(controlId, nullptr, 16);
    vector<int8_t> tmpVec = { (int8_t)hexByte, 1, (int8_t)bright, (int8_t) cct, 0, h2b[0], h2b[1], h2b[2], h2b[3] };
    return sendCmd(RF_ADDR, tmpVec, (int)SEND_DEFAULT_TIME);
}

std::string SHSmartLight::sendStatusOpCmd(int i)
{
    return sendStatusOpCmd(i, (int)SEND_DEFAULT_TIME);
}

vector<int8_t> SHSmartLight::hexToBytes(std::string str)
{
    int length = str.length() / 2;
    vector<int8_t> vArr(length);
    for (int i = 0; i < length; i++)
    {
        int i2 = i * 2;
        std::string strByte = str.substr(i2, 2);
        int hexByte  = std::stol(strByte, nullptr, 16);
        vArr[i] = (int8_t)hexByte; 
    }
    return vArr;
}
std::string SHSmartLight::bytesToHex(vector<int8_t> bytes)
{
    vector<char> hexChars(bytes.size() * 3);
    for (int j = 0; j < bytes.size(); j++)
    {
        int v = bytes[j] & 0xFF;
        hexChars[j * 3] = HEX_ARRAY[v >> 4];
        hexChars[j * 3 + 1] = HEX_ARRAY[v & 0x0F];
        hexChars[j * 3 + 2] = ' ';
    }
    return std::string(hexChars.begin(), hexChars.end());
}