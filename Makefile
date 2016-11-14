

all:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c tester.c -o tester.o
	gcc tester.o -Wall -lm -lev3dev-c -o tester

sensor: 
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c tester_sensors.c -o tester_sensors.o
	gcc tester_sensors.o -Wall -lm -lev3dev-c -o tester_sensors
run:
	./tester
