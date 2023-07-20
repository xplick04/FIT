/**
 * ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
 * File that contains main cycle of the program
 * @file feedreader
 * @author Maxim Plička (xplick04)
 */

#include "feedreader.hpp"

int main(int argc, char *argv[])
{
    ArgumentParser argumentParser;  //ArgumentParser init
    if(!argumentParser.parseArgs(argc, argv))
    {
        return 1;
    }

    URLProcessor urlProcessor(argumentParser);  //URLProcessor init
    if(!urlProcessor.parseURL())
    {
        return 1;
    }
    if(!urlProcessor.processURL())
    {
        return 1;
    }
    return 0;
}
