#include <iostream>
#include <getopt.h>
#include <string.h>
#include <pcap/pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netinet/ip6.h>

void argParse(int argc, char* argv[], char *interface, char *filter, int *numberOfPackets, int *interfaceFlag)
{
    const struct option long_options[] =
            {
                    {"tcp", no_argument, 0, 't'},
                    {"udp", no_argument, 0, 'u'},
                    {"arp", no_argument, 0, 'a'},
                    {"icmp", no_argument, 0, 'c'},
                    {0,0,0,0}
            };
    int opt, portNum;
    // main cycle for parsing arguments
    while ((opt = getopt_long(argc, argv, ":i:p:n:tu", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'i':
                strcpy(interface, optarg);
                *interfaceFlag = 1;
                break;
            case 'p':
                try
                {
                    portNum = std::stoi(optarg);
                }
                catch(std::invalid_argument& e)
                {
                    fprintf(stderr,"Port number must be integer\n");
                    exit(1);
                }
                catch(...)  // catches all other exceptions
                {
                    fprintf(stderr,"Other error in -p argument\n");
                    exit(1);
                }
                break;
            case 't':
                if(!strcmp(filter, ""))
                {
                    strcpy(filter, "tcp"); // adding tcp option into filter
                }
                else
                {
                    strcat(filter, " or tcp"); // adding tcp option into filter
                }
                break;
            case 'u':
                if(!strcmp(filter, ""))
                {
                    strcpy(filter, "udp"); // adding udp option into filter
                }
                else
                {
                    strcat(filter, " or udp"); // adding upd option into filter
                }
                break;
            case 'n':
                try
                {
                    *numberOfPackets = std::stoi(optarg);
                }
                catch(std::invalid_argument& e)
                {
                    fprintf(stderr,"Number of ports must be integer\n");
                    exit(1);
                }
                catch(...)  // catches all other exceptions
                {
                    fprintf(stderr,"Other error in -n argument\n");
                    exit(1);
                }
                break;
            case 'a':
                if(!strcmp(filter, ""))
                {
                    strcpy(filter, "arp");  // adding arp option into filter
                }
                else
                {
                    strcat(filter, " or arp"); // adding arp option into filter
                }
                break;
            case 'c':
                if(!strcmp(filter, ""))
                {
                    strcpy(filter, "icmp"); // adding icmp option into filter
                }
                else
                {
                    strcat(filter, " or icmp"); // adding icmp option into filter
                }
                break;
            case ':':
                if(optopt == 'i') // missing argument in opotions -i
                {
                    *interfaceFlag = 0;
                }
                else if(optopt == 'p') // missing arugment in options -p
                {
                    fprintf(stderr,"-p needs argument\n");
                    exit(1);
                }
                break;
            default: // unrecognised argument
                fprintf(stderr,"Wrong argument format\n");
                printf("Usage:\n -i [interface name]\n -p [port number]\n -t or --tcp\n -u or --udp\n -n or [number of packets]\n --udp\n --icmp\n");
                exit(1);
        }
    }
    if(portNum != 0)    // port number was set
    {
        if(!strcmp(filter, ""))
        {

            sprintf(filter, "port %d", portNum); // adding port option into filter
        }
        else
        {
            char tmp[30];
            sprintf(tmp, " port %d", portNum);
            strcat(filter, tmp); // adding port option into filter
        }
    }
}

void printInterface()
{
    pcap_if_t *devs;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&devs, errbuf) == -1) {
        fprintf(stderr,"%s\n", errbuf);
        exit(1);
    }

    if(!devs)   // if any device was not found
    {
        printf("No devices are currently connected");
    }

    printf("Connected devices:\n");
    int count = 1;
    while(devs) // printing elements from device list
    {
        printf("%d. %s\n", count, devs->name);
        count++;
        devs = devs->next;
    }

    //cleanup
    pcap_freealldevs(devs);
}

// source: sniffex.c
void print_hex_ascii_line(const u_char *payload, int len, int offset)
{
    int i;
    int gap;
    const u_char *ch;

    /* offset */
    printf("0x%04x   ", offset); //modified print

    /* hex */
    ch = payload;
    for(i = 0; i < len; i++) {
        printf("%02x ", *ch);
        ch++;
        /* print extra space after 8th byte for visual aid */
        if (i == 7)
            printf(" ");
    }
    /* print space to handle line less than 8 bytes */
    if (len < 8)
        printf(" ");

    /* fill hex gap with spaces if not full line */
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            printf("   ");
        }
    }
    printf("   ");

    /* ascii (if printable) */
    ch = payload;
    for(i = 0; i < len; i++) {
        if (isprint(*ch))
            printf("%c", *ch);
        else
            printf(".");
        ch++;
    }
    printf("\n");
}

// source: sniffex.c
void printPayload(const u_char *payload, int len)
{
    printf("\n");
    int len_rem = len;
    int line_width = 16;			/* number of bytes per line */
    int line_len;
    int offset = 0;					/* zero-based offset counter */
    const u_char *ch = payload;

    if (len <= 0)
        return;

    /* data fits on one line */
    if (len <= line_width) {
        print_hex_ascii_line(ch, len, offset);
        return;
    }

    /* data spans multiple lines */
    for ( ;; ) {
        /* compute current line length */
        line_len = line_width % len_rem;
        /* print line */
        print_hex_ascii_line(ch, line_len, offset);
        /* compute total remaining */
        len_rem = len_rem - line_len;
        /* shift pointer to remaining bytes to print */
        ch = ch + line_len;
        /* add offset */
        offset = offset + line_width;
        /* check if we have line width chars or less */
        if (len_rem <= line_width) {
            /* print last line and get out */
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
    }
}

/*
  * Author name : nategoose
  * Date : Sep 16, 2010 at 15:11
  * Type : source code
  * Web address : https://stackoverflow.com/questions/3727421/expand-an-ipv6-address-so-i-can-print-it-to-stdout
 */
void ipv6_to_str_unexpanded(char *str, const struct in6_addr *addr)
{
    sprintf(str, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
            (int)addr->s6_addr[0], (int)addr->s6_addr[1],
            (int)addr->s6_addr[2], (int)addr->s6_addr[3],
            (int)addr->s6_addr[4], (int)addr->s6_addr[5],
            (int)addr->s6_addr[6], (int)addr->s6_addr[7],
            (int)addr->s6_addr[8], (int)addr->s6_addr[9],
            (int)addr->s6_addr[10], (int)addr->s6_addr[11],
            (int)addr->s6_addr[12], (int)addr->s6_addr[13],
            (int)addr->s6_addr[14], (int)addr->s6_addr[15]);
}

void parseTCP(const u_char *packetData)
{
    struct tcphdr *tHead = (struct tcphdr*) packetData;
    printf("src port: %d\n", ntohs(tHead->th_sport));
    printf("dst port: %d\n", ntohs(tHead->th_dport));
    printf("------------------------------------------------------------------\n");
}

void parseUDP(const u_char *packetData)
{
    struct udphdr *uHead = (struct udphdr*) packetData;
    printf("src port: %d\n", ntohs(uHead->uh_sport));
    printf("dst port: %d\n", ntohs(uHead->uh_dport));
    printf("------------------------------------------------------------------\n");
}

void parseICMP(const u_char *packetData)
{
    printf("------------------------------------------------------------------\n");
}

void parseIPv4(const u_char *packetData)
{
    struct iphdr *iHead = (struct iphdr*) packetData;
    int headSize = iHead->ihl * 4; //words -> bytes
    printf("src IP: %s\n", inet_ntoa(*(struct in_addr*)&iHead->saddr));
    printf("dst IP: %s\n", inet_ntoa(*(struct in_addr*)&iHead->daddr));

    if(ntohs(iHead->protocol) == ntohs(IPPROTO_TCP))
    {
        parseTCP(packetData + headSize);
    }
    else if(ntohs(iHead->protocol) == ntohs(IPPROTO_UDP))
    {
        parseUDP(packetData + headSize);
    }
    else if(ntohs(iHead->protocol) == ntohs(IPPROTO_ICMP))
    {
        parseICMP(packetData + headSize);
    }
    else
    {
        printf("untracked IPv4 packet type\n");
        printf("------------------------------------------------------------------\n");
    }
}

void parseIPv6(const u_char *packetData)
{
    struct ip6_hdr *iHead = (struct ip6_hdr*) packetData;
    int headSize = 40;  //fixed size
    char result[100];
    ipv6_to_str_unexpanded(result, &iHead->ip6_src);
    printf("src IP: %s\n", result);
    ipv6_to_str_unexpanded(result, &iHead->ip6_dst);
    printf("dst IP: %s\n", result);

    int nextHeader = iHead->ip6_ctlun.ip6_un1.ip6_un1_nxt;  //gets next header

    if(nextHeader == IPPROTO_TCP)
    {
        parseTCP(packetData + headSize);
    }
    else if(nextHeader == IPPROTO_UDP)
    {
        parseUDP(packetData + headSize);
    }
    else if(nextHeader == IPPROTO_ICMPV6)
    {
        parseICMP(packetData + headSize);
    }
    else
    {
        printf("untracked IPv6 packet type\n");
        printf("------------------------------------------------------------------\n");
    }
}

/*
  * Author name : indiv
  * Date : Jan 19, 2011 at 18:06
  * Type : source code
  * Web address : https://stackoverflow.com/questions/4736718/mac-addresspad-missing-left-zeros
 */
char* ether_ntoa_rz(const struct ether_addr *addr)
{
    static char buf[18];
    sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x",
            addr->ether_addr_octet[0], addr->ether_addr_octet[1],
            addr->ether_addr_octet[2], addr->ether_addr_octet[3],
            addr->ether_addr_octet[4], addr->ether_addr_octet[5]);
    return buf;
}

void printTimestamp(const struct pcap_pkthdr *header)
{
    long int timestampSec = header->ts.tv_sec;
    struct tm *p = localtime(&timestampSec);
    char buf[30], timezone[7], milisecStr[10];
    int milisec = header->ts.tv_usec / 1000;    // microseconds -> miliseconds

    strftime(buf, sizeof(buf) - 1, "%FT%T", p);
    strftime(timezone, sizeof(timezone) - 1,"%z", p);
    timezone[5] = timezone[4];  // +0200 -> +02:00
    timezone[4] = timezone[3];  // +0200 -> +02:00
    timezone[3] = ':';  // +0200 -> +02:00
    timezone[6] = '\0'; // +0200 -> +02:00
    sprintf(milisecStr,".%03d",milisec);
    strcat(buf, milisecStr);
    strcat(buf, timezone);

    printf("timestamp: %s\n", buf);
}

void parsePacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packetData)
{
    struct ether_header *eHead = (struct ether_header*) packetData;
    printTimestamp(header);
    printf("src MAC: %s\n", ether_ntoa_rz((struct ether_addr*)eHead->ether_shost));
    printf("dst MAC: %s\n", ether_ntoa_rz((struct ether_addr*)eHead->ether_dhost));
    printf("frame length: %d bytes\n", header->len);

    int size = header->len;
    if(eHead->ether_type == ntohs(ETHERTYPE_IP))
    {
        parseIPv4(packetData + sizeof(struct ether_header)); //  + 14 bit offset
    }
    else if(eHead->ether_type == ntohs(ETHERTYPE_IPV6))
    {
        parseIPv6(packetData + sizeof(struct ether_header)); //  + 14 bit offset
    }
    else if(eHead->ether_type == ntohs(ETHERTYPE_ARP))
    {
        printf("------------------------------------------------------------------\n");
    }
    printPayload(packetData, size);
}

/*
  * Author name :
  * Date :
  * Title of source code :
  * Code version :
  * Type : source code
  * Web address :
 */
//https://www.tcpdump.org/other/sniffex.c
int main(int argc, char **argv)
{
    char interface[50], filter[50];;
    int interfaceFlag = 0;
    int numberOfPackets = 1;
    memset(filter, 0,strlen(filter));
    argParse(argc, argv, interface, filter, &numberOfPackets, &interfaceFlag);
    struct bpf_program fp;		// The compiled filter expression
    bpf_u_int32 mask;		// The netmask of our sniffing interface
    bpf_u_int32 net;		// The IP of our sniffing device

    if(!interfaceFlag) // if -i argument was not set
    {
        printInterface();
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];

    // get network number and mask associated with capture interface
    if (pcap_lookupnet(interface, &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Can't get netmask for device %s\n", interface);
        net = 0;
        mask = 0;
    }

    // open capture interface
    pcap_t* handle = pcap_open_live(interface, BUFSIZ, 1, 100, errbuf); // 1 = promisc mode
    if(handle == nullptr)
    {
        fprintf(stderr,"Error in starting packet capture\n");
        return 1;
    }

    // make sure we're capturing on an Ethernet device
    if (pcap_datalink(handle) != DLT_EN10MB) {
        fprintf(stderr, "%s is not an Ethernet\n", interface);
        return 1;
    }

    // compiling filter
    if (pcap_compile(handle, &fp, filter, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter, pcap_geterr(handle));
        return 1;
    }

    // applying filter
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handle));
        return 1;
    }

    // calling callback function parsePacket()
    if(pcap_loop(handle, numberOfPackets, parsePacket, NULL) < 0)   //main loop for parsing packets
    {
        fprintf(stderr,"Error during packet capturing\n");
        return 1;
    }

    //cleanup
    pcap_close(handle);
    pcap_freecode(&fp);
    return 0;
}

/*
 * sniffex.c
 *
 * Sniffer example of TCP/IP packet capture using libpcap.
 *
 * Version 0.1.1 (2005-07-05)
 * Copyright (c) 2005 The Tcpdump Group
 *
 * This software is intended to be used as a practical example and
 * demonstration of the libpcap library; available at:
 * http://www.tcpdump.org/
 *
 ****************************************************************************
 *
 * This software is a modification of Tim Carstens' "sniffer.c"
 * demonstration source code, released as follows:
 *
 * sniffer.c
 * Copyright (c) 2002 Tim Carstens
 * 2002-01-07
 * Demonstration of using libpcap
 * timcarst -at- yahoo -dot- com
 *
 * "sniffer.c" is distributed under these terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. The name "Tim Carstens" may not be used to endorse or promote
 *    products derived from this software without prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * <end of "sniffer.c" terms>
 *
 * This software, "sniffex.c", is a derivative work of "sniffer.c" and is
 * covered by the following terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Because this is a derivative work, you must comply with the "sniffer.c"
 *    terms reproduced above.
 * 2. Redistributions of source code must retain the Tcpdump Group copyright
 *    notice at the top of this source file, this list of conditions and the
 *    following disclaimer.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. The names "tcpdump" or "libpcap" may not be used to endorse or promote
 *    products derived from this software without prior written permission.
 *
 * THERE IS ABSOLUTELY NO WARRANTY FOR THIS PROGRAM.
 * BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
 * FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
 * PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
 * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
 * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
 * REPAIR OR CORRECTION.
 *
 * IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
 * WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
 * REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
 * INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
 * OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
 * TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
 * YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
 * PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 * <end of "sniffex.c" terms>
 *
 */