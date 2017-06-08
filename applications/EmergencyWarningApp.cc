
#include <veins/modules/application/traci/EmergencyWarningApp.h>

Define_Module(EmergencyWarningApp);

void EmergencyWarningApp::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        //Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;
        //onDuty = par("onDuty").boolValue();
//        cryptography = new SimpleCryptography();
        numberOfRoads = par("numberOfRoads");

        currentSubscribedServiceId = -1;
    }
    else if (stage == 1) {
        //Initializing members that require initialized other modules goes here
    }
}

//void EmergencyWarningApp::finish() {
//    BaseWaveApplLayer::finish();
//    //statistics recording goes here
//
//}

//void EmergencyWarningApp::onBSM(BasicSafetyMessage* bsm) {
//    //Your application has received a beacon message from another car or RSU
//    //code for handling the message goes here
//
//}

void EmergencyWarningApp::onWSM(WaveShortMessage* wsm) {
    //Your application has received a data message from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples

    /*
     * @lauro
     * TODO Adicionar conteúdo da função onData
     * TODO Verificar outras possibilidades de checagem na classe wsm
     * TODO Pesquisar o que significa o campo serial
     * TODO Descobrir o significado da função wsm->dup()
     * */

    findHost()->getDisplayString().updateWith("r=16,green");

    if (gps->onRoute(wsm->getWsmData())) {
            driver->changeLane();
    }

}

void EmergencyWarningApp::onWSA(WaveServiceAdvertisment* wsa) {
    //Your application has received a service advertisement from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples

    /* @lauro
     *  Trocas de canal ocorrem a partir do recebimento de WSAs
     *  TODO Aqui verificar o PSID da msg recebida para seleção de canal
     *  Em seu exemplo, o Veins usa o psid 42 (não alocado)
     *  TODO EmergencyWarning usa o PSID decimal 12
     * */

    if (currentSubscribedServiceId == -1) {
        mac->changeServiceChannel(wsa->getTargetChannel());
        currentSubscribedServiceId = wsa->getPsid();
        if  (currentOfferedServiceId != wsa->getPsid()) {
            stopService();
            startService((Channels::ChannelNumber) wsa->getTargetChannel(), wsa->getPsid(), "Emergency Warning");
        }
    }
}

void EmergencyWarningApp::handleSelfMsg(cMessage* msg) {
    BaseWaveApplLayer::handleSelfMsg(msg);
    //this method is for self messages (mostly timers)
    //it is important to call the BaseWaveApplLayer function for BSM and WSM transmission

    /* @lauro
     * O Veins trabalha o channel switching especialmente nesta função
     * Para a aplicação de emergência talvez fosse necessário ter troca de canal
     *
     * Configurações no omnetpp.ini seriam:
     * *.**.nic.mac1609_4.useServiceChannel = true
     * *.node[*].appl.dataOnSch = true
     * *.rsu[*].appl.dataOnSch = true
    **/

    if (WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg)) {
        //send this message on the service channel until the counter is 3 or higher.
        //this code only runs when channel switching is enabled
        sendDown(wsm->dup());
        wsm->setSerial(wsm->getSerial() +1);
        if (wsm->getSerial() >= 3) {
            //stop service advertisements
            stopService();
            delete(wsm);
        }
        else {
            scheduleAt(simTime()+1, wsm);
        }
    }
    else {
        BaseWaveApplLayer::handleSelfMsg(msg);
    }
}

void EmergencyWarningApp::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);
    //the vehicle has moved. Code that reacts to new positions goes here.
    //member variables such as currentPosition and currentSpeed are updated in the parent class

    if (onDuty){
    // @lauro: Neste caso, apenas a ambulância ficará vermelha.
        findHost()->getDisplayString().updateWith("r=16,red");
        WaveShortMessage* wsm = new WaveShortMessage();
        populateWSM(wsm);
        wsm->setWsmData(gps->getCourse(numberOfRoads));
        //wsm->setSecurityType(); TODO Implementar segurança
        wsm->setPsid(12);
        wsm->setPsc("Give way! Public Safety vehicle is on Duty!");
        wsm->setChannelNumber(Channels::CRIT_SOL);
    }

}
