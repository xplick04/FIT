/**
 * ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
* @file urlProcessor
* @author Maxim Plička (xplick04)
*/

#include "urlProcessor.hpp"

#define BUFFSIZE 1024

bool URLProcessor::isValid(std::string url)
{
    std::regex url_regex (
            "^(http(s)?:\\/\\/)[\\w.-]+(?:\\.[\\w\\.-]+)+[\\w\\-\\._~:/?#[\\]@!\\$&'\\(\\)\\*\\+,;=.]+$"
    );
    if(std::regex_match(url, url_regex)) return true;
    return false;
}

URLProcessor::URLProcessor(ArgumentParser argumentParserIN)
{
    argumentParser = argumentParserIN;
}

bool URLProcessor::parseURL()
{
    if(!argumentParser.getURL().empty())    //particular url was set in param
    {
        if(isValid(argumentParser.getURL()))    //valid url format
        {
            URLList.push_back(argumentParser.getURL());
        }
        else
        {
            std::cerr << "URL address \'" << argumentParser.getURL() << "\' is not valid"<< std::endl;    //invalid url format
            return false;
        }
    }
    else    //url in feedfile
    {
        std::ifstream file;
        file.open(argumentParser.getfeedfile());
        if (!file.is_open())
        {
            std::cerr << "Could not open file: " << argumentParser.getfeedfile() << std::endl;
            return false;
        }
        std::string line;
        while (std::getline(file, line))    //processing feedfile line by line
        {

            if(line.size() != 0 && line.at(0) != '#')   //line is not empty and is not commentary
            {
                if(isValid(line))
                {
                    URLList.push_back(line);  //valid url format
                }
                else
                {
                    std::cerr << "URL address \'" << line << "\' is not valid"<< std::endl;    //invalid url format
                }
            }
        }
        if(URLList.empty()) //no valid url was found
        {
            std::cerr << "No valid URL was found in " << argumentParser.getfeedfile() << std::endl;
            return false;
        }
    }
    return true;
}

bool URLProcessor::processURL()
{
    //OpenSSL library loading
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    XMLParser xmlParser(argumentParser);    //XMLParser init

    for(; curIndex < URLList.size(); curIndex++)    //cycle threw all URLs
    {
        url.load(URLList[curIndex]);    //sets url from URLList into URLClass
        SSL_CTX *ctx = nullptr;
        SSL *ssl = nullptr;

        if(url.getSecured())    //HTTPS
        {
            ctx = SSL_CTX_new(SSLv23_client_method());
            if(argumentParser.getcertaddr().empty() && argumentParser.getcertfile().empty())    //no certAddr, no certFile
            {
                SSL_CTX_set_default_verify_paths(ctx);
            }
            else
            {
                if(argumentParser.getcertfile().empty())  //certAddr, no certFile
                {
                    std::ifstream dir;
                    dir.open(argumentParser.getcertaddr().c_str());
                    if (!dir.is_open()) //checks if directory exists
                    {
                        std::cerr << "Certificate path "<< argumentParser.getcertaddr() << " does not exist "<< std::endl;
                        return false;
                    }
                    dir.close();
                    if(!SSL_CTX_load_verify_locations(ctx, nullptr, argumentParser.getcertaddr().c_str()))
                    {
                        std::cerr << "Certificate can not be loaded from " <<  argumentParser.getcertaddr() << std::endl;
                        continue;
                    }
                }
                else if(argumentParser.getcertaddr().empty())   //no certAddr, certFile
                {
                    if(!SSL_CTX_load_verify_locations(ctx, argumentParser.getcertfile().c_str(), nullptr))
                    {
                        std::cerr << "Certificate can not be loaded from " <<  argumentParser.getcertfile() << std::endl;
                        continue;
                    }
                }
                else    //certAddr, certFile
                {
                    if(!SSL_CTX_load_verify_locations(ctx, argumentParser.getcertfile().c_str(),
                                                      argumentParser.getcertaddr().c_str()))
                    {
                        std::cerr << "Certificate can not be loaded from " <<  argumentParser.getcertaddr()
                        << argumentParser.getcertfile() << std::endl;
                        continue;
                    }
                }
            }

            bio = BIO_new_ssl_connect(ctx);
            if (bio == nullptr)
            {
                std::cerr << "Could not open " <<  url.getURL() << std::endl;
                continue;
            }

            BIO_get_ssl(bio, &ssl);
            SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY); //SSL_MODE_AUTO_RETRY for unexpected handshakes with server
            BIO_set_conn_hostname(bio, url.getConnectionString());

            if(SSL_get_verify_result(ssl) != X509_V_OK)
            {
                std::cerr << "Certificate is not valid for " <<  url.getURL() << std::endl;
                continue;
            }
        }
        else    //HTTP
        {
            bio = BIO_new_connect(url.getConnectionString());
            if (bio == nullptr)
            {
                std::cerr << "Could not connect to " <<  url.getURL() << std::endl;
                continue;   //skipping rest of cycle
            }
        }

        if(BIO_do_connect(bio) <= 0)    //failed connection
        {
            std::cerr << "Could not connect to " <<  url.getURL() << std::endl;
            BIO_free_all(bio);  //bio free
            continue;   //skipping rest of cycle
        }

        std::string requestStr;
        requestStr = "GET " + url.getPath() + " HTTP/1.0\r\n"
                     "Host: " + url.getHost() + "\r\n"
                     "Connection: Close\r\n"
                     "\r\n";

        //write
        bool successfullWrite = false;
        do
        {
            if(BIO_write(bio, requestStr.c_str(), requestStr.length()) > 0)    //sends request to server
            {
                successfullWrite = true;
                break;
            }
        }
        while(BIO_should_retry(bio));

        if(!successfullWrite)
        {
            std::cerr << "Could not write into " <<  url.getURL() << std::endl;
            continue;   //skipping rest of cycle
        }

        //read
        char responsePart[BUFFSIZE];    //buffer for response
        bool successfullRead = false;
        int readResult = 0;

        do
        {
            do
            {
                memset(responsePart, 0,BUFFSIZE);   //deleting responsePart content
                readResult = BIO_read(bio, responsePart, BUFFSIZE - 1);  //reading
                if(readResult > 0)   //returned number of chars
                {
                    url.responseAdd(responsePart);
                    successfullRead = true;
                    break;
                }
                else if (readResult == 0)    //closed connection
                {
                    break;
                }
            }
            while(BIO_should_retry(bio));
        }
        while(readResult);  //read until readResult is 0 (connection closed)

        if(!successfullRead)
        {
            std::cerr << "Could not read from " <<  url.getURL() << std::endl;
            continue;   //skipping rest of cycle
        }

        bool XMLSuccess = false;   //bool to not print \n if first URL is not valid

        if(url.cleanResponse()) //deletes HTTP header from response
        {
            XMLSuccess = xmlParser.parseXML(url);
        }
        else
        {
            std::cerr << "No valid XML format in " << url.getURL() << std::endl;
        }

        if(URLList.size() >= 1 && curIndex != URLList.size() - 1 && XMLSuccess) //handles new line printing
        {
            std::cout << std::endl; //new line before another content
        }

        if(XMLSuccess)
        {
            numberOfSuccesses++;
        }

        if(ctx)
        {
            SSL_CTX_free(ctx);  //ctx free
        }
        if(bio)
        {
            BIO_free_all(bio);  //bio free
        }
    }
    if(numberOfSuccesses > 0)   //at least one XML was printed successfully
    {
        return true;
    }
    std::cerr << "No valid XML format in input URL(s)" << std::endl;
    return false;
}
