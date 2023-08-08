/**
 * @file packageprocess.cpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include <iostream>
#include <algorithm>
#include "simlib.h"

#include "common.hpp"

class PackageProcess: public Process
{
	void Behavior()
	{
		double arrival = Time;
		incoming_packages++;
		Enter(*workers, 1); //seize worker
		Wait(Exponential(0.30)); //time to get and scan package
		Enter(*robots, 1); //seize robot
		Leave(*workers, 1); //unseize worker
		Wait(Uniform(0.40, 0.60)); //sort package
		Leave(*robots, 1); //unseize package
		package_hist(Time - arrival);
		sorted_packages++;
		if(incoming_packages == sorted_packages && cars_unloaded == num_of_cars)
		{
			time_to_sort = (int) Time;
		}
	}
};