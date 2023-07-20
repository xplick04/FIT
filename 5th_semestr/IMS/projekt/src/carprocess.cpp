/**
 * @file carprocess.cpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include <iostream>
#include <algorithm>
#include "simlib.h"

#include "common.hpp"
#include "packageprocess.cpp"

class CarProcess: public Process
{
	void Behavior()
	{
		Priority = 1;
		Wait(Uniform(0, 240)); //arrival
		Enter(*ramps, 1); //seize ramp
		
		for (int i = 0; i < 4; i++)
		{
			Enter(workers, 1);
		}

		int num_of_packages = (int) Normal(1000, 200); //packages to unload

		Wait(Uniform(20, 25)); //unloading
		cars_unloaded++;
		for (int i = 0; i < num_of_packages; i++)
		{
			(new PackageProcess()) -> Activate();
		}
		
		if(!prev_packages.Empty())
		{
			num_of_packages = (int) Normal(1000, 200); //packages to load
			num_of_packages = std::min(num_of_packages,(int) prev_packages.Used());
			Leave(*prev_packages, num_of_packages); //packages from prev shift
			Wait(Uniform(15, 20)); //loading
		}
		Leave(*ramps, 1); //unseize ramp
		Leave(*workers, 4); //unseize workers
	}
};