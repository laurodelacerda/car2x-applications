
#include <veins/modules/application/car2x/apps/PSEmergency.h>

Define_Module(PSEmergency);

void PSEmergency::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        //Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;

        // read parameters
        numberOfRoads = par("numberOfRoads");
        onDuty = par("onDuty").boolValue();
        currentSubscribedServiceId = -1;
    }
    else if (stage == 1) {
        //Initializing members that require initialized other modules goes here

    }
}

void PSEmergency::finish() {
    BaseWaveApplLayer::finish();
    //statistics recording goes here

}

void PSEmergency::onBSM(BasicSafetyMessage* bsm) {
    //Your application has received a beacon message from another car or RSU
    //code for handling the message goes here

}

void PSEmergency::onWSM(WaveShortMessage* wsm) {
    //Your application has received a data message from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples

    /*
     * @lauro
     * TODO Adicionar conteúdo da função onData
     * TODO Verificar outras possibilidades de checagem na classe wsm
     * TODO Pesquisar o que significa o campo serial
     * TODO Descobrir o significado da função wsm->dup()
     * TODO Informar qual tipo de gps e driver são utilizados
     * */

    findHost()->getDisplayString().updateWith("r=16,green");

    if (onDuty == false){
        if (gps->onRoute(wsm->getWsmData())) {
                driver->changeLane();
        }
    }

}

void PSEmergency::onWSA(WaveServiceAdvertisment* wsa) {
    //Your application has received a service advertisement from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples

}

void PSEmergency::handleSelfMsg(cMessage* msg) {
    BaseWaveApplLayer::handleSelfMsg(msg);
    //this method is for self messages (mostly timers)
    //it is important to call the BaseWaveApplLayer function for BSM and WSM transmission

}

void PSEmergency::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);
    //the vehicle has moved. Code that reacts to new positions goes here.
    //member variables such as currentPosition and currentSpeed are updated in the parent class

}
