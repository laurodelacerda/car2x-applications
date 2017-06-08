
//package org.car2x.veins.modules.application.v2x_applications.applications;
package org.car2x.veins.modules.application.car2x.apps;
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;

simple PSEmergency extends BaseWaveApplLayer
{
    @class(PSEmergency);
    @display("i=block/app2");
    string appName = default("Emergency Warning Application");
    int numberOfRoads = default(4);
    bool onDuty = default(false);
    //wsaInterval = 0.5s ; // @lauro: Parâmetro declarado na superclasse 
}
