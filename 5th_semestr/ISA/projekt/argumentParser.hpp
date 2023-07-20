/**
 * ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
 * @file ArgumentParser
 * @author Maxim Plička (xplick04)
 */

#include <iostream>
#include <getopt.h>
#include <string>

/**
 * Parsing arguments from user, user input error handler
 *
 *  @param argc
 *  @param *argv[]
 */
class ArgumentParser
{
    private:
        //holds arguments specified from user
        std::string url;
        std::string feedfile;
        std::string certfile;
        std::string certaddr;
        bool TFlag = false; //time flag
        bool aFlag = false; //author flag
        bool uFlag = false; //asociated URL flag

    public:
    /** Parsing arguments from user, user input error handler
    *
    * @param argc
    * @param *argv[]
    * @return true if arguments are valid, false if not
    */
        bool parseArgs(int argc,char *argv[]);

    /**  Single URL getter, only if feedfile was not specified
    *
    *  @return URL address
    */
        std::string getURL();

    /**  Feedfile getter
    *
    *  @return address of feedfile
    */
        std::string getfeedfile();

    /**  Certificate file address getter
    *
    *  @return certificate directory address
    */
        std::string getcertfile();

    /**  Certificate directory address getter
    *
    *  @return certificate directory address
    */
        std::string getcertaddr();

    /**  Time flag getter
    *
    *  @return true if flag was set true, false if not
    */
        bool getTFlag();

    /**  Author flag getter
    *
    *  @return true if flag was set true, false if not
    */
        bool getaFLag();

    /**  Asociated URL flag getter
    *
    *  @return true if flag was set true, false if not
    */
        bool getuFlag();
};