IPK 2. projekt - Varianta ZETA: Sniffer paketů
Autor: Maxim Plička(xplick04)

Popis programu:
Program na základě zadaných parametrů bere pakety s ethernet hlavičkou o kterých vypíše čas, informace o odesílateli a příjemci,
délku rámce v bytech a samotná data daného paketu. Tento program zpracovává pouze pakety typu IPv4, IPv6 a ARP. U IPv4 a
IPv6 paketů dále zpracovává pouze pakety typu TCP, UDP a ICMP.

Volání programu:
./ipk-sniffer [-i rozhraní | --interface rozhraní] {-p ­­port} {[--tcp|-t] [--udp|-u] [--arp] [--icmp] } {-n num}

kde
    -i eth0 (právě jedno rozhraní, na kterém se bude poslouchat. Nebude-li tento parametr uveden, či bude-li uvedené jen -i bez hodnoty, vypíše se seznam aktivních rozhraní)
    -p 23 (bude filtrování paketů na daném rozhraní podle portu; nebude-li tento parametr uveden, uvažují se všechny porty; pokud je parametr uveden, může se daný port vyskytnout jak v source, tak v destination části)
    -t nebo --tcp (bude zobrazovat pouze TCP pakety)
    -u nebo --udp (bude zobrazovat pouze UDP pakety)
    --icmp (bude zobrazovat pouze ICMPv4 a ICMPv6 pakety)
    --arp (bude zobrazovat pouze ARP rámce)
    Pokud nebudou konkrétní protokoly specifikovány, uvažují se k tisknutí všechny (tj. veškerý obsah, nehledě na protokol)
    -n 10 (určuje počet paketů, které se mají zobrazit, tj. i "dobu" běhu programu; pokud není uvedeno, uvažujte zobrazení pouze jednoho paketu, tedy jakoby -n 1)
    argumenty mohou být v libovolném pořadí

Seznam souborů:
    ipk-sniffer.cpp
    README
    manual.pdf
    Makefile