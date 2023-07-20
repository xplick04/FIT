CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -pedantic -g -static-libstdc++
TARGET = feedreader
LDFLAGS = -L/usr/local/ssl/lib
LDLIBS = -lssl -lcrypto
XMLFLAGS = -lxml2
XMLCFLAGS!=pkg-config --cflags libxml-2.0
XMLCFLAGS?=$(shell pkg-config --cflags libxml-2.0)
SOURCES = argumentParser.o urlProcessor.o urlClass.o xmlParser.o

currDir=$(PWD)

all: argumentParser.o urlProcessor.o urlClass.o xmlParser.o feedreader.o
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) feedreader.o $(LDFLAGS) $(LDLIBS) $(XMLFLAGS) $(XMLCFLAGS)

feedreader.o: feedreader.cpp feedreader.hpp
	$(CC) $(CFLAGS) -c feedreader.cpp $(XMLFLAGS) $(XMLCFLAGS)

argumentParser.o: argumentParser.cpp argumentParser.hpp
	$(CC) $(CFLAGS) -c argumentParser.cpp

urlProcessor.o: urlProcessor.cpp urlProcessor.hpp
	$(CC) $(CFLAGS) -c urlProcessor.cpp $(XMLFLAGS) $(XMLCFLAGS)

urlClass.o: urlClass.cpp urlClass.hpp
	$(CC) $(CFLAGS) -c urlClass.cpp

xmlParser.o: xmlParser.cpp xmlParser.hpp
	$(CC) $(CFLAGS) -c xmlParser.cpp $(XMLFLAGS) $(XMLCFLAGS)

test:
	./tests/testScript.sh

clean:
	rm -f $(currDir)/*.o feedreader
