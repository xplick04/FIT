/**
 * @file simulator.cpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include <iostream>
#include "simlib.h"

#include "argparser.hpp"
#include "prevpackages.cpp"
#include "carprocess.cpp"
#include "genchargerobot.cpp"
#include "breakprocess.cpp"
#include "common.hpp"

Store ramps;
Store workers;
Store robots;
Store prev_packages;

Histogram package_hist("package time to sort", 0, 4, 40);

int charges;
int incoming_packages;
int sorted_packages;
int time_to_sort = 510;
int num_of_cars;
int cars_unloaded;

int main(int argc, char *argv[]) {
	RandomSeed(time(NULL));

	//arg parser
	Argparser argparser(argc, argv);
	num_of_cars = argparser.Cars();

	Init(0, 510);
	SetOutput(argparser.File().c_str());
	ramps.SetName("ramps");
	ramps.SetCapacity(10);

	robots.SetName("robots");
	robots.SetCapacity(argparser.Robots());

	workers.SetName("workers");
	workers.SetCapacity(argparser.Workers());

	prev_packages.SetName("prev_packages");
	prev_packages.SetCapacity(100000);

	(new PrevPackages(argparser.Packages())) -> Activate();

	for (int i = 0; i < argparser.Cars(); i++)
	{
		(new CarProcess()) -> Activate();
	}

	for (int i = 0; i < argparser.Robots(); i++)
	{
		(new GenChargeRobot()) -> Activate(Time + Uniform(0, 120));
	}

	(new BreakProcess(argparser.Workers())) -> Activate(Time + 360);
	

	Run();

	ramps.Output();
	workers.Output();
	robots.Output();
	prev_packages.Output();
	package_hist.Output();

	std::cout << "Number of charges:" << charges << std::endl
	<< "Number of incoming packages:" << incoming_packages << std::endl
	<< "Number of sorted packages:" << sorted_packages << std::endl
	<< "Time to sort all packages:" << time_to_sort << std::endl
	<< "Workers:" << argparser.Workers() << std::endl
	<< "Robots:" << argparser.Robots() << std::endl
	<< "Cars:" << argparser.Cars() << std::endl
	<< "Prev packages:" << argparser.Packages() << std::endl; 

	return 0;
}