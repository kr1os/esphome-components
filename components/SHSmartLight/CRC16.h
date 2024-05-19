#pragma once
#include <vector>
#include <algorithm>

using namespace std;

class CRC16
{
private:
    const std::vector<int> ANSI_CRC16R_TABLE = { 0, 49345, 49537, 320, 49921, 960, 640, 49729, 50689, 1728, 1920, 51009, 1280, 50625, 50305, 1088, 52225, 3264, 3456, 52545, 3840, 53185, 52865, 3648, 2560, 51905, 52097, 2880, 51457, 2496, 2176, 51265, 55297, 6336, 6528, 55617, 6912, 56257, 55937, 6720, 7680, 57025, 57217, 8000, 56577, 7616, 7296, 56385, 5120, 54465, 54657, 5440, 55041, 6080, 5760, 54849, 53761, 4800, 4992, 54081, 4352, 53697, 53377, 4160, 61441, 12480, 12672, 61761, 13056, 62401, 62081, 12864, 13824, 63169, 63361, 14144, 62721, 13760, 13440, 62529, 15360, 64705, 64897, 15680, 65281, 16320, 16000, 65089, 64001, 15040, 15232, 64321, 14592, 63937, 63617, 14400, 10240, 59585, 59777, 10560, 60161, 11200, 10880, 59969, 60929, 11968, 12160, 61249, 11520, 60865, 60545, 11328, 58369, 9408, 9600, 58689, 9984, 59329, 59009, 9792, 8704, 58049, 58241, 9024, 57601, 8640, 8320, 57409, 40961, 24768, 24960, 41281, 25344, 41921, 41601, 25152, 26112, 42689, 42881, 26432, 42241, 26048, 25728, 42049, 27648, 44225, 44417, 27968, 44801, 28608, 28288, 44609, 43521, 27328, 27520, 43841, 26880, 43457, 43137, 26688, 30720, 47297, 47489, 31040, 47873, 31680, 31360, 47681, 48641, 32448, 32640, 48961, 32000, 48577, 48257, 31808, 46081, 29888, 30080, 46401, 30464, 47041, 46721, 30272, 29184, 45761, 45953, 29504, 45313, 29120, 28800, 45121, 20480, 37057, 37249, 20800, 37633, 21440, 21120, 37441, 38401, 22208, 22400, 38721, 21760, 38337, 38017, 21568, 39937, 23744, 23936, 40257, 24320, 40897, 40577, 24128, 23040, 39617, 39809, 23360, 39169, 22976, 22656, 38977, 34817, 18624, 18816, 35137, 19200, 35777, 35457, 19008, 19968, 36545, 36737, 20288, 36097, 19904, 19584, 35905, 17408, 33985, 34177, 17728, 34561, 18368, 18048, 34369, 33281, 17088, 17280, 33601, 16640, 33217, 32897, 16448 };
    const std::vector<int> ANSI_CRC16_TABLE = { 0, 32773, 32783, 10, 32795, 30, 20, 32785, 32819, 54, 60, 32825, 40, 32813, 32807, 34, 32867, 102, 108, 32873, 120, 32893, 32887, 114, 80, 32853, 32863, 90, 32843, 78, 68, 32833, 32963, 198, 204, 32969, 216, 32989, 32983, 210, 240, 33013, 33023, 250, 33003, 238, 228, 32993, 160, 32933, 32943, 170, 32955, 190, 180, 32945, 32915, 150, 156, 32921, 136, 32909, 32903, 130, 33155, 390, 396, 33161, 408, 33181, 33175, 402, 432, 33205, 33215, 442, 33195, 430, 420, 33185, 480, 33253, 33263, 490, 33275, 510, 500, 33265, 33235, 470, 476, 33241, 456, 33229, 33223, 450, 320, 33093, 33103, 330, 33115, 350, 340, 33105, 33139, 374, 380, 33145, 360, 33133, 33127, 354, 33059, 294, 300, 33065, 312, 33085, 33079, 306, 272, 33045, 33055, 282, 33035, 270, 260, 33025, 33539, 774, 780, 33545, 792, 33565, 33559, 786, 816, 33589, 33599, 826, 33579, 814, 804, 33569, 864, 33637, 33647, 874, 33659, 894, 884, 33649, 33619, 854, 860, 33625, 840, 33613, 33607, 834, 960, 33733, 33743, 970, 33755, 990, 980, 33745, 33779, 1014, 1020, 33785, 1000, 33773, 33767, 994, 33699, 934, 940, 33705, 952, 33725, 33719, 946, 912, 33685, 33695, 922, 33675, 910, 900, 33665, 640, 33413, 33423, 650, 33435, 670, 660, 33425, 33459, 694, 700, 33465, 680, 33453, 33447, 674, 33507, 742, 748, 33513, 760, 33533, 33527, 754, 720, 33493, 33503, 730, 33483, 718, 708, 33473, 33347, 582, 588, 33353, 600, 33373, 33367, 594, 624, 33397, 33407, 634, 33387, 622, 612, 33377, 544, 33317, 33327, 554, 33339, 574, 564, 33329, 33299, 534, 540, 33305, 520, 33293, 33287, 514 };
    const std::vector<int> CCITT_CRC16R_TABLE = { 0, 4489, 8978, 12955, 17956, 22445, 25910, 29887, 35912, 40385, 44890, 48851, 51820, 56293, 59774, 63735, 4225, 264, 13203, 8730, 22181, 18220, 30135, 25662, 40137, 36160, 49115, 44626, 56045, 52068, 63999, 59510, 8450, 12427, 528, 5017, 26406, 30383, 17460, 21949, 44362, 48323, 36440, 40913, 60270, 64231, 51324, 55797, 12675, 8202, 4753, 792, 30631, 26158, 21685, 17724, 48587, 44098, 40665, 36688, 64495, 60006, 55549, 51572, 16900, 21389, 24854, 28831, 1056, 5545, 10034, 14011, 52812, 57285, 60766, 64727, 34920, 39393, 43898, 47859, 21125, 17164, 29079, 24606, 5281, 1320, 14259, 9786, 57037, 53060, 64991, 60502, 39145, 35168, 48123, 43634, 25350, 29327, 16404, 20893, 9506, 13483, 1584, 6073, 61262, 65223, 52316, 56789, 43370, 47331, 35448, 39921, 29575, 25102, 20629, 16668, 13731, 9258, 5809, 1848, 65487, 60998, 56541, 52564, 47595, 43106, 39673, 35696, 33800, 38273, 42778, 46739, 49708, 54181, 57662, 61623, 2112, 6601, 11090, 15067, 20068, 24557, 28022, 31999, 38025, 34048, 47003, 42514, 53933, 49956, 61887, 57398, 6337, 2376, 15315, 10842, 24293, 20332, 32247, 27774, 42250, 46211, 34328, 38801, 58158, 62119, 49212, 53685, 10562, 14539, 2640, 7129, 28518, 32495, 19572, 24061, 46475, 41986, 38553, 34576, 62383, 57894, 53437, 49460, 14787, 10314, 6865, 2904, 32743, 28270, 23797, 19836, 50700, 55173, 58654, 62615, 32808, 37281, 41786, 45747, 19012, 23501, 26966, 30943, 3168, 7657, 12146, 16123, 54925, 50948, 62879, 58390, 37033, 33056, 46011, 41522, 23237, 19276, 31191, 26718, 7393, 3432, 16371, 11898, 59150, 63111, 50204, 54677, 41258, 45219, 33336, 37809, 27462, 31439, 18516, 23005, 11618, 15595, 3696, 8185, 63375, 58886, 54429, 50452, 45483, 40994, 37561, 33584, 31687, 27214, 22741, 18780, 15843, 11370, 7921, 3960 };
    const std::vector<int> CCITT_CRC16_TABLE = { 0, 4129, 8258, 12387, 16516, 20645, 24774, 28903, 33032, 37161, 41290, 45419, 49548, 53677, 57806, 61935, 4657, 528, 12915, 8786, 21173, 17044, 29431, 25302, 37689, 33560, 45947, 41818, 54205, 50076, 62463, 58334, 9314, 13379, 1056, 5121, 25830, 29895, 17572, 21637, 42346, 46411, 34088, 38153, 58862, 62927, 50604, 54669, 13907, 9842, 5649, 1584, 30423, 26358, 22165, 18100, 46939, 42874, 38681, 34616, 63455, 59390, 55197, 51132, 18628, 22757, 26758, 30887, 2112, 6241, 10242, 14371, 51660, 55789, 59790, 63919, 35144, 39273, 43274, 47403, 23285, 19156, 31415, 27286, 6769, 2640, 14899, 10770, 56317, 52188, 64447, 60318, 39801, 35672, 47931, 43802, 27814, 31879, 19684, 23749, 11298, 15363, 3168, 7233, 60846, 64911, 52716, 56781, 44330, 48395, 36200, 40265, 32407, 28342, 24277, 20212, 15891, 11826, 7761, 3696, 65439, 61374, 57309, 53244, 48923, 44858, 40793, 36728, 37256, 33193, 45514, 41451, 53516, 49453, 61774, 57711, 4224, 161, 12482, 8419, 20484, 16421, 28742, 24679, 33721, 37784, 41979, 46042, 49981, 54044, 58239, 62302, 689, 4752, 8947, 13010, 16949, 21012, 25207, 29270, 46570, 42443, 38312, 34185, 62830, 58703, 54572, 50445, 13538, 9411, 5280, 1153, 29798, 25671, 21540, 17413, 42971, 47098, 34713, 38840, 59231, 63358, 50973, 55100, 9939, 14066, 1681, 5808, 26199, 30326, 17941, 22068, 55628, 51565, 63758, 59695, 39368, 35305, 47498, 43435, 22596, 18533, 30726, 26663, 6336, 2273, 14466, 10403, 52093, 56156, 60223, 64286, 35833, 39896, 43963, 48026, 19061, 23124, 27191, 31254, 2801, 6864, 10931, 14994, 64814, 60687, 56684, 52557, 48554, 44427, 40424, 36297, 31782, 27655, 23652, 19525, 15522, 11395, 7392, 3265, 61215, 65342, 53085, 57212, 44955, 49082, 36825, 40952, 28183, 32310, 20053, 24180, 11923, 16050, 3793, 7920 };
    const int bMAX_VALUE = 255;
public:
    CRC16()
    {

    };

    int get_crc16(vector<int> iArr, vector<int8_t> bArr, int i, int i2, int i3)
    {
        int i4 = i3 & 65535;
        for (int i5 = 0; i5 < std::min(i2, (int)bArr.size()); i5++)
        {
            i4 = iArr[((i4 >> 8) ^ bArr[i + i5]) & 255] ^ (i4 << 8);
        }
        return i4;
    };

    int get_crc16r(vector<int> iArr, vector <int8_t> bArr, int i, int i2, int i3)
    {
        uint8_t b = (uint8_t)(i3 & bMAX_VALUE);
        for (int i4 = 0; i4 < std::min(i2, (int)bArr.size()); i4++)
        {
            b = (uint8_t)(iArr[(b ^ bArr[i + i4]) & bMAX_VALUE] ^ (b >> 8));
        }
        return b;
    };

    int get_ansi_crc16(vector<int8_t> bArr, int i, int i2, int i3)
    {
        return get_crc16(this->ANSI_CRC16_TABLE, bArr, i, i2, i3);
    };

    int get_ansi_crc16r(vector<int8_t> bArr, int i, int i2, int i3)
    {
        return get_crc16r(ANSI_CRC16R_TABLE, bArr, i, i2, i3);
    };

    int get_ccitt_crc16(vector<int8_t> bArr, int i, int i2, int i3)
    {
        return get_crc16(CCITT_CRC16_TABLE, bArr, i, i2, i3);
    };

    int get_ccitt_crc16r(vector<int8_t> bArr, int i, int i2, int i3)
    {
        return get_crc16r(CCITT_CRC16R_TABLE, bArr, i, i2, i3);
    };
};