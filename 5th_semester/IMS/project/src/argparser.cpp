/**
 * @file argparser.cpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include "argparser.hpp"

Argparser::Argparser(int argc, char *argv[])
{
	int opt;

	static struct option long_options[] =
	{
		{0, required_argument, 0, 'c'},
		{0, required_argument, 0, 'w'},
		{0, required_argument, 0, 'r'},
		{0, required_argument, 0, 'p'},
		{0, no_argument, 0, 'h'},
		{0, required_argument, 0, 'f'},
		{0, 0, 0, 0}
	};

	while((opt = getopt_long(argc, argv, "c:w:r:p:hf:", long_options, 0)) != -1)
	{
		switch(opt)
		{
			case 'c':
				try
				{
					cars = std::stoi(optarg);
					if(cars <= 0)
					{
						std::cout << "Number of cars must be grater than 0." << std::endl;
						exit(1);
					}
				}
				catch(...)
				{
					std::cout << "Number of cars is not a number." << std::endl;
					exit(1);
				}
				continue;
			case 'w':
				try
				{
					workers = std::stoi(optarg);
					if(workers <= 0)
					{
						std::cout << "Number of workers must be grater than 0." << std::endl;
						exit(1);
					}
				}
				catch(...)
				{
					std::cout << "Number of workers is not a number." << std::endl;
					exit(1);
				}
				continue;
			case 'r':
				try
				{
					robots = std::stoi(optarg);
					if(robots <= 0)
					{
						std::cout << "Number of robots must be grater than 0." << std::endl;
						exit(1);
					}
				}
				catch(...)
				{
					std::cout << "Number of robots is not a number." << std::endl;
					exit(1);
				}
				continue;
			case 'p':
				try
				{
					packages = std::stoi(optarg);
					if(packages <= 0 || packages > 100000)
					{
						std::cout << "Number of packages must be grater than 0 and less than 100000." << std::endl;
						exit(1);
					}
				}
				catch(...)
				{
					std::cout << "Number of packages is not a number." << std::endl;
					exit(1);
				}
				continue;
			case 'h':
				std::cout << "-----HELP-----" << std::endl
				<< "./simulator [-c <cars>] [-w <workers>] [-r <robots>] [-p <prev packages>] [-f <file>] [-h]" << std::endl
				<< "-c <cars>: počet aut, které přijedou za směnu" <<std::endl
				<< "-w <workers>: počet pracovníků depa" << std::endl
				<< "-r <robots>: počet robotů v depu" <<std::endl
				<< "-p <prev packages>: počet balíků z předchozí směny" <<std::endl
				<< "-f <file>: výstupní soubor pro SIMLIB statistiky (ostatní statistiky jsou tisknuty na standardní výstup)" << std::endl
				<< "-h: vypíše pomoc pro spuštění simulátoru na standardní výstup" <<std::endl;
				exit(0);
			case 'f':
				file.assign(optarg);
				continue;
			default:
				std::cout << "Wrong argument." << std::endl;
				exit(1);
		}
	}
}

int Argparser::Cars()
{
	return cars;
}

int Argparser::Workers()
{
	return workers;
}

int Argparser::Robots()
{
	return robots;
}

int Argparser::Packages()
{
	return packages;
}

std::string Argparser::File()
{
	return file;
}