/** ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
* @file urlProcessor
* @author Maxim Plička (xplick04)
*/

#include <filesystem>
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>

//openSSL
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#include "xmlParser.hpp"



class URLProcessor
{
    private:
        //class attributes
        std::vector<std::string> URLList;   //list of URLs from feedfile or one URL specified from user
        long unsigned curIndex = 0; //index for cycling threw URLList
        ArgumentParser argumentParser;  //Holds arguments from user
        BIO *bio;   //pointer to connection
        URLClass url;   //currently processed URL
        int numberOfSuccesses = 0;


    /**  Function for checking validity of URL
    *  @source of regex https://www.regextester.com/105539
    *  @param url
    *  @return true if url format is valid, false if not
    */
        bool isValid(std::string url);

    public:
    /**  URLProcessor constructor
    *
    *  @param argumentParserIN, class that holds arguments from user
    */
        URLProcessor(ArgumentParser argumentParserIN);

    /**  Function for parsing URLs and putting them into URLList
    *
    *   @return true if parsing was success, false if not
    */
        bool parseURL();

    /**  Function that contains main cycle for parsing URLs and sending them to XMLProcessor for printing.
    *    Function was inspired by https://developer.ibm.com/tutorials/l-openssl/
    *
    *   @return true if processing and printing was success, false if not
    */
        bool processURL();
};
