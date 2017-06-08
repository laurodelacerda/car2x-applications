
#include "PSEmergency.h"
#include <algorithm>

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

    if (onDuty == false){ // a vehicle which is not on Public Safety duty
        if (onRoute(wsm->getWsmData())) {
            changeLane();
            std::cout << ". I am changing lane!" << std::endl;
            // TODO Adicionar estatística para quantificar mudanças de faixa.
        }
    }

}

void PSEmergency::onWSA(WaveServiceAdvertisment* wsa) {
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
            startService((Channels::ChannelNumber) wsa->getTargetChannel(), wsa->getPsid(), "Emergency Warning!");
        }
    }
}

void PSEmergency::handleSelfMsg(cMessage* msg) {
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

void PSEmergency::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);
    //the vehicle has moved. Code that reacts to new positions goes here.
    //member variables such as currentPosition and currentSpeed are updated in the parent class

    if (onDuty){
        findHost()->getDisplayString().updateWith("r=16,red");

        WaveShortMessage* wsm = new WaveShortMessage();

        populateWSM(wsm);

        std::cout<< "I am vehicle number " << wsm->getSenderAddress() <<
                ". I am on duty! This is my route: " << getCourse(numberOfRoads) << std::endl;

        wsm->setWsmData(getCourse(numberOfRoads).c_str());
        //      wsm->setPsid(12);
        //      wsm->setPsc("Give way! Public Safety vehicle is on Duty!");
        //      wsm->setSecurityType(); TODO Implementar segurança

        if (dataOnSch) {
            startService(Channels::SCH2, 12, "Emergency Warning");
            //started service and server advertising, schedule message to self to send later
            scheduleAt(computeAsynchronousSendingTime(1,type_SCH),wsm);
        }
        else {
            //send right away on CCH, because channel switching is disabled
            sendDown(wsm);
        }
    }

}

std::string PSEmergency::formatRoad(std::string road) {
    return "[" + road + "]";
}

std::string PSEmergency::formatLane(int lane) {
    return "(" + std::to_string(lane) + ")";
}

std::string PSEmergency::getCourse(int maxNumberOfRoads) {
    //TODO Verificar o que ocorre quando a Rota Planejada é alterada em tempo de execução
    std::list<std::string> route = traciVehicle->getPlannedRoadIds();
    std::string roadInTransit = formatRoad(traciVehicle->getRoadId());
    std::string formattedCourse = "";

    int addedRoads = 0;
    bool addInMessage = false;

    //TODO Melhorar lógica
    for (std::list<std::string>::iterator roadId = route.begin(); roadId != route.end(); ++roadId) {
        std::string road = formatRoad(*roadId);
        if (road == roadInTransit)
            addInMessage = true;

        if (addInMessage) {
            if (addedRoads < maxNumberOfRoads) {
                formattedCourse = formattedCourse + road;
                addedRoads++;
            } else {
                break;
            }
        }
    }

    formattedCourse = formatLane(traciVehicle->getLaneIndex())
            + formattedCourse;

    return formattedCourse;
}

bool PSEmergency::onRoute(std::string course) {

    std::string roadInTransit = formatRoad(traciVehicle->getRoadId());
    std::string laneInTransit = formatLane(traciVehicle->getLaneIndex());

    if (course.find(roadInTransit) != std::string::npos)
        if (course.find(laneInTransit) != std::string::npos)
            return true; //on route

    return false; //not on route
}

void PSEmergency::changeLane() {
    if (traciVehicle->getLaneIndex() > 0) {
        traciVehicle->changeLane(traciVehicle->getLaneIndex()-1, 0);
    }
}
