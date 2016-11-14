

all:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c tester.c -o output/tester.o
	gcc output/tester.o -Wall -lm -lev3dev-c -o output/tester

sensor: 
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c tester_sensors.c -o output/tester_sensors.o
	gcc output/tester_sensors.o -Wall -lm -lev3dev-c -o output/tester_sensors

engine:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c engine.c -o output/engine.o
	gcc output/engine.o -Wall -lm -lev3dev-c -o output/engine

run:
	./tester

clean:
	rm -f output/*

