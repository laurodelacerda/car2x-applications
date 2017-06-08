
#ifndef _PSEmergency_H_
#define _PSEmergency_H_

#include <omnetpp.h>
#include <veins/modules/application/ieee80211p/BaseWaveApplLayer.h>

using namespace omnetpp;

class PSEmergency : public BaseWaveApplLayer {

    public:
        PSEmergency();
        virtual void initialize(int stage);
        virtual void finish();

    protected:
        int numberOfRoads;
        bool onDuty;
        int currentSubscribedServiceId;

    protected:
        virtual void onBSM(BasicSafetyMessage* bsm);
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);

        // functions that emulate a GPS
        std::string formatRoad(std::string road);
        std::string formatLane(int lane);
        std::string getCourse(int maxNumberOfRoads);
        bool onRoute(std::string course);

        // functions that emulate a Driver
        void changeLane();

    };

#endif
