- ISA - Čtečka novinek ve formátu Atom a RSS s podporou TLS
- Autor: Maxim Plička(xplick04)
- Datum: 25.10 2022

Popis programu:
Cílem projektu bylo vytvořit konzolový program, který ze zdrojových URL adres
zpracuje jejich feed a následně vypíše na standartní výstup požadované infromace.
Podporované formáty feedu jsou atom a RSS 2.0. Jazyk zvolený pro vývoj tohoto
programu byl C++.

Návod na použití:
Projekt se překládá pomocí přikazu make. Testy se spouští pomocí příkazu make test.
 Mazání binárních souborů pomocí make clean.

Spouštění programu:
Program se spouští pomocí přikazu:

./feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u] [-h]

- [-h] - vypíše nápovědu
- URL - zdrojová URL
- -f <feedfile> - soubor, který v sobě má zdrojové URL adresy. Každá URL adresa
má svůj řádek a řádek je ukončen pomocí znaku LF. Pokud řádek začíná # je ignorován.
- -c <certfile> - soubor obsahující certifikáty pro ověření platnosti certifikátu serveru
- -C <certaddr> - adresář obsahující certifikáty pro ověření platnosti certifikátu serveru
- [-T] - vypíše datum poslední aktualizace u jednotlivých zdrojů
- [-a] - vypíše autora u jednotlivých zdrojů
- [-u] - vypíše asociované URL adresy u jednotlivých zdrojů
- Program lze spouštět i pomocí zadání argumentu -TaU

Seznam souborů:
argumentParser.cpp
argumentParser.hpp
feedreader.cpp
feedreader.hpp
urlClass.cpp
urlClass.hpp
urlProcessor.cpp
urlProcessor.hpp
xmlParser.cpp
xmlParser.hpp
Makefile
tests
tests/testScrypt.sh
tests/invalidURL
tests/invalidURL/invalidURL.txt
tests/invalidXML
tests/invalidXML/invalidXML.txt
tests/validAtom
tests/validAtom/validAtom.txt
tests/validRSS
tests/invalidXML/validRSS.txt
tests/validAtomRSS
tests/validAtomRSS/validAtomRSS.txt
