#include <list>
#include <string>
#include <stdint.h>

#include <veins/modules/mobility/traci/TraCICommandInterface.h>
#include "veins/modules/mobility/traci/TraCIBuffer.h"
#include "veins/modules/mobility/traci/TraCIColor.h"
#include "veins/base/utils/Coord.h"

/*
 * TODO Implementar as funções extras estendendo a classe TraCICommandInterface
 */

class MyTraCI{

    protected:
        // @author: Lauro de Lacerda & Arthur Zopellaro
        void changeLane(uint8_t laneIndex, int32_t time);
        void setLaneChangeMode (int32_t bitset);
        //

};
