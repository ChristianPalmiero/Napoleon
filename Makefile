

all:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensors.c -o output/sensors.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c engines.c -o output/engines.o
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c main.c -o output/main.o
	gcc output/sensors.o output/engines.o output/main.o -Wall -lm -lev3dev-c -o output/main


clean:
	rm -f output/*

