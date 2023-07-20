/**
 * @file prevpackages.cpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include <iostream>
#include "simlib.h"

#include "common.hpp"

class PrevPackages: public Process
{
private:
	int packages;

	void Behavior()
	{
		Enter(*prev_packages, packages);
	}

public:
	PrevPackages(int packages_in)
	{
		packages = packages_in;
	}
};