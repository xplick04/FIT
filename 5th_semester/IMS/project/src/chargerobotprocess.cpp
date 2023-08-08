/**
 * @file chargerobotprocess.cpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include <iostream>
#include "simlib.h"

#include "common.hpp"

class ChargeRobotProcess: public Process
{
	void Behavior()
	{
		charges++;
		Priority = 1;
		Enter(robots, 1);
		Wait(10);
		Leave(robots, 1);
	}
};