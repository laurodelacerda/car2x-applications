
#ifndef TraCITest_H
#define TraCITest_H

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

class TraCITest : public BaseWaveApplLayer {
	public:
		virtual void initialize(int stage);
	protected:
		simtime_t lastDroveAt;
		bool sentMessage;
		int currentSubscribedServiceId;
	protected:
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
		virtual void handlePositionUpdate(cObject* obj);
};

#endif
