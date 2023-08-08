/** ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
*
* @author Maxim Plička (xplick04)
*/

#include <libxml/parser.h>
/* ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
* @file xmlParser
* @author Maxim Plička (xplick04)
*/

#include "argumentParser.hpp"
#include "urlClass.hpp"

class XMLParser
{
    private:
        //Class attributes
        xmlNodePtr active;
        xmlDocPtr doc;
        ArgumentParser argumentParser;
        bool first; //boolean for printing new line before first title

    /** Parsing first layer of XML format, printing main title and searching for items
    *
    */
        void parseAtom();

    /** Parsing item from atom format, that contains specific titles, authors, asociated URLs and dates
    *
    */
        void parseItem();

    /** Parsing first layer of XML format, printing main title and searching for entries
    *
    */
        void parseRSS();

    /** Parsing entry from RSS format, that contains specific titles, authors, asociated URLs and dates
    *
    */
        void parseEntry();

    public:
    /** XMLParser constructor
    *
    * @param argumentParserIN, contains arguments set by user
    */
        XMLParser(ArgumentParser argumentParserIN);

    /** Main branching for RSS and Atom format
    *
    *  @return true, if parsing and printing is successfull, false if not
    */
        bool parseXML(URLClass url);
};


