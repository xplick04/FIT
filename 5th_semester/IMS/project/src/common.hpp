/**
 * @file common.hpp
 * @author Jan Zdeněk (xzdene01), Maxim Plička (xplick04)
 * @date 2022-12-01
 */

#include "simlib.h"

#ifndef COMMON
#define COMMON

extern Store ramps;
extern Store workers;
extern Store robots;
extern Store prev_packages;

extern Histogram package_hist;

extern int charges;
extern int incoming_packages;
extern int sorted_packages;
extern int time_to_sort;
extern int num_of_cars;
extern int cars_unloaded;

#endif