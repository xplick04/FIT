/**
 * @file genchargerobot.cpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include <iostream>
#include "simlib.h"

#include "chargerobotprocess.cpp"

class GenChargeRobot: public Event
{
	void Behavior()
	{
		(new ChargeRobotProcess) -> Activate();
		Activate(Time + Normal(120, 3) + 10);
	}
};