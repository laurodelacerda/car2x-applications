
#include <stdlib.h>
#include "MyTraCI.h"

void MyTraCI::setLaneChangeMode(int32_t bitset){
    uint8_t variableId = VAR_LANECHANGE_MODE;
    uint8_t variableType = TYPE_INTEGER;
    TraCIBuffer buf = traci->connection.query(CMD_SET_VEHICLE_VARIABLE, TraCIBuffer() << variableId << nodeId << variableType << bitset);
    ASSERT(buf.eof());
}

void MyTraCI::changeLane(uint8_t laneIndex, int32_t time){
    uint8_t variableId = CMD_CHANGELANE;
    uint8_t variableType = TYPE_COMPOUND;
    int32_t count = 2;
    uint8_t laneIndexType = TYPE_BYTE;
    uint8_t durationType = TYPE_INTEGER;
    TraCIBuffer buf = connection->query(CMD_SET_VEHICLE_VARIABLE, TraCIBuffer() << variableId << nodeId << variableType << count << laneIndexType << laneIndex << durationType << time);
        ASSERT(buf.eof());
}

