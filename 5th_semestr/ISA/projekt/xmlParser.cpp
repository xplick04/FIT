/**
 * ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
* @file xmlParser
* @author Maxim Plička (xplick04)
*/

#include "xmlParser.hpp"

void XMLParser::parseItem() //RSS format
{
    active = active->xmlChildrenNode;   //diving in item
    xmlChar* content;
    bool titleFlag = true;  //flag for printing only one title if more were found

    while(1)
    {
        if(!xmlStrcmp(active->name, (const xmlChar *) "title") && titleFlag)
        {
            if((argumentParser.getTFlag() || argumentParser.getaFLag() || argumentParser.getuFlag()) && !first)
            {
                std::cout << std::endl;
            }
            first = false;  //bool for not printing new line in the beginning
            content = xmlNodeListGetString(doc, active->xmlChildrenNode, 1);
            std::cout << content << std::endl;
            titleFlag = false;
            xmlFree(content);
        }
        else if (!xmlStrcmp(active->name, (const xmlChar *) "link") && argumentParser.getuFlag())
        {
            content = xmlNodeListGetString(doc, active->xmlChildrenNode, 1);
            std::cout << "Link: " << content << std::endl;
            xmlFree(content);
        }
        else if (!xmlStrcmp(active->name, (const xmlChar *) "pubDate") && argumentParser.getTFlag())
        {
            content = xmlNodeListGetString(doc, active->xmlChildrenNode, 1);
            std::cout << "Aktualizace: " << content << std::endl;
            xmlFree(content);
        }
        else if (!xmlStrcmp(active->name, (const xmlChar *) "author") && argumentParser.getaFLag())
        {
            content = xmlNodeListGetString(doc, active->xmlChildrenNode, 1);
            std::cout << "Autor: " << content  << std::endl;
            xmlFree(content);
        }
        if(!active->next)
        {
            break;    //next is null
        }
        active = active->next;
    }
    active = active->parent;     //emerging from item
}

void XMLParser::parseEntry()    //atom format
{
    active = active->xmlChildrenNode;   //diving in entry
    xmlChar* content;
    bool titleFlag = true;  //flag for printing only one title if more were found

    while(1)
    {
        if(!xmlStrcmp(active->name, (const xmlChar *) "title") && titleFlag)
        {
            if((argumentParser.getTFlag() || argumentParser.getaFLag() || argumentParser.getuFlag()) && !first)
            {
                std::cout << std::endl;
            }
            first = false;
            content = xmlNodeListGetString(doc, active->xmlChildrenNode, 1);
            std::cout << content << std::endl;
            xmlFree(content);
            titleFlag = false;
        }
        else if (!xmlStrcmp(active->name, (const xmlChar *) "link") && argumentParser.getuFlag())
        {
            std::cout << "URL: " << active->properties->children->content << std::endl;
        }
        else if (!xmlStrcmp(active->name, (const xmlChar *) "updated") && argumentParser.getTFlag())
        {
            content = xmlNodeListGetString(doc, active->xmlChildrenNode, 1);
            std::cout << "Aktualizace: " << content << std::endl;
            xmlFree(content);
        }
        else if (!xmlStrcmp(active->name, (const xmlChar *) "author") && argumentParser.getaFLag())
        {
            active = active->xmlChildrenNode;   //diving in title
            std::cout << "Autor: ";
            while(1)    //printing all author tags
            {
                content = xmlNodeListGetString(doc, active->xmlChildrenNode, 1);
                std::cout << content << ' ';

                if(!active->next)
                {
                    break;    //next is null
                }
                active = active->next;
            }
            xmlFree(content);
            std::cout << std::endl;
            active = active->parent;    //emerging from title
        }
        if(!active->next)
        {
            break;    //next is null
        }
        active = active->next;
    }
    active = active->parent;    //emerging from entry
}

void XMLParser::parseAtom()
{
    xmlChar* content;
    while(1)
    {
        if(!xmlStrcmp(active->name, (const xmlChar *) "title"))
        {
            content = xmlNodeListGetString(doc, active->xmlChildrenNode, 1);
            std::cout << "*** " << content << " ***" << std::endl;
            xmlFree(content);
        }
        else if (!xmlStrcmp(active->name, (const xmlChar *) "entry"))
        {
            parseEntry();
        }
        if(!active->next)
        {
            break;    //next is null
        }
        active = active->next;
    }
}

void XMLParser::parseRSS()
{
    while(1)    //cycle until active name is channel
    {
        if(!xmlStrcmp(active->name, (const xmlChar *) "channel"))
        {
            break;
        }
        else if(active->next == nullptr)
        {
            return;
        }
        active = active->next;
    }

    active = active->xmlChildrenNode;  //diving in channel
    xmlChar* content;
    while(1)
    {
        if (!xmlStrcmp(active->name, (const xmlChar *) "title"))
        {
            content = xmlNodeListGetString(doc, active->xmlChildrenNode, 1);
            std::cout << "*** " << content << " ***" << std::endl;
            xmlFree(content);
        }
        else if (!xmlStrcmp(active->name, (const xmlChar *) "item"))
        {
            parseItem();
        }
        if(!active->next)
        {
            break;    //next is null
        }
        active = active->next;
    }
}

XMLParser::XMLParser(ArgumentParser argumentParserIN)
{
    argumentParser = argumentParserIN;
}

bool XMLParser::parseXML(URLClass url)
{
    first = true;   //bool for not printing new line before first title
    doc = xmlReadMemory(url.getResponse().c_str(), url.getResponse().length(), NULL, NULL, XML_PARSE_NOERROR);

    if(doc == nullptr)
    {
        std::cerr << "Wrong xml format in " << url.getURL() << std::endl;
        xmlCleanupParser();
        return false;
    }
    active = xmlDocGetRootElement(doc);

    if(!xmlStrcmp(active->name, (const xmlChar *) "feed"))
    {
        active = active->xmlChildrenNode;   //diving in feed
        parseAtom();
    }
    else if(!xmlStrcmp(active->name, (const xmlChar *) "rss"))
    {
        active = active->xmlChildrenNode;   //diving in rss
        parseRSS();
    }
    else
    {
        std::cerr << "No supported xml format in " << url.getURL() << std::endl;
        xmlCleanupParser();
        xmlFreeDoc(doc);
        return false;
    }
    xmlCleanupParser();
    xmlFreeDoc(doc);
    return true;
}
