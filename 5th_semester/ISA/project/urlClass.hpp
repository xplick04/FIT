/**
 * ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
* @file urlClass
* @author Maxim Plička (xplick04)
*/

#include <string>
#include <regex>

class URLClass
{
    private:
        //class attributes
        std::string url;
        bool secured;
        std::string host;
        std::string path;
        std::string connectionString;   //conection string for BIO_connect()
        std::string response;

    public:
    /**  Function for setting new params of urlClass
    *  @source regex from https://stackoverflow.com/questions/5620235/cpp-regular-expression-to-validate-url
    *  @param urlNew, new url passed from urlParser
    */
        void load(std::string urlNew);

    /**  Function for cleaning response string
    *
    *  @param urlNew, new url passed from urlParser
    */
        bool cleanResponse();

    /**  Function for checking if url is secured
    *
    *  @return true if url uses TLS(HTTPS), false if not(HTTP)
    */
        bool getSecured();

    /**  Function for adding part of response loaded in responsePart to response
    */
        void responseAdd(std::string responsePart);

    /**  Function for getting hostname
    *
    *  @return string that contains hostname
    */
        std::string getHost();

    /**  Function for getting url address
    *
    *  @return string that contains url address
    */
        std::string getURL();

    /**  Function for getting path
    *
    *  @return string that contains path
    */
        std::string getPath();

    /**  Function for getting whole response
    *
    *  @return string that contains response
    */
        std::string getResponse();

    /**  Function for getting connectionString for bio_connect
    *
    *  @return string that contains connectionString
    */
        const char* getConnectionString();
};
