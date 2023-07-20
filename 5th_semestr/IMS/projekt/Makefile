CC := g++
FLAGS := -L/usr/local/lib -I/usr/local/include
SIMLIBFLAGS := -lsimlib -lm
EXEC := simulator

srcDir := $(PWD)/src
SOURCES := breakprocess.o chargerobotprocess.o genchargerobot.o packageprocess.o carprocess.o prevpackages.o argparser.o simulator.o

all: $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) $(SIMLIBFLAGS) -o $(EXEC)

simulator.o: $(srcDir)/simulator.cpp
	$(CC) $(FLAGS) $(srcDir)/simulator.cpp $(SIMLIBFLAGS) -c

argparser.o: $(srcDir)/argparser.cpp
	$(CC) $(FLAGS) $(srcDir)/argparser.cpp -c

carprocess.o: $(srcDir)/carprocess.cpp
	$(CC) $(FLAGS) $(srcDir)/carprocess.cpp $(SIMLIBFLAGS) -c

prevpackages.o: $(srcDir)/prevpackages.cpp
	$(CC) $(FLAGS) $(srcDir)/prevpackages.cpp $(SIMLIBFLAGS) -c

packageprocess.o: $(srcDir)/packageprocess.cpp
	$(CC) $(FLAGS) $(srcDir)/packageprocess.cpp $(SIMLIBFLAGS) -c

genchargerobot.o: $(srcDir)/genchargerobot.cpp
	$(CC) $(FLAGS) $(srcDir)/genchargerobot.cpp $(SIMLIBFLAGS) -c

chargerobotprocess.o: $(srcDir)/chargerobotprocess.cpp
	$(CC) $(FLAGS) $(srcDir)/chargerobotprocess.cpp $(SIMLIBFLAGS) -c

breakprocess.o: $(srcDir)/breakprocess.cpp
	$(CC) $(FLAGS) $(srcDir)/breakprocess.cpp $(SIMLIBFLAGS) -c

clean:
	rm -rf *.o
	rm -rf simulator
	rm -rf out.txt

clean_exp:
	rm -rf ./exps/*.png
	rm -rf ./exps/*.txt

run:
	./$(EXEC)

exp_1:
	python3 ./exps/exp_1.py 15 35 20 80 200

exp_2:
	python3 ./exps/exp_2.py 20 60

exp_3:
	python3 ./exps/exp_3.py 2000 15000