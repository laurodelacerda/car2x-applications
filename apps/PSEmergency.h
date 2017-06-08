
#ifndef _PSEmergency_H_
#define _PSEmergency_H_

#include <omnetpp.h>
#include <veins/modules/application/car2x/drivers/SimpleDriver.h>
#include <veins/modules/application/car2x/gps/SimpleGPS.h>
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
        SimpleGPS* gps;
        SimpleDriver* driver;

    protected:
        virtual void onBSM(BasicSafetyMessage* bsm);
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);
    };

#endif
