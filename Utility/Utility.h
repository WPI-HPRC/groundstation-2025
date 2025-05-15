//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_UTILITY_H
#define GS_BACKEND_2024_2025_UTILITY_H

#include "json_struct.h"

#define GROUND_STATION_ADDR 0x0013A200423F474C

namespace RadioPerformanceStat
{
    enum PacketType
    {
        NoPacketType = 0x00,
        Rocket = 0x01,
        Payload = 0x02,
        Combined = 0x03
    };

    enum StatType
    {
        NoStatType = 0x00,
        Throughput = 0x01,
        Count = 0x02
    };

    enum DataType
    {
        NoDataType = 0x00,
        Packets = 0x01,
        Bytes = 0x02
    };
}


#endif //GS_BACKEND_2024_2025_UTILITY_H
