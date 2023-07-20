/**
 * @file argparser.hpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include <iostream>
#include <getopt.h>
#include <string>

#ifndef ARGPARSER
#define ARGPARSER

class Argparser
{
private:
	int cars = 22;
	int workers = 20;
	int robots = 58;
	int packages = 0;
	std::string file = "out.txt";

public:
	Argparser(int argc, char *argv[]);
	int Cars();
	int Workers();
	int Robots();
	int Packages();
	std::string File();
};

#endif