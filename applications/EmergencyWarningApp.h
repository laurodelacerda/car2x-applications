
/*      This application message uses PSID header "0x0C" for emergency-warning messages.
 *      This message is based on the ISO 15628
 */

#ifndef EmergencyWarningApp_H_
#define EmergencyWarningApp_H_

#include <omnetpp.h>
#include <veins/modules/application/ieee80211p/BaseWaveApplLayer.h>
#include <veins/modules/application/v2x_applications/drivers/BaseDriver.h>
#include <veins/modules/application/v2x_applications/gps/BaseGPS.h>
#include <veins/modules/application/v2x_applications/security/BaseCryptography.h>

class EmergencyWarningApp : public BaseWaveApplLayer {

    public:
        EmergencyWarningApp(int roads);
        virtual void initialize(int stage);
        // virtual void finish();

    protected:
        BaseGPS* gps;
        BaseCryptography* cryptography;
        BaseDriver* driver;
        int numberOfRoads;
        int currentSubscribedServiceId;

    protected:
        // virtual void onBSM(BasicSafetyMessage* bsm); TODO Implementação futura
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);


};
#endif /* EmergencyWarningApp_H_ */
