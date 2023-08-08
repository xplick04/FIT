/**
 * @file breakprocess.cpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include <iostream>
#include "simlib.h"

#include "common.hpp"

class BreakProcess: public Process
{
private:
	int num_workers;

	void Behavior()
	{
		Priority = 2;
		double break_start = Time;
		for (int i = 0; i < num_workers; i++)
		{
			Enter(workers, 1);
		}
		double time = Time - break_start;
		Wait(30 - time);
		Leave(workers, num_workers);
	}
public:
	BreakProcess(int num_workers_in)
	{
		num_workers = num_workers_in;
	}
};