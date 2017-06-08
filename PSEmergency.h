
#ifndef _PSEmergency_H_
#define _PSEmergency_H_

#include <omnetpp.h>
#include <veins/modules/application/ieee80211p/BaseWaveApplLayer.h>

using namespace omnetpp;

class PSEmergency : public BaseWaveApplLayer {

    public:
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
        virtual std::string formatRoad(std::string road);
        virtual std::string formatLane(int lane);
        virtual std::string getCourse(int maxNumberOfRoads);
        virtual bool onRoute(std::string course);

        // functions that emulate a Driver
        virtual void changeLane();

    };

#endif
