
package org.car2x.veins.modules.application.car2x.examples;

import org.car2x.veins.nodes.RSU;
import org.car2x.veins.nodes.Scenario;

network Simple extends Scenario
{
	parameters:
        int numberOfRSUs = default(0); //whether to print debug output
    submodules:
        rsu[numberOfRSUs]: RSU {
            @display("p=150,140;b=10,10,oval");
        }
}

