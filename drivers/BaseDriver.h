
#ifndef BaseDriver_H
#define BaseDriver_H

/*  @lauro:
 *  TODO Criar uma classe que estenda as funcionalidades da classe TraCICommandInterface::Vehicle
 *  com as funções relativas a mudança de faixa.
 *
 */


#include "veins/modules/mobility/traci/TraCICommandInterface.h"

using Veins::TraCICommandInterface;

class BaseDriver {

protected:
    TraCICommandInterface::Vehicle* traciVehicle;

public:
    virtual void changeLane() = 0;

};

#endif
