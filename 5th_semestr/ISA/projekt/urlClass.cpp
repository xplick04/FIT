/**
 * ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
* @file urlClass
* @author Maxim Plička (xplick04)
*/

#include "urlClass.hpp"

bool URLClass::cleanResponse()
{
    std::string::size_type pos = response.find("<?xml");   //finds start of xml document
    if (pos == std::string::npos)
    {
        return false; //could not find '<'
    }
    response = response.substr(pos, response.length());  //removes http(s) header from response
    return true;
}

void URLClass::responseAdd(std::string responsePart)
{
    response = response + responsePart;
}

void URLClass::load(std::string urlNew)
{
    response.clear();   //cleans response string
    url = urlNew;

    if(!url.compare(0,8,"https://"))
    {
        secured = true; //secured flag set
    }
    else
    {
        secured = false;
    }

/***************************************************************************************
*    Title: CPP + Regular Expression to Validate URL
*    Author: Ciro Costa
*    Date: Jul 24, 2015 at 14:33
*    Availability: https://stackoverflow.com/questions/5620235/cpp-regular-expression-to-validate-url
 *   Licence:
***************************************************************************************/
    std::regex url_regex (
            R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
            std::regex::extended
    );
    std::smatch url_match_result;
    std::regex_match(url, url_match_result, url_regex);

    host = url_match_result[4]; //host set

    bool portSpecified;
    std::string::size_type pos = host.find(':');    //finding position of : if port was specified
    if (pos == std::string::npos)   //port was not specified
    {
        portSpecified = false;
    }
    else
    {
        portSpecified = true;
    }

    if(url_match_result[5] == "")
    {
        path = "/"; //path for HTTP request
    }
    else
    {
        path = url_match_result[5];
    }

    if(!portSpecified)  //port specified
    {
        if(secured)
        {
            connectionString = host + ":443";    //connection string set
        }
        else
        {
            connectionString = host + ":80";
        }
    }
    else
    {
        connectionString = host;
        host = host.substr(0, pos);
    }
}

bool URLClass::getSecured()
{
    return secured;
}

std::string URLClass::getHost()
{
    return host;
}

std::string URLClass::getURL()
{
    return url;
}

std::string URLClass::getResponse()
{
    return response;
}

std::string URLClass::getPath()
{
    return path;
}

const char* URLClass::getConnectionString()
{
    return connectionString.c_str();
}
